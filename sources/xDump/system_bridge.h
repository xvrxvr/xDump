#ifndef SYSTEM_BRIDGE_H
#define SYSTEM_BRIDGE_H

#include <QObject>

#include "config_parser.h"

namespace xDump {

class SystemBridge : public QObject {
    Q_OBJECT
public:
    SystemBridge(ConfigParser &_parser) : QObject (), parser(_parser) {}
    Q_INVOKABLE void transferToParser(QString file = "", QString section = "");

private:
    ConfigParser &parser;
};

}
#endif // SYSTEM_BRIDGE_H
