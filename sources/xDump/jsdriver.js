console.log("JSDriver loading");

function Environment() {
    this.globObjects = {};
}

Environment.prototype = {
    logLevel : 1,
    logFile : './log.txt',
    loadConfig : function (file, section) {
        if (typeof(file) === 'undefined') file = '';
        if (typeof(section) === 'undefined') section = '';

        parseConfig(file, section);

        this.postLoad();
    },
    addGlobObject : function (name, object) {
        if (name in this.globObjects)
            console.log("Warning. Variable " + name + " already set.")

        this.globObjects[name] = object;
    },
    getGlobObject : function (name) {
        //if (name in this.globObjects)
            return this.globObjects[name];
    },
    /*getVar : function(name) {
        if (name in this) return this.name;
    },*/

    _replacer : function (regex, str, newSymbol) {
        re = new RegExp(regex, 'g');
        return str.replace(re, newSymbol);
    },

    substituteString : function(str) {
        if (typeof(str) !== 'string')
            return str;

        var result = str;
        result = this._replacer('\\$\\([0-9a-zA-Z\\_]+\\=([0-9a-zA-Z\\/\\.\\\\\\:\\-]*)\\)',
                                result, '$1');
        for (var key in this.globObjects) {
            result = this._replacer('\\$\\(' + key + '\\)', result, this.getGlobObject(key));
        }

        return result;
    },
    postLoad : function () {
        // default $(PATH) etc in config_parser.cpp
    }

}

// Global object Environment
var env = new Environment()

/*--------------------------------------------------------------------------*/

function ViewConfigSet() {}

ViewConfigSet.prototype = {

}

var viewConfigSet = new ViewConfigSet();

/*--------------------------------------------------------------------------*/
// Environment.LineStream = function (data) {...}
// Environment.LineStream.prototype.getLine() {...}

console.log("Loading LineStream");

function StreamError(message) {
    this.name = 'StreamError';
    this.message = message || 'Error occured!';
    this.stack = (new Error()).stack;
    this.getReason = function () {return 'EOF';}
}

StreamError.prototype = Object.create(Error.prototype);
StreamError.prototype.constructor = StreamError;


function LineStream(data) {
    this.data = data.split(/\r?\n/);
    this.index = 0;
}

LineStream.prototype = {
    getLine : function() {
        /*try {throw new StreamError();} catch(e) {console.log(e.message)}
        return 'Line got from objdump.';*/
        if(this.index + 1 > this.data.length)
            return new StreamError('EOF');

        this.index += 1;
        return this.data[this.index - 1];
    },

    getLines : function() {return this.data},


    kill : function() {this.data = '';},

    isEOF : function() {
        if(this.index + 1 >= this.data.length)
            return true;
        return false;
    }
}

/*--------------------------------------------------------------------------*/

function Executer() {}


Executer.prototype =  {
    config : new Object,

    exec : function (sectionName) {
        var fullExecPath = env.substituteString(executer.config.common[0] + "/" + executer.config.common[1])
        var argsStr = env.substituteString(this.config.sections[sectionName]).replace(/ +(?= )/g,'').trim()
        //console.log(fullExecPath)
        //console.log(argsStr)
        var str = executeCommand(fullExecPath, argsStr.split(' '));
        //console.log(str)
        //console.log("Objdump done.");

        return new LineStream(str);
    }
}

var executer = new Executer();

/*--------------------------------------------------------------------------*/
function ViewConfig(cfg) {
    if(typeof(cfg) === 'undefined') {this.config = {}; return;}
    this.config = cfg;
    this.postLoad();
}

ViewConfig.prototype = {
    /*config : {
        viewTranslatorConfig : {},
        viewTranslator : {
            getHeader   : function() {return 'dummy'},
            getFooter   : function() {return 'dummy'},
            getBody     : function() {return 'dummy'}
        }
    },*/
    postLoad : function() {
        if (!('viewTranslator' in this.config)) this.config.viewTranslator = PlainViewTranslator;
        if (!('execDriver' in this.config)) this.config.execDriver = SimpleExecDriver;
        this.execDriver = new this.config.execDriver(this);
        this.viewTranslator = new this.config.viewTranslator(this);
        if ('postLoad' in this.config.execDriver) this.config.execDriver.postLoad();
        if ('postLoad' in this.config.viewTranslator) this.config.viewTranslator.postLoad();
    },
    getViewTranslator : function(lines) {
        if (this.config.viewTranslator === 'undefined')
            console.log('Translator undefined.');

        //console.log(typeof(PlainViewTranslator));
        //console.log(typeof(this.config.viewTranslator));
        //this.config.viewTranslator = PlainViewTranslator;
        //console.log(typeof(this.config.viewTranslator));
        var vt = new this.config.viewTranslator(lines);
        //if ('postLoad' in vt) vt.postLoad();

        return vt;
    }
}


