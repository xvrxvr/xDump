#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>

#include <error_handler.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QtWebEngine::initialize();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
