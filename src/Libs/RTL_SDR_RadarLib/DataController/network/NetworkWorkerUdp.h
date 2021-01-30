#ifndef NETWORKWORKERUDP_H
#define NETWORKWORKERUDP_H

#include <QUdpSocket>
#include <QDateTime>

#include <thread>
#include <QThread>

#include <interface/INetworkWorker.h>
#include "interface/ILogger.h"

class NetworkWorkerUdp : public INetworkWorker
{

    Q_OBJECT
    ///< модуль логгирования
    ILogger* _log = nullptr;
    ///< контроллер пакетов
    IPackageController* _pkgCtrl = nullptr;
    ///< сокет для подключения по сети
    std::unique_ptr<QUdpSocket> _socket = nullptr;
    ///< добавление сообщения в модуль логгирования
    void addDebugMsg(const QString& str);

    bool _addLogInfo = true;

public:

    NetworkWorkerUdp();
    NetworkWorkerUdp(const QString &ip, uint16_t port);
    ~NetworkWorkerUdp() override;
    /*!
     * \brief setLogger - внедрение зависимости модуля логгирования
     */
    void setLogger(ILogger* log) override { _log = log; }
    /*!
     * \brief setController - внедрение зависимости модуля
     * формирования сетевых пакетов
     */
    void setController(IPackageController* ctrl) override { _pkgCtrl = ctrl; }


    /*!
     *   \brief Подключение к физическому устройству.
     *   \return результат операции
     */
    bool connect(const QString& ip,
                 uint16_t port,
                 uint16_t timeout = 1000) override;

    /*!
     * \brief Проверка состояние подключения
     * \return есть подключение или нет
     */
    bool isConnected() override;

    /*!
     * \brief отключение
     */
    void disconnect() override;
    /*!
     *  \brief Запись массива данных,
     *  используя ранее установленное подключение
     *  \param [in] byteArray - массив данных
     *  \return количество отправленных байт, -1 в случае ошибки
    */
    int64_t writeDatagramm(const QByteArray &byteArray) override;
    /*!
     *  \brief Запись массива данных,
     *  без использования установленного ранее подключения.
     *  Либо использование udp рассылки
     *  \param [in] ip - IP адрес получателя
     *  \param [in] port - порт получателя
     *  \param [in] byteArray - массив данных
     *  \return количество отправленных байт, -1 в случае ошибки
    */
    int64_t writeDatagramm(QString &ip,
                           uint16_t port,
                           const QByteArray &byteArray) override;
    /*!
     *  \brief Чтение массива данных
     *  \return количество прочтённых байт, -1 в случае ошибки
    */
    int64_t readDatagramm() override { return 0; }

//    void setNetworkSettings(bool needSend, QString IP, uint16_t port) override;

};

#endif // NETWORKWORKERUDP_H
