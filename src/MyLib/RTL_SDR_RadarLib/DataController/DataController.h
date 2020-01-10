#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QThread>
#include <memory>

#include "datacontroller_global.h"
#include "interface/IDataController.h"
#include "interface/IPoolObject.h"
#include "interface/IWorker.h"

/*!
 * \brief The DataController class
 * реализация интерфейса контроллера обработки данных
 * \author Данильченко Артём
 */

class DATACONTROLLERSHARED_EXPORT DataController : public IDataController
{
    std::unique_ptr<IWorker> _worker = nullptr;
    QThread* _dataThread = nullptr;

public:
    /*!
     * \brief DataController конструктор с внедрением зависимостей
     * Создается экземпляр без функций сетевого взаимодействия
     * \param dev - модуль работы с утройством
     * \param dem - модуль демодуляции
     */
    explicit DataController(QSharedPointer<IReciverDevice> dev,
                            QSharedPointer<IDemodulator> dem);
    /*!
     * \brief DataController конструктор для экземпляра класса
     * с функций подключения к серверу сбора данных
     * \param dev - модуль работы с устройством
     * \param dem - модуль демодуляции
     * \param ip - адрес сервера
     * \param port - порт сервера
     */
    explicit DataController(QSharedPointer<IReciverDevice> dev,
                            QSharedPointer<IDemodulator> dem,
                            const QString & ip,
                            uint16_t port);

    ~DataController() override;

    /*!
     * \brief run запуск цикла приема и обработки данных
     */
    void run() override;
    /*!
     * \brief stop остановка обработки данных
     */
    void stop() override;
    /*!
     * \brief isRunning - проверка состояния обработки данных
     * \return
     */
    bool isRunning() override;
    /*!
     * \brief setLogger - внедрение зависимости модуля логгирования
     */
    void setLogger(ILogger* log) override;
    /*!
     * \brief setReciverDevice внедрение зависимости модуля работы с приемником
     */
    void setReciverDevice(QSharedPointer<IReciverDevice> dev) override;
    /*!
     * \brief setDemodulator - внедрение зависимости модуля
     * демодуляции входного сигнала
     */
    void setDemodulator(QSharedPointer<IDemodulator> dem) override;
    /*!
     * \brief setDSP внедрение зависимости модуля
     * алгоритмов ЦОС
     */
    void setDSP(QSharedPointer<IDSP> dsp) override;
};

#endif // DATACONTROLLER_H
