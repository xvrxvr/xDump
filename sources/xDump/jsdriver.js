
function Environment() {}

Environment.prototype = {
    logLevel : 1,
    logFile : './log.txt',
    loadConfig : function (file, section) {
        if (typeof(file) === 'undefined') file = '';
        if (typeof(section) === 'undefined') section = '';
        parseConfig(file, section)
    }
}

// Global object Environment
var env = new Environment()

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


function LineStream() {}

LineStream.prototype = {
    getLine : function() {
        try {throw new StreamError();} catch(e) {console.log(e.message)}
        return 'Line got from objdump.';
    },
    getLines : function() {},

    kill : function() {this.data = '';}
}


/*--------------------------------------------------------------------------*/

function Executer() {}


Executer.prototype =  {
    config : new Object,
    exec : function (options) {
        // Run objdump here
        var result = new LineStream()

        return ;
    },

}

var executer = new Executer();

/*--------------------------------------------------------------------------*/
function ViewConfig(cfg) {
    if(typeof(cfg) === 'undefined') {this.config = {}; return;}
    this.config = cfg;

    this.execDriver = function () {
        this.inputStream = executer.exec();
    }

    // Legit ?
    this.execDriver.getLines = function() {
        return this.inputStream;
    }
}

// Or this >>>
/*ViewConfig.execDriver.getLines.prototype = function () {
    return this.inputStream
};*/

ViewConfig.prototype = {
    viewTranslatorConfig : {
        // From config file?
        header : '<html>',
        footer : '</html>'
    },
    viewTranslator : {
        getHeader   : function() {
            return '<html><body>';
            /*this.viewTranslatorConfig.header; ???*/
        },
        getFooter   : function() {return '</body></html>';},
        getBody     : function() {
            var body = 'LineStreamEx object TBD';
            return body;
        }
    }
}

var vcfg = new ViewConfig();

