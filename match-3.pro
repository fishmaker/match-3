TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    gametable.cpp \
    gameitem.cpp \
    datareader.cpp

RESOURCES += qml.qrc \
    res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    gametable.h \
    gameitem.h \
    datareader.h

