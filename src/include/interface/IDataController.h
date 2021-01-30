#ifndef IDATACONTROLLER_H
#define IDATACONTROLLER_H

#include <stdint.h>

#include <QSharedPointer>
#include <QObject>

#include "ILogger.h"
#include "IReciverDevice.h"
#include "IDemodulator.h"
#include "interface/IDSP.h"
#include "INetworkWorker.h"

/*!
 * \brief The IDataController class
 * Интерфейс класса контроллера обработки данных
 * \author Данильченко Артём
 */

enum class  TYPE_WORKER
{
    DATA_STREAM = 0,
    DATA_TO_NETWORK
};

class IDataController: public QObject
{
    Q_OBJECT
public:
    virtual ~IDataController(){}
    /*!
     * \brief setLogger - внедрение зависимости модуля логгирования
     */
    virtual void setLogger(ILogger*) = 0;
    /*!
     * \brief setReciverDevice внедрение зависимости модуля работы с приемником
     */
    virtual void setReciverDevice(QSharedPointer<IReciverDevice>) = 0;
    /*!
     * \brief setDemodulator - внедрение зависимости модуля
     * демодуляции входного сигнала
     */
    virtual void setDemodulator(QSharedPointer<IDemodulator>) = 0;
    /*!
     * \brief setDSP внедрение зависимости модуля
     * алгоритмов ЦОС
     */
    virtual void setDSP(QSharedPointer<IDSP>) = 0;
    /*!
     * \brief run запуск цикла приема и обработки данных
     */
    virtual void run() = 0;
    /*!
     * \brief stop остановка обработки данных
     */
    virtual void stop() = 0;
    /*!
     * \brief isRunning - проверка состояния обработки данных
     * \return
     */
    virtual bool isRunning() = 0;

signals:
    /*!
     * \brief сигнал об установки подключения к серверу
     */
    void signalStateConnectToServer(bool state);

    void signalNetworkExchange(quint64 byteSend, quint64 errorCount);

    void signalSetNetworkSettings(bool isSend,QString ip, quint16 port);
};
#endif // IDATACONTROLLER_H
