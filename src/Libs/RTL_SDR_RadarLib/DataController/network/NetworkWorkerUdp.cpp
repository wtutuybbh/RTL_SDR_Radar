#include "NetworkWorkerUdp.h"

NetworkWorkerUdp::NetworkWorkerUdp()
{

}

NetworkWorkerUdp::NetworkWorkerUdp(const QString &ip, uint16_t port)
{
    Q_UNUSED(ip)
    Q_UNUSED(port)
//    if(connect(ip,port))
//         qDebug()<<"connect to "<<ip<<port;
    qDebug() << "create NetworkWorker";
}

NetworkWorkerUdp::~NetworkWorkerUdp()
{
    disconnect();
    _socket.reset(nullptr);
    qDebug() << "delete NetworkWorker";
}

void NetworkWorkerUdp::addDebugMsg(const QString &str)
{
    if(_log)
        _log->push(str);

    qDebug()<<str;
}

bool NetworkWorkerUdp::connect(const QString &ip, uint16_t port, uint16_t timeout)
{
    Q_UNUSED(timeout)

    if(_socket == nullptr)
        _socket = std::unique_ptr<QUdpSocket>(new QUdpSocket());


    if(isConnected())
        disconnect();

    if(!_socket)
    {
        addDebugMsg(QString("Error create QTcpSocket"));
        return false;
    }

    if(ip.isEmpty())
    {
        addDebugMsg(QString("IP is empty"));
        return false;
    }

    if(_addLogInfo)
    {
        QString logString = QString("[UDP] try connect to: IP = %1, port = %2 ..../").arg(ip).arg(port);
        addDebugMsg(logString);
    }

    if( _socket->state() != QAbstractSocket::BoundState)
    {
        if(!_socket->bind(QHostAddress(ip), port))
        {
            if(_addLogInfo)
            {
                QString logString = QString( "[UDP] get error connection: %1 ")
                        .arg(_socket->errorString());

                addDebugMsg(logString);
            }

            return false;
        }
    }
    return  isConnected();
}

bool NetworkWorkerUdp::isConnected()
{
    return (_socket != nullptr) && (_socket->state() == QAbstractSocket::BoundState);
}

void NetworkWorkerUdp::disconnect()
{
    if(_socket)
    {
        _addLogInfo = false;
        _socket->disconnectFromHost();
        addDebugMsg("[UDP] disconnect");
    }
}

int64_t NetworkWorkerUdp::writeDatagramm(const QByteArray &byteArray)
{
    if(_socket == nullptr)
        return  -1;

    if(byteArray.isEmpty())
        return -1;

//    qDebug() << "[UDP] write data:" <<  "[" <<  byteArray.toHex() << "]";

    int64_t ret = _socket->write(byteArray);
    addDebugMsg(QString("[UDP] size write data : %1").arg(ret)) ;

    _socket->flush();
    return ret;
}

int64_t NetworkWorkerUdp::writeDatagramm(QString& ip,
                                      uint16_t port,
                                      const QByteArray &byteArray)
{

    if(byteArray.isEmpty())
        return  -1;

    if(_socket == nullptr)
        _socket = std::unique_ptr<QUdpSocket>(new QUdpSocket());


    int64_t len =  _socket->writeDatagram(byteArray, QHostAddress(ip), port);
    //    qDebug() << "[UDP] write data:" <<  "[" <<  byteArray.toHex() << "]";
    _socket->flush();
    return len;
}

