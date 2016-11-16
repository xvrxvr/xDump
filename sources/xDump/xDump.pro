TEMPLATE = app

QT += qml quickcontrols2 webengine xml core
CONFIG += c++11

SOURCES += main.cpp \
    error_handler.cpp \
    config_parser.cpp \
    system_bridge.cpp

RESOURCES += qml.qrc \
    config.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    default_config.xml

HEADERS += \
    error_handler.h \
    config_parser.h \
    system_bridge.h
