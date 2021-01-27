#ifndef IWORKER_H
#define IWORKER_H

#include <QObject>
#include <QSharedPointer>

#include "ILogger.h"
#include "IReciverDevice.h"
#include "IDemodulator.h"
#include "IDSP.h"
#include "INetworkWorker.h"
/*!
 * \brief The IWorker class
 *  Интерфейс класса получения и обработки данных от приемника
 * \author Данильченко Артём
 */

class IWorker: public QObject
{
   Q_OBJECT

public:
    virtual ~IWorker(){}
    /*!
     * \brief setLogger - внедрение зависимости модуля логгирования
     */
    virtual void setLogger(ILogger*) = 0;
    /*!
     * \brief setLogger - внедрение зависимости модуля
     * взаимодействия с приемником сигнала
     */
    virtual void setReciverDevice(QSharedPointer<IReciverDevice>) = 0;
    /*!
     * \brief setLogger - внедрение зависимости модуля
     * демодуляции
     */
    virtual void setDemodulator(QSharedPointer<IDemodulator>) = 0;
    /*!
     * \brief setLogger - внедрение зависимости модуля
     * алгоритмов ЦОС
     */
    virtual void setDSP(QSharedPointer<IDSP>) = 0;
    /*!
     * \brief abortExec - прекращение выполения цикла обработки данных
     */
    virtual void abortExec() = 0;
    /*!
     * \brief setTimeout задание периода съема и обработки данных с приёмника
     * \param msleep - время в мс
     */
    virtual void setTimeout(uint64_t msleep) = 0;
public slots:
    /*!
    * \brief exec запуск цикла получения и обработки данных
    */
   virtual void exec() = 0;

   virtual void slotSetNetworkSettings(bool isSend,QString ip, quint16 port) = 0;
signals:
    /*!
     * \brief finished сигнал выхода из цикла обработки данных
     */
    void finished();
    /*!
     * \brief сигнал об установки подключения к серверу
     */
    void signalStateConnectToServer(bool state);

    void signalNetworkExchange(quint64 byteSend, quint64 errorCount);

};
#endif // IWORKER_H
