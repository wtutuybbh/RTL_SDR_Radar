#include <QApplication>
#include <QDir>
#include <QProcessEnvironment>
#include <QCommandLineParser>
#include <signal.h>
#include <QTranslator>
#include "AppCore/Core.h"

int main(int argc, char *argv[])
{
    QTranslator qtLanguageTranslator;
    QApplication a(argc, argv);

    // парсер аргументов командной строки
    QApplication::setApplicationName("RadarApp");
    QApplication::setApplicationVersion("1.0");


    signal(SIGPIPE, SIG_IGN);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main",
                                                                 "RadarApp"));

    parser.addPositionalArgument("ip",
                                 QCoreApplication::translate("main",
                                                             "IP-address to connect to the server"));
    parser.addPositionalArgument("port",
                                 QCoreApplication::translate("main",
                                                             "port to connect to the server"));

    parser.process(a);

    //TODO: добавить проверку на наличие всех параметров командной строки
    const QStringList args = parser.positionalArguments();

    QString strIp = QString();
    uint16_t port = 0;
    int64_t interval = 3000;
    Core core;
    if(!args.isEmpty() && args.size() >= 2)
    {
        strIp = args.at(0);
        port = args.at(1).toUShort();
        if(args.size() == 3)
            interval = args.at(2).toLong();
        qDebug()<<"run with param:"<<strIp<<":"<<port<<" period = "<<interval;
        core.init(strIp,port,interval);
    }
    else
        core.init();

    return a.exec();
}