// Escapes signs in html
function escapeCommonSigns(str) {
    if (str === undefined) return str;

    var tmp = str.replaceAll('&', '&amp');
    tmp = tmp.replaceAll('<', '&lt;');
    return tmp;
}


String.prototype.replaceAll = function (sign, newSign) {
  var str = this + '';
  if (typeof(sign) === "string") {
    return str.split(sign).join(newSign);
  }
}


/*--------------------------------------------------------------------------*/
// Takes LineStream as arg -> html view
function PlainViewTranslator(data, config) {
    //assert(typeof(data) === 'array');
    this.data = data.getLines();
}

PlainViewTranslator.prototype = {
    getHeader : function () {return '<html><body>'},
    getFooter : function () {return '</body></html>'},
    getBody   : function () {
        // Escape symbols
        var result = '';
        for (var i in this.data) {
            var tmp = escapeCommonSigns(this.data[i]);
            tmp += '<br>';
            result += tmp;
        }

        return result;
    }
}

/*--------------------------------------------------------------------------*/

function SimpleExecDriver(objdump_options) {this.opts = objdump_options}

SimpleExecDriver.prototype = {
    exec : function () {
        this.lines = executer.exec(this.opts);
    },
    getLines : function () {return this.lines;}
}

console.log("JSDriver loaded");

/*--------------------------------------------------------------------------*/

function SectionsViewTranslator(data, config) {
    this.data = data.getLines();

    var tableFlag = false;
    var dataFlag = true;
    var header = false;
    for (var i in this.data) {
        this.data[i] = escapeCommonSigns(this.data[i]);
        if (!tableFlag) this.data[i] += '<br>';
        if (!tableFlag) this.data[i] = this.data[i].replace(/\,/g, "");
        if(this.data[i].match(/Sections/g) !== null) {

            tableFlag = true;
            continue;
        }
        if(tableFlag) {
            if (dataFlag) {
                var tmp = this.data[i].replace(/File off/g, 'File_off');
                tmp = tmp.replace(/\s+/g, ' ').trim();
                if (tmp === '')
                    continue;
                var section = tmp.split(' ', 2)[1]
                if (header) {
                    tmp = tmp.split(' ').join('</a></td><td><a href="qrc:///INTERNAL_LINK '+
                                              section + '">');

                    this.data[i] = '<tr><td><a href="qrc:///INTERNAL_LINK' +
                         section + '">' + tmp + '</a></td>';

                }
                else {
                    tmp = tmp.split(' ').join('</th><th>');
                    this.data[i] = '<tr><th>' + tmp + '</th></tr>';
                }

                if (header) {
                    dataFlag = false;
                }
                header = true;
            }
            else {
                this.data[i] = '<td>' + this.data[i] + '</td></tr>';
                dataFlag = true;
            }
        }
    }
}

SectionsViewTranslator.prototype = {
    getHeader : function () {
        return '<!DOCTYPE html>' +
                '<html>' +
                '<body>' +
                '<style>' +
                loadStyles('section_style.css') +
                '</style>'+
                '<table border="1" width="100%">';
    },
    getFooter : function () {
        return '</table>' +
                '</body>' +
                '</html>';
    },
    getBody   : function (from, to) {
        if (from === 'undefined' | to === 'undefined') {
            return this.data;
        }
        return this.data.slice(from, to);
    }
}

/*--------------------------------------------------------------------------*/

function getEvenOddElems(arr, odd) {
    res = new Array;
    for (var i = 0; i < arr.length; i++) {
        if(i % 2 === odd)
            res.push(arr[i]);
    }

    return res;
}

/*--------------------------------------------------------------------------*/

