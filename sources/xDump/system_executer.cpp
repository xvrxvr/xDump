#include <iostream>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QByteArray>

#include "system_executer.h"

namespace xDump {

bool SystemExecuter::runCommand(QString execName, QStringList arguments)
{
    output = QString();
    error  = QString();

    process = new QProcess();

    connect (process, &QProcess::readyReadStandardOutput, this, &SystemExecuter::captureOutput);
    connect (process, &QProcess::readyReadStandardError, this, &SystemExecuter::captureError);

    process->start(execName, arguments);
    process->waitForFinished(-1);
    return true;
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

QString SystemExecuter::getOutput()
{
    return output;
}

QString SystemExecuter::getError()
{
    return error;
}

}
