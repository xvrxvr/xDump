#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>
#include <QtWebView/QtWebView>
#include <QtQuickControls2/QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QtWebEngine::initialize();
    QQuickStyle::setStyle("Default");
    //QQuickStyle::setStyle("Universal");
    //QtWebView::initialize();
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
