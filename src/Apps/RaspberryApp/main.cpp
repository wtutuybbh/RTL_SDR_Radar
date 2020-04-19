#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <signal.h>
#include "core/Core.h"
#include "ui/Mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // парсер аргументов командной строки
    QApplication::setApplicationName("RaspberryApp");
    QApplication::setApplicationVersion("1.0");

    signal(SIGPIPE, SIG_IGN);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main",
                                                                 "RaspberryApp"));

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

    Core core;
    if(!args.isEmpty() && args.size() >= 2)
    {
        strIp = args.at(0);
        port = args.at(1).toUShort();
        qDebug()<<strIp<<port;
        core.init(strIp,port);
    }
    else
        core.init();

    return a.exec();
}
