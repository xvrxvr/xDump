#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>

#include "config_parser.h"
#include "system_bridge.h"

namespace xDump {

void SystemBridge::transferToParser(QString file, QString section){
    parser.parseConfig(file, section);
}

void SystemBridge::executeCommand(QString execName, QStringList arguments)
{
    executer.runCommand(execName, arguments);
}

QString SystemBridge::getOutput()
{
    return executer.getOutput();
}

QString SystemBridge::getError()
{
    return executer.getError();
}


}
