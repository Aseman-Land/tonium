#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "qtquick/cpp/tonqtquick.h"
#include "qtquick/cpp/toolkit/core/tontoolkitapplicationitem.h"

int main(int argc, char *argv[])
{
    qputenv("QT_ANDROID_ENABLE_WORKAROUND_TO_DISABLE_PREDICTIVE_TEXT", "1");
    qputenv("QT_LOGGING_RULES", "qt.qml.connections=false");

#if !defined(Q_OS_LINUX) || defined(Q_OS_ANDROID)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#ifdef Q_OS_ANDROID
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Ceil);
#endif
#endif

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/ton/common/icons/icon.png"));

    TonToolkitApplicationItem::setApplicationId( QStringLiteral("8c37fdef-2156-458e-ae82-6c7aad1078b3") );
    if (TonToolkitApplicationItem::isRunning())
    {
        if (app.arguments().count() == 2 && app.arguments().at(1).left(6) == QStringLiteral("ton://"))
            TonToolkitApplicationItem::sendMessage(app.arguments().at(1));
        else
            qDebug() << "is running";
        return 0;
    }

    TonQtQuick::registerToolkit();
    TonQtQuick::registerViewport();

    if (app.arguments().contains(QStringLiteral("--widgets")))
    {
        qDebug() << "Under construction!";
        return 0;
    }
    else
    {
        QQmlApplicationEngine engine;

        engine.rootContext()->setContextProperty("testMode", qEnvironmentVariable("TON_TEST_MODE") == QStringLiteral("1"));
        if (app.arguments().count() == 2 && app.arguments().at(1).left(6) == QStringLiteral("ton://"))
            engine.rootContext()->setContextProperty("linkToOpen", app.arguments().at(1));
        else
            engine.rootContext()->setContextProperty("linkToOpen", QString());

        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
    }
}
