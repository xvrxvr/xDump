#include <iostream>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QByteArray>

#include "system_executer.h"

namespace xDump {

QString SystemExecuter::runCommand(QString execName, QStringList arguments)
{
    output = QString();
    error  = QString();

    process = new QProcess();

    connect (process, &QProcess::readyReadStandardOutput, this, &SystemExecuter::captureOutput);
    connect (process, &QProcess::readyReadStandardError, this, &SystemExecuter::captureError);

    process->start(execName, arguments);
    process->waitForFinished(-1);

    if (!error.isEmpty())
        return error;
    return output;
}

void SystemExecuter::captureOutput()
{
    QByteArray byteArray = process->readAllStandardOutput();
    output += QString(byteArray);
}

void SystemExecuter::captureError()
{
    QByteArray byteArray = process->readAllStandardError();
    error += QString(byteArray);
}

}
