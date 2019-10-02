#ifndef DATAWORKERNET_H
#define DATAWORKERNET_H

#include "DataWorker.h"

class DataWorkerNetSender : public DataWorker
{
    std::unique_ptr<INetworkWorker> _net;
    const QString DEFAULT_IP = QString("127.0.0.1");
    const uint16_t DEFAULT_PORT = 62000;
    const int64_t SEND_INTERVAL = 500;
    const uint16_t CONNECT_TIMEOUT = 100;

    QString _ip = DEFAULT_IP;
    uint16_t _port = DEFAULT_PORT;
    int64_t _sendInterval = SEND_INTERVAL;

public:
    DataWorkerNetSender(QSharedPointer<IReciverDevice> dev,
                  QSharedPointer<IDemodulator> dem,
                  const QString& ip,
                  uint16_t port,
                  size_t dataSize = MODES_DATA_LEN + MODES_FULL_LEN_OFFS);
    ~DataWorkerNetSender() override;

public slots:
    void exec() override;
};

#endif // DATAWORKERNET_H
