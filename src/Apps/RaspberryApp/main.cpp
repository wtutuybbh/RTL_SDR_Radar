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

    QString strIp = QString("127.0.0.1");
    uint16_t port = 62000;
    int64_t interval = 3000;
    Core core;

    qDebug()<<"run with param:"<<strIp<<":"<<port<<" period = "<<interval;
    core.init(strIp,port,interval);

    return a.exec();
}
