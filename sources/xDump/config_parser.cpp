#include <iostream>

#include <QDomNamedNodeMap>
#include <QDomDocument>
#include <QFile>

#include <error_handler.h>
#include <config_parser.h>

namespace xDump {
QString ConfigParser::defaultConfigFileName = ":/default_config.xml";

void ConfigParser::parseConfig(QString inputFile)
{
    QDomDocument doc("mydocument");
    QFile file(inputFile == "" ? defaultConfigFileName : inputFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text )) {
        ErrorHandler::reportError("Failed to open file for reading.",
                                  ErrorHandler::fatal, __FILE__, __LINE__);
        ErrorHandler::checkState();
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        ErrorHandler::reportError("Failed to parse the file into a DOM tree.",
                                  ErrorHandler::fatal, __FILE__, __LINE__);
        ErrorHandler::checkState();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    parseXmlElements(docElem, ConfigElementAttributes());
}

void ConfigParser::parseXmlElements(QDomNode docElem, ConfigElementAttributes parentAttributes)
{
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        //std::cout << "Node: " << qPrintable(n.nodeName()) << std::endl;
        //std::cout << "Child: " << n.hasChildNodes() << " : " << n.lineNumber() << " : " << n.columnNumber() << std::endl;
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if (e.tagName() == "field") {
                //std::cout << "Tag: " << qPrintable(e.tagName()) << std::endl;
                //std::cout << "Child: " << n.hasChildNodes() << std::endl;
                //ConfigElementAttributes(parentAttributes, e.attributes()).dump();
                //std::cout << "FText: " << qPrintable(e.text()) << std::endl;
                QDomNode childNode = n.firstChild();
                //std::cout << "childNode: " << childNode.isNull() << " " << qPrintable(childNode.nodeName()) << std::endl;
                QString childText = "";
                if (!childNode.isNull() && !childNode.toText().isNull())
                    childText = childNode.toText().data();
                addXmlToJsEngine(ConfigElementAttributes(parentAttributes, e.attributes()), childText);
                parseXmlElements(n, ConfigElementAttributes(parentAttributes, e.attributes()));
            } else if (e.tagName() == "JS") {
                QDomNode childNode = n.firstChild();
                QString childText = "";
                if (childNode.isNull() || childNode.toText().isNull()) {
                    ErrorHandler::reportError("JS tag should have text",
                                              ErrorHandler::fatal, __FILE__, __LINE__);
                }
                childText = childNode.toText().data();
                jsEngine.evaluate(childText);
            } else if (e.tagName() == "skip") {

            } else {
                ErrorHandler::reportError("Unrecognuzed tag name: " + e.tagName(),
                                          ErrorHandler::fatal, __FILE__, __LINE__);
            }
        }
        //QDomText t = n.toText();
        //if(!t.isNull()) {
        //    std::cout << "Has text: " << qPrintable(n.nodeName()) << std::endl;
            //    addXmlToJsEngine(parentAttributes, t.data());
        //}
        n = n.nextSibling();
    }
}

void ConfigParser::addXmlToJsEngine(ConfigElementAttributes attributes, QString text)
{
    //std::cout << "List: " << qPrintable(attributes.qGetName()) << std::endl;
    //Check whether all nodes exist in hierarchy
    QStringList parentObjectHierarchy = attributes.qGetName().split(".");
    QJSValue parentObject = jsEngine.globalObject();
    for (int i = 0; i < parentObjectHierarchy.size() - 1; ++i) {
        if (!parentObject.hasProperty(parentObjectHierarchy.at(i))) {
            ErrorHandler::reportError("Can't find property " + parentObjectHierarchy.at(i).toLatin1(),
                                      ErrorHandler::fatal, __FILE__, __LINE__);
            return;
        }
        parentObject = parentObject.property(parentObjectHierarchy.at(i));
    }
    std::cout << qPrintable(parentObjectHierarchy.last()) << std::endl;

    QJSValue lastSuccessor = parentObject;
    if (attributes.qGetMethod() == "add") {
        if (parentObject.hasProperty(parentObjectHierarchy.last())) {
            lastSuccessor = parentObject.property(parentObjectHierarchy.last());
            QJSValue instanceof = jsEngine.evaluate(attributes.qGetName() + " instanceof " + attributes.qGetType());
            if (!instanceof.toBool()) {
                ErrorHandler::reportError("Object " + attributes.qGetName() + " should be instanceof " + attributes.qGetType(),
                                          ErrorHandler::fatal, __FILE__, __LINE__);
            }
            return;
        } else {
            lastSuccessor.setProperty(parentObjectHierarchy.last(),
                                      jsEngine.evaluate("new " + attributes.qGetType().toLatin1() + "()"));
        }
        return;
    }

    if (attributes.qGetMethod() == "assign") {
        std::cout << "type: " << qPrintable(attributes.qGetType()) << std::endl;
        std::cout << "text: " << qPrintable(text) << std::endl;
        if (attributes.qGetType() == "JS") {
            //TODO: check evaluation
            lastSuccessor.setProperty(parentObjectHierarchy.last(), jsEngine.evaluate(text));
            return;
        } else {
            if (attributes.qGetType() == "String")
                text = "'" + text + "'";
            if (attributes.qGetType() == "Array")
                text = "[" + text + "]";
            lastSuccessor.setProperty(parentObjectHierarchy.last(),
                                     jsEngine.evaluate("new " + attributes.qGetType().toLatin1() + "(" + text + ")"));
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConfigParser::ConfigElementAttributes::ConfigElementAttributes(QDomNamedNodeMap attributesMap)
{
    //std::cout << "Name: " << qPrintable(elem.tagName()) << std::endl;
    //std::cout << "Text: " << qPrintable(elem.text()) << std::endl;
    for (auto i = 0; i < attributesMap.length(); ++i) {
        QDomNode attrNode = attributesMap.item(i);
        if (attrNode.isAttr()) {
            QDomAttr attr = attrNode.toAttr();
            //std::cout << "Field: " << qPrintable(attr.name()) <<  " = " << qPrintable(attr.value()) << std::endl;
            if (attr.name() == "name")
                name = attr.value();
            if (attr.name() == "type")
                type = attr.value();
            if (attr.name() == "method")
                method = attr.value();
        }
    }
}

ConfigParser::ConfigElementAttributes::ConfigElementAttributes(ConfigElementAttributes parentAttributes,
                                                 QDomNamedNodeMap attributesMap) :
    ConfigElementAttributes(attributesMap)
{
    if (!parentAttributes.qGetName().isEmpty())
        name = parentAttributes.qGetName() + "." + name;
}

QString ConfigParser::ConfigElementAttributes::qGetName()
{
    return name;
}

QString ConfigParser::ConfigElementAttributes::qGetType()
{
    return type;
}

QString ConfigParser::ConfigElementAttributes::qGetMethod()
{
    return method;
}

void ConfigParser::ConfigElementAttributes::dump()
{
    std::cout << "Name: "   << qPrintable(name) << std::endl;
    std::cout << "Type: "   << qPrintable(type) << std::endl;
    std::cout << "Method: " << qPrintable(method) << std::endl;
}


}
