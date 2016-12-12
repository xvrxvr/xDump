function SectionsViewTranslator(data, config) {
    this.data = data.getLines();

}

SectionsViewTranslator.prototype = {
    getHeader : function () {
        return '<!DOCTYPE html>' +
                '<html>' +
                '<body>' +
                '<table>';
    },
    getFooter : function () {
        return '</table>' +
                '</body>' +
                '</html>';
    },
    getBody   : function () {}
}

/*--------------------------------------------------------------------------*/
