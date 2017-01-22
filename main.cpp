#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "gametable.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<GameTable>("com.apps.gametable", 1, 0, "GameTable");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

