#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>

#include "error_handler.h"
#include "config_parser.h"
#include "system_bridge.h"
#include "system_executer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QtWebEngine::initialize();

    // Create global executer.config Object.
    //TODO: Delete this section later!
    QJSValue executer = engine.newObject();
    executer.setProperty("executer", engine.evaluate("new Object()"));
    QJSValue globalObject = engine.globalObject();
    globalObject.setProperty("executer", executer);
    QJSValue config = engine.newObject();
    config.setProperty("config", engine.evaluate("new Object()"));
    QJSValue executerObject = engine.globalObject().property("executer");
    executerObject.setProperty("config", config);

    xDump::ConfigParser configParser(engine);
    xDump::SystemExecuter systemExecuter;
    xDump::SystemBridge systemBridge (configParser, systemExecuter);

    //systemBridge.executeCommand("", QStringList());

    QJSValue jsSystemBridge = engine.newQObject(&systemBridge);
    QJSValue jsParseConfig = jsSystemBridge.property("transferToParser");
    globalObject.setProperty("parseConfig", jsParseConfig);
    QJSValue jsExecuteCommand = jsSystemBridge.property("executeCommand");
    globalObject.setProperty("executeCommand", jsExecuteCommand);

    configParser.parseConfig();

    //engine.evaluate("executeCommand('objdump', ['-D', 'xDump'])");

    //xDump::SystemExecuter systemExecuter;
    //systemExecuter.runCommand("ls", QStringList("-la"));


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
