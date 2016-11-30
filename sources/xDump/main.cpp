#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>
#include <QTextStream>

#include "error_handler.h"
#include "config_parser.h"
#include "system_bridge.h"
#include "system_executer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QtWebEngine::initialize();

    // Create systemBridge and export functions from it to jsEngine
    xDump::SystemBridge systemBridge (engine);
    QJSValue globalObject = engine.globalObject();
    QJSValue jsSystemBridge = engine.newQObject(&systemBridge);
    QJSValue jsParseConfig = jsSystemBridge.property("transferToParser");
    globalObject.setProperty("parseConfig", jsParseConfig);
    QJSValue jsExecuteCommand = jsSystemBridge.property("executeCommand");
    globalObject.setProperty("executeCommand", jsExecuteCommand);
    QJSValue jsGetOutput = jsSystemBridge.property("getOutput");
    globalObject.setProperty("getOutput", jsGetOutput);
    QJSValue jsGetError = jsSystemBridge.property("getError");
    globalObject.setProperty("getError", jsGetError);

    // Load JS script to jsEngine
    QString fileName = ":/jsdriver.js";
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly))
        xDump::PrintError("Can't read JS script", xDump::ErrorHandler::internal);
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    engine.evaluate(contents, fileName);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
