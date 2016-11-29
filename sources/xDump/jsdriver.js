
function ViewConfig(cfg) {
    if(typeof(cfg) === 'undefined') {this.config = {}; return;}
    this.config = cfg;
}

function Config() {}
function Executer() {}


Executer.prototype =  {
    config : {},
    exec : function (options) {}
}

ViewConfig.prototype = {
    viewTranslator : function () {

    },
    execDriver : function () {}
}

vcfg = new ViewConfig();

