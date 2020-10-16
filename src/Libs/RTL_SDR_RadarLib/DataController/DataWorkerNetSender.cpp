#include <sys/types.h>
#include <unistd.h>
#include <QSettings>
#include <QThread>
#include <QElapsedTimer>

#include "DataWorkerNetSender.h"
#include "NetworkWorker.h"

using namespace std::chrono;

DataWorkerNetSender::DataWorkerNetSender(QSharedPointer<IReciverDevice> dev,
                                         QSharedPointer<IDemodulator> dem,
                                         const QString &ip,
                                         uint16_t port,
                                         int64_t sendIntervalMs,
                                         size_t dataSize) :
    DataWorker (dev,dem,dataSize)
{
    if(!_ip.isEmpty())
    {
        _ip = ip;
        _port = port;
        _sendInterval = sendIntervalMs;
    }
    qDebug()<<"create DataWorkerNet"<< _ip<<_port<<_sendInterval;
}

DataWorkerNetSender::~DataWorkerNetSender()
{
    if(_net)
        _net->disconnect();
    _net.reset(nullptr);

    qDebug()<<"delete DataWorkerNet";
}

void DataWorkerNetSender::exec()
{
    _net = std::unique_ptr<INetworkWorker>(new NetworkWorker(_ip,_port));
    _abort = false;

    QElapsedTimer timer;
    timer.start();

    forever
    {
        if(_abort)
            break;

        QMutexLocker lock(&_mutex);
        if(processData())
        {
            if(_net && (timer.elapsed() > _sendInterval))
            {
                if(!_net->isConnected())
                    _net->connect(_ip,_port,CONNECT_TIMEOUT);

                if(_net->isConnected())
                    _net->writeDatagramm(_demod->getRawDumpOfObjectsInfo());

                timer.restart();
            }
        }
    }

    qDebug()<<"terminate thread id" << QThread::currentThreadId();
    emit finished();
}
