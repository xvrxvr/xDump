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
    ErrorHandler::checkState();
}

void ConfigParser::parseXmlElements(QDomNode docElem, ConfigElementAttributes parentAttributes)
{
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if (e.tagName() == "field") {
                //Check whether the node has text. We need it for "assign" method.
                QDomNode childNode = n.firstChild();
                QString childText = "";
                if (!childNode.isNull() && !childNode.toText().isNull())
                    childText = childNode.toText().data();
                addXmlToJsEngine(ConfigElementAttributes(parentAttributes, e.attributes()), childText);
                parseXmlElements(n, ConfigElementAttributes(parentAttributes, e.attributes()));
            } else if (e.tagName() == "JS") {
                //Get text from child node and evaluate it
                QDomNode childNode = n.firstChild();
                QString childText = "";
                if (childNode.isNull() || childNode.toText().isNull()) {
                    ErrorHandler::reportError("JS tag should have text",
                                              ErrorHandler::fatal, __FILE__, __LINE__);
                }
                childText = childNode.toText().data();
                jsEngine.evaluate(childText);
            } else if (e.tagName() == "skip") {
                //TODO
            } else {
                ErrorHandler::reportError("Unrecognuzed tag name: " + e.tagName(),
                                          ErrorHandler::fatal, __FILE__, __LINE__);
            }
        }
        n = n.nextSibling();
    }
}

void ConfigParser::addXmlToJsEngine(ConfigElementAttributes attributes, QString text)
{
    //Check whether all parent nodes exist in hierarchy
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

    QJSValue lastSuccessor = parentObject;
    if (attributes.qGetMethod() == "add") {
        //If object exists, it should have specified type. Otherwise, we create it.
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
        if (attributes.qGetType() == "JS") {
            //If type is JS - just evaluate text
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
    for (auto i = 0; i < attributesMap.length(); ++i) {
        QDomNode attrNode = attributesMap.item(i);
        if (attrNode.isAttr()) {
            QDomAttr attr = attrNode.toAttr();
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

}
