#include "options-handler.hpp"
#include "provider.hpp"
#include "parser-wrapper.hpp"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    detail::OptionsHandler options {argc, argv};

    qmlRegisterUncreatableType<Provider>("Provider", 1, 0, "Provider", "");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    ParserWrapper wrapper {options.filePath, options.countPage, options.singleThread};
    engine.rootObjects().first()->setProperty("provider",
                                              QVariant::fromValue(qobject_cast<QObject *>
                                                                  (&wrapper.getProvider())));
    return app.exec();
}
