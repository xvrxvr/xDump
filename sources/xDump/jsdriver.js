console.log("JSDriver loading");

function Environment() {}

Environment.prototype = {
    logLevel : 1,
    logFile : './log.txt',
    loadConfig : function (file, section) {
        if (typeof(file) === 'undefined') file = '';
        if (typeof(section) === 'undefined') section = '';

        parseConfig(file, section);

        this.postLoad();
    },
    globObjects : {},
    addGlobObject : function (name, object) {
        if (name in this.globObjects)
            console.log("Warning. Variable already set.")

        this.globObjects[name] = object;
    },
    getGlobObject : function (name) {
        //if (name in this.globObjects)
            return this.globObjects[name];
    },
    /*getVar : function(name) {
        if (name in this) return this.name;
    },*/

    substituteString : function(str) {
        if (typeof(str) !== 'string')
            return str;

        var result = str;
        for (var key in this.globObjects) {
            tmp = '\\$\\(' + key + '\\)';
            re = new RegExp(tmp, 'g');
            result = result.replace(re, this.getGlobObject(key));
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

    isEOF : function() {}
}


/*--------------------------------------------------------------------------*/

function Executer() {}


Executer.prototype =  {
    config : new Object,
    exec : function (options) {
        var str = executeCommand('objdump', ['-x', env.substituteString('$(INP_FILE)')]);
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
            var tmp = this.data[i].replaceAll('&', '&amp;');
            tmp = tmp.replaceAll('<', '&lt;');
            //tmp = tmp.replaceAll('\n', '<br>\n');
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
