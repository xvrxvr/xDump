#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>

#include <error_handler.h>
#include <config_parser.h>

int main(int argc, char *argv[])
{
    //QGuiApplication app(argc, argv);
    QCoreApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //QtWebEngine::initialize();

    // Create global executer Object
    QJSValue executer = engine.newObject();
    executer.setProperty("executer", engine.evaluate("new Object()"));
    QJSValue globalObject = engine.globalObject();
    globalObject.setProperty("executer", executer);

    QJSValue config = engine.newObject();
    config.setProperty("config", engine.evaluate("new Object()"));
    QJSValue executerObject = engine.globalObject().property("executer");
    executerObject.setProperty("config", config);

    xDump::ConfigParser configParser(engine);
    configParser.parseConfig();
    engine.dumpObjectTree();
    engine.installExtensions(QJSEngine::ConsoleExtension);
    engine.evaluate("console.log(executer.config.sections.globHeader)");

    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
