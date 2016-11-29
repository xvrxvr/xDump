
function LineStream() {}

LineStream.prototype = {
    getLine : function() {},
    getLines : function() {},
    data : ''
}

// Global?
dataContainer = new LineStream();

function ViewConfig(cfg) {
    if(typeof(cfg) === 'undefined') {this.config = {}; return;}
    this.config = cfg;
}

function Executor() {}


Executor.prototype =  {
    config : {
        common : ['$(PATH)', '$(EXE_NAME=objdump)'],
        sections : {
            globHeader : '-h $(OPT=) $(INP_FILE)',
            //...
        }
    },
    exec : function (options) {
        // Run objdump here
        dataContainer.data = 'objdump >';

        return ;
    },

}


ViewConfig.prototype = {
    viewTranslatorConfig : {
        // From config file?
        header : '<html>',
        footer : '</html>'
    },
    viewTranslator : {
        getHeader   : function() {return '<html><body>'/*this.viewTranslatorConfig.header; ???*/;},
        getFooter   : function() {return '</body></html>';},
        getBody     : function() {
            var body = 'body';
            return body;
        }
    },
    execDriver : function () {}
}

vcfg = new ViewConfig();

