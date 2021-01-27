#include <sys/types.h>
#include <unistd.h>
#include <QSettings>
#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>

#include "DataWorkerNetSender.h"
#include "NetworkWorkerTcp.h"
#include "NetworkWorkerUdp.h"

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
    _net = std::unique_ptr<INetworkWorker>(new NetworkWorkerUdp());
    _pkgProcessor = std::unique_ptr<ProtocolPkgProcessor>(new ProtocolPkgProcessor(VERSION::VER_1));

    _abort = false;

    QElapsedTimer timer, timerNetInfo;
    timer.start();
    timerNetInfo.start();

    uint64_t bytesSend = 0;
    uint64_t countError = 0;

    forever
    {
        if(_abort)
            break;

        QMutexLocker lock(&_mutex);
        if(processData() && _isSendData)
        {
            if(_net && (timer.elapsed() > _sendInterval))
            {
                QVector<QByteArray> dataFrame = _pkgProcessor->serialize(_demod->getListOfObjects());
                if(!dataFrame.isEmpty())
                {
                    int N = dataFrame.size();
                    for(int i = 0 ; i < N ; i++)
                    {
                        int64_t ret = _net->writeDatagramm(_ip,_port,dataFrame[i]);
                        if(ret > 0)
                            bytesSend += uint64_t(ret);
                        else
                            countError++;
                    }
                }
                timer.restart();
            }
        }

        if(timerNetInfo.elapsed() > 2000)
        {
            emit signalNetworkExchange(bytesSend,countError);
            timerNetInfo.restart();
        }
        QCoreApplication::instance()->processEvents();
    }

    qDebug()<<"terminate thread id" << QThread::currentThreadId();
    emit finished();
}

void DataWorkerNetSender::slotSetNetworkSettings(bool isSend, QString ip, quint16 port)
{
    _isSendData = isSend;
    _ip = ip;
    _port = port;
}
