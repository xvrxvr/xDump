#ifndef SYSTEM_BRIDGE_H
#define SYSTEM_BRIDGE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "config_parser.h"
#include "system_executer.h"

namespace xDump {

class SystemBridge : public QObject {
    Q_OBJECT
public:
    SystemBridge (QJSEngine &_jsEngine) : jsEngine(_jsEngine), parser(_jsEngine) {}
    Q_INVOKABLE void transferToParser(QString file = "", QString section = "");
    Q_INVOKABLE void executeCommand(QString execName, QStringList arguments);
    Q_INVOKABLE QString getOutput();
    Q_INVOKABLE QString getError();

private:
    QJSEngine &jsEngine;
    ConfigParser parser;
    SystemExecuter executer;
};

}
#endif // SYSTEM_BRIDGE_H
