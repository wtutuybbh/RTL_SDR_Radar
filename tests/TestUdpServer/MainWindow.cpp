#include <QDebug>
#include <QNetworkDatagram>
#include <QNetworkInterface>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <string>
#include <cstdint>
#include <algorithm>
#include <stdlib.h>


uint64_t convert_mac(std::string mac) {
  // Remove colons
  mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());

  // Convert to uint64_t
  return strtoul(mac.c_str(), NULL, 16);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _socket = new QUdpSocket();
    qDebug()<<"bind socket "<< _socket->bind(QHostAddress("127.0.0.1"),62000);

    QObject::connect(_socket,&QUdpSocket::readyRead,this, &MainWindow::slotReadyRead);

//    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
//        {
//            // Return only the first non-loopback MAC Address
//            if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
//                if(netInterface.type() == QNetworkInterface::InterfaceType::Ethernet /*||
//                   netInterface.type() == QNetworkInterface::InterfaceType::Wifi*/)
//                qDebug()<< netInterface.hardwareAddress();
//            qDebug()<< convert_mac(netInterface.hardwareAddress().toStdString());
//        }

}



MainWindow::~MainWindow()
{
    _socket->close();
    delete _socket;

    delete ui;
}


void MainWindow::slotReadyRead()
{
    int64_t ret = _socket->bytesAvailable();
    while(_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        _socket->readDatagram(datagram.data(),
                                 datagram.size(),
                                 &sender,
                                 &senderPort);
         qDebug()<<"NetworkWorker::readDatagramm Read size=" << datagram.size() <<" byte from host ="<<sender<<" port="<<senderPort;



    }
}
