#ifndef SYSTEM_EXECUTER_H
#define SYSTEM_EXECUTER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

namespace xDump {

class SystemExecuter : public QObject {
    Q_OBJECT

public:
    SystemExecuter () : QObject (), process (NULL) {}
    QString runCommand(QString execName, QStringList arguments);

private slots:
    void captureOutput();
    void captureError();

private:
    QProcess *process;
    QString output;
    QString error;
};

}
#endif // SYSTEM_EXECUTER_H
