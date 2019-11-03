#include <sys/types.h>
#include <unistd.h>
#include <QSettings>

#include "DataWorkerNetSender.h"
#include "NetworkWorker.h"

using namespace std::chrono;

DataWorkerNetSender::DataWorkerNetSender(QSharedPointer<IReciverDevice> dev,
                                         QSharedPointer<IDemodulator> dem,
                                         const QString &ip,
                                         uint16_t port,
                                         size_t dataSize) :
    DataWorker (dev,dem,dataSize)
{
    if(!_ip.isEmpty())
    {
        _ip = ip;
        _port = port;
    }
    qDebug()<<"create DataWorkerNet";
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

    _firstTimeBreakpoint = steady_clock::now();

    forever
    {
        if(_abort)
            break;

        if(_net && !_net->isConnected())
            _net->connect(_ip,_port,CONNECT_TIMEOUT);

        QMutexLocker lock(&_mutex);
        if(processData())
        {
            _secondTimeBreakpoint = steady_clock::now();

            int64_t value = duration_cast<std::chrono::milliseconds>(_secondTimeBreakpoint - _firstTimeBreakpoint).count();
            if(value > _sendInterval)
            {
                if(_net && _net->isConnected())
                    _net->writeDatagramm(_demod->getRawDump());

                _firstTimeBreakpoint = steady_clock::now();
            }
        }
    }

    qDebug()<<"terminate thread id" << QThread::currentThreadId();
    emit finished();
}
