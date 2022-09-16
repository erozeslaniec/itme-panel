#include <QApplication>
#include <QQmlContext>
#include "SLMPProtocol.h"
#include <QThread>
#include <QQmlApplicationEngine>
//#include "kyeboardSrc/register.h"


//#define CAMERA_ENABLED //comment this line to disable camera








#include <QtWebEngine/QtWebEngine>

int main(int argc, char *argv[]){
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //registerQmlTypes();


    QApplication app(argc, argv);
    CSLMPProtocol SLMPProtocol;
    SLMPProtocol.takeRecipeSchemaFromFile("recipeSchema.txt");
    QQmlApplicationEngine engine;

    SLMPProtocol.startThread();
    engine.rootContext()->setContextProperty("PLC",&SLMPProtocol);
    engine.rootContext()->setContextProperty("alarmListModel",&SLMPProtocol.alarmList);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
