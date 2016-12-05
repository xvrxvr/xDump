TEMPLATE = app

QT += qml quickcontrols2 webengine xml core xmlpatterns
CONFIG += c++11

SOURCES += main.cpp \
    error_handler.cpp \
    config_parser.cpp \
    system_bridge.cpp \
    system_executer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!android: copydata.commands = $(COPY_DIR) $$shell_path($$PWD/configs) $$shell_path($$OUT_PWD)
else:copydata.commands = $(COPY_DIR) $$shell_path($$PWD/configs) $$shell_path($$OUT_PWD/configs)
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

DISTFILES += \
    configs/default_config.xml

HEADERS += \
    error_handler.h \
    config_parser.h \
    system_bridge.h \
    system_executer.h
