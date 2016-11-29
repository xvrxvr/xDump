#include <iostream>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QByteArray>

#include "system_executer.h"

namespace xDump {

bool SystemExecuter::runCommand(QString execName, QStringList arguments)
{
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
    QString output (byteArray);
    std::cout << qPrintable(output) << std::endl;
}

void SystemExecuter::captureError()
{
    QByteArray byteArray = process->readAllStandardError();
    QString error (byteArray);
    std::cout << "ERROR:\n" << qPrintable(error) << std::endl;
}

}
