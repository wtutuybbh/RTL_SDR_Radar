#ifndef DATAWORKERNET_H
#define DATAWORKERNET_H

#include "DataWorker.h"
#include "protocol/ProtocolPkgProcessor.h"
#include <memory>

/*!
 * \brief The DataWorkerNetSender class
 * Реализация интерфейса получения и обработки данных от приемника
 * с возможностью отправки данных на сервер сбора информации
 * \author Данильченко Артём
 */

class DataWorkerNetSender : public DataWorker
{
    ///< указатель на модуль сетевого взаимодействия
    std::unique_ptr<INetworkWorker> _net;
    ///< указатель на носитель приемника
    std::unique_ptr<ProtocolPkgProcessor> _pkgProcessor;
    ///< ip для подключения по умолчанию
    const QString DEFAULT_IP = QString("127.0.0.1");
    ///< порт для подключения по умолчания
    const uint16_t DEFAULT_PORT = 62000;
    ///< интервал отправки сообщений
    const int64_t SEND_INTERVAL = 500;
    ///< таймат ожидания подключения
    const uint16_t CONNECT_TIMEOUT = 100;

    QString _ip = DEFAULT_IP;
    uint16_t _port = DEFAULT_PORT;
    int64_t _sendInterval = SEND_INTERVAL;
    bool _isSendData = true;

public:
    /*!
     * \brief DataWorkerNetSender конструктор
     * \param dev - модуль работы с приемником
     * \param dem - модуль демодуляции
     * \param ip - ip сервера
     * \param port  порт сервера
     * \param sendIntervalMs - период отправки данных
     * \param dataSize размер данных для чтения с приемника
     */
    DataWorkerNetSender(QSharedPointer<IReciverDevice> dev,
                  QSharedPointer<IDemodulator> dem,
                  const QString& ip,
                  uint16_t port,
                  int64_t sendIntervalMs,
                  size_t dataSize = MODES_DATA_LEN + MODES_FULL_LEN_OFFS);
    ///< деструктор
    ~DataWorkerNetSender() override;

public slots:
    /*!
    * \brief exec запуск цикла получения и обработки данных
    */
    void exec() override;

    void slotSetNetworkSettings(bool isSend,QString ip, quint16 port) override;


};

#endif // DATAWORKERNET_H
