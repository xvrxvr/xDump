#include <iostream>

#include <QDomNamedNodeMap>
#include <QDomDocument>
#include <QFile>
#include <QXmlQuery>

#include "error_handler.h"
#include "config_parser.h"

namespace xDump {

QString ConfigParser::defaultConfigFileName = ":/default_config.xml";

void ConfigParser::parseConfig(QString inputFile, QString section)
{
    QString fileToOpen = inputFile == "" ? defaultConfigFileName : inputFile;
    QFile file(fileToOpen);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text )) {
        ErrorHandler::reportError("Failed to open file for reading.",
                                  ErrorHandler::fatal, __FILE__, __LINE__);
        ErrorHandler::checkState();
        return;
    }

    QXmlQuery query;
    query.setFocus(&file);
    query.setQuery(section == "" ? "root" : section); // Example: query.setQuery("/root/skip[@section='ElfConfig']");
    if (!query.isValid()) {
        ErrorHandler::reportError("Invalid query for " + fileToOpen,
                                  ErrorHandler::fatal, __FILE__, __LINE__);
        ErrorHandler::checkState();
        return;
    }
    QString res;
    query.evaluateTo(&res);
    file.close();

    QDomDocument doc;
    if (!doc.setContent(res)) {
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
    name = attributesMap.namedItem("name").toAttr().value();
    type = attributesMap.namedItem("type").toAttr().value();
    method = attributesMap.namedItem("method").toAttr().value();
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
