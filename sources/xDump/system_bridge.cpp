#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "config_parser.h"
#include "system_bridge.h"
#include "error_handler.h"

namespace xDump {

void SystemBridge::transferToParser(QString file, QString section){
    parser.parseConfig(file, section);
}

QString SystemBridge::executeCommand(QString execName, QStringList arguments)
{
    return executer.runCommand(execName, arguments);
}

QString SystemBridge::wrapFileUrl (QUrl fileUrl)
{
    return fileUrl.toLocalFile();
}

void SystemBridge::addToBuffer (QString key, QString data)
{
    htmlBuffer[key.toStdString()] = data.toStdString();
}

QString SystemBridge::getFromBuffer (QString key)
{
    return QString::fromStdString(htmlBuffer[key.toStdString()]);
}

QString SystemBridge::loadStyles (QString cssFile)
{
    if (cssFile == "")
        return "";

    QString fileToOpen = QString("configs") + QDir::separator() + cssFile;
    QFile file(fileToOpen);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text )) {
        PrintError("Failed to open file for reading.", ErrorHandler::fatal);
        ErrorHandler::checkState();
        return "";
    }

    QTextStream in(&file);

    QString res = in.readAll();

    file.close();
    return res;
}

}
