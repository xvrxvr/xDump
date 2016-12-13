#ifndef SYSTEM_BRIDGE_H
#define SYSTEM_BRIDGE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>

#include "config_parser.h"
#include "system_executer.h"

namespace xDump {

class SystemBridge : public QObject {
    Q_OBJECT
public:
    SystemBridge (QJSEngine &_jsEngine) : jsEngine(_jsEngine), parser(_jsEngine) {}
    Q_INVOKABLE void transferToParser(QString file = "", QString section = "");
    Q_INVOKABLE QString executeCommand(QString execName, QStringList arguments);
    Q_INVOKABLE QString wrapFileUrl (QUrl fileUrl);
    Q_INVOKABLE void    addToBuffer (QString key, QString data);
    Q_INVOKABLE QString getFromBuffer (QString key);
    Q_INVOKABLE QString loadStyles  (QString fileUrl = "");

private:
    QJSEngine &jsEngine;
    ConfigParser parser;
    SystemExecuter executer;
    std::map<std::string, std::string> htmlBuffer;
};

}
#endif // SYSTEM_BRIDGE_H
