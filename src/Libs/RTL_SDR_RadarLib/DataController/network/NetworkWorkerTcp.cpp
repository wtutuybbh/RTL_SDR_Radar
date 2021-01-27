#include <QString>
#include <QHostAddress>
#include <QThread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "NetworkWorkerTcp.h"


NetworkWorkerTcp::NetworkWorkerTcp(const QString &ip, uint16_t port)
{
    Q_UNUSED(ip)
    Q_UNUSED(port)
//    if(connect(ip,port))
//         qDebug()<<"connect to "<<ip<<port;
    qDebug() << "create NetworkWorker";
}

NetworkWorkerTcp::~NetworkWorkerTcp()
{
    disconnect();
    _socket.reset(nullptr);
    qDebug() << "delete NetworkWorker";
}

void NetworkWorkerTcp::addDebugMsg(const QString &str)
{
    if(_log)
        _log->push(str);

    qDebug()<<str;
}

bool NetworkWorkerTcp::connect(const QString &ip, uint16_t port, uint16_t timeout)
{
    if(_socket == nullptr)
        _socket = std::unique_ptr<QTcpSocket>(new QTcpSocket());

    int fd = _socket->socketDescriptor();
    _socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    int maxIdle = 5; /* seconds */
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 3;  // send up to 3 keepalive packets out, then disconnect if no response
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 2;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

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
        QString logString = QString("[TCP] try connect to: IP = %1, port = %2 ..../").arg(ip).arg(port);
        addDebugMsg(logString);
    }

    if( _socket->state() != QAbstractSocket::ConnectedState &&
            _socket->state() != QAbstractSocket::ConnectingState)
    {
        _socket->connectToHost(ip, port);
        if(!_socket->waitForConnected(timeout))
        {
            if(_addLogInfo)
            {
                QString logString = QString( "[TCP] get error string from connection: %1 ")
                        .arg(_socket->errorString());

                addDebugMsg(logString);
            }

            return false;
        }
    }
    return  isConnected();
}

bool NetworkWorkerTcp::isConnected()
{
    return (_socket != nullptr) && (_socket->state() == QAbstractSocket::ConnectedState);;
}

void NetworkWorkerTcp::disconnect()
{
    if(_socket)
    {
        _addLogInfo = false;
        _socket->disconnectFromHost();
        addDebugMsg("[TCP] disconnect");
    }
}

int64_t NetworkWorkerTcp::writeDatagramm(const QByteArray &byteArray)
{
    if(_socket == nullptr)
        return  -1;

    if(byteArray.isEmpty())
        return -1;

    //qDebug() << "[TCP] write data:" <<  "[" <<  byteArray.toHex() << "]";

    int64_t ret = _socket->write(byteArray);
    addDebugMsg(QString("[TCP] size write data : %1").arg(ret)) ;

    _socket->flush();
    return ret;
}

int64_t NetworkWorkerTcp::writeDatagramm(QString& ip,
                                      uint16_t port,
                                      const QByteArray &byteArray)
{
    if(_socket == nullptr)
        return  -1;

    if( (_socket->peerAddress().toString() != ip) &&
            (_socket->peerPort() != port ))
    {
        disconnect();
        connect(ip,port);
    }

    if(!isConnected())
        return -1;

    return writeDatagramm(byteArray);
}