function ProgramViewTranslator(data, config) {
    this.data = data.getLines();
    var state = 0;

    var tableSet = false;
    var i = 0;
    while(i < this.data.length) {
        if (this.data[i].trim() === '') {i+=1; continue;}
        this.data[i] = escapeCommonSigns(this.data[i]);

        if(this.data[i].match(/Program Header/g) !== null) {
            this.data[i] = '<h2>' + this.data[i] + '</h2><br>';
            state = 1;
            tableSet = false;
            i += 1;
            continue;
        }
        if(this.data[i].match(/Dynamic Section/g) !== null) {
            this.data[i] = '</table><h2>' + this.data[i] + '</h2><br>';
            state = 2;
            tableSet = false;
            i += 1;
            continue;
        }
        if(this.data[i].match(/Version References/g) !== null) {
            this.data[i] = '</table><h2>' + this.data[i] + '</h2><br>';
            state = 3;
            tableSet = false;
            i += 1;
            continue;
        }

        if(state == 0) {this.data[i] += "<br>"; i += 1; continue;}

        if(state == 1) {
            var tmp = this.data[i].replace(/\s+/g, " ");
            tmp = tmp.replace(/([a-zA-Z]+)\soff/, "$1_off");
            tmp = tmp.trim().split(' ');
            if (!tableSet) {
                // Add class styles
                this.data[i] = "<table>";
                tableSet = !tableSet;
            } else this.data[i] = '';
            i += 1;
            var tmp1 = this.data[i].replace(/\s+/g, " ");
            tmp1 = tmp1.trim().split(' ');
            tmp = tmp.concat(tmp1);
            var head = getEvenOddElems(tmp, 0).join('</th><th>');
            var vals = getEvenOddElems(tmp, 1).join('</td><td>');
            this.data[i-1] += '<tr><th>' + head + '</th><tr>';
            this.data[i] = '<tr><td>' + vals + '</td></tr>';
        }

        if(state == 2) {
            var tmp = this.data[i].trim().replace(/\s+/g, " ");
            tmp = tmp.split(" ").join('</td><td>');
            this.data[i] = '<tr><td>' + tmp + '</td></tr>';
            if (!tableSet) {
                this.data[i] = "<table>" + this.data[i];
                tableSet = !tableSet;
            }
        }
        if(state == 3) {
            var tmp = this.data[i].trim().replace(/\s+/g, " ");
            if (tmp.match(/required from/g) !== null) {
                this.data[i] = '<tr><th colspan="4">' + tmp + '</th></tr>';
            }
            else {
                tmp = tmp.split(" ").join('</td><td>');
                this.data[i] = '<tr><td>' + tmp + '</td></tr>';
            }
            if (!tableSet) {
                this.data[i] = "<table>" + this.data[i];
                tableSet = !tableSet;
            }
        }
        i += 1;
    }
}

ProgramViewTranslator.prototype = {
    getHeader : function() {
        return '<!DOCTYPE html>' +
                '<html>' +
                '<body>' +
                '<style>' +
                loadStyles('section_style.css') +
                '</style>';
    },
    getFooter : function() {
        return '</body>' +
                '</html>';
    },
    getBody   : function() {return this.data;}
}


/*--------------------------------------------------------------------------*/
function highlightHeader(str) {
    if (str === undefined) return str;
    var result = '';
    result += '<span style="color: #000000; font-weight: bold;">';
    result += escapeCommonSigns(str);
    result += '</span>';
    return result;
}

function highlightLeftPane(str) {
    if (str === undefined) return str;
    var result = '';
    result += '<span style="color: #000000; ">';
    result += escapeCommonSigns(str);
    result += '</span>';
    return result;
}

function highlightCMD(str) {
    if (str === undefined) return str;
    var result = '';
    result += '<span style="color: #00007f; font-weight: bold;">';
    result += escapeCommonSigns(str);
    result += '</span>';
    return result;
}

function highlightARGS(arg_str) {
    if (arg_str === undefined) return arg_str;
    var result = '';
    if (arg_str.indexOf(',') > -1) { // Two args
        result += highlightARG(arg_str.split(',')[0]);
        result += ', ';
        result += highlightARG(arg_str.split(',')[1]);
    } else {
        result += highlightARG(arg_str); // One arg
    }
    return result;
}

function highlightARG(str) {
    var ret = str;
    // Hex numbers
    ret = ret.replace(/0x[0-9,a-f]+/gi, '<span style="color: #ff0000; ">$&</span>');
    // '(' and ')'
    ret = ret.replace(/[(,)]/gi, '<span style="color: #808030; ">$&</span>');
    // '$'
    ret = ret.replace(/$/gi, '<span style="color: #0000ff; font-weight: bold; ">$&</span>');
    // Registers
    ret = ret.replace(/%([0-9,a-z]+)/gi, '%<span style="color: #008000; font-weight: bold; ">$1</span>');
    return ret;
}

function AsmViewTranslator(data, config) {
    this.data = data.getLines();
}

AsmViewTranslator.prototype = {
    getHeader : function() {
        return '<!DOCTYPE html>' +
                '<html>' +
                '<body>';
    },
    getFooter : function() {
        return  '</body>' +
                '</html>';
    },
    getBody   : function () {
        var result = '';
        for (var i in this.data) {
            var splt = this.data[i].split('\t');

            // If that is not a command, just print it as-is:
            if (splt[0] === undefined || splt.length == 1) {
                result += highlightHeader(this.data[i]);
                result += '<br>\n';
                continue;
            }

            var left_pane = splt[0] + splt[1];
            result += highlightLeftPane(left_pane).replaceAll(' ', '&nbsp;');

            if (splt.length == 2) {
                result += '<br>\n';
                continue;
            }

            var cmd_str = splt[2];
            var arg_str = '';
            if (splt[2].indexOf(' ') > -1) {
                cmd_str = splt[2].substring(0, splt[2].indexOf(' '));
                arg_str = splt[2].substring(splt[2].indexOf(' ')).replace(/^\s*/, '').replace(/\s*$/, '');
             }

            result += highlightCMD(cmd_str) + ' ';
            result += highlightARGS(arg_str);
            result += '<br>\n';
        }

        return result;
    }
}


