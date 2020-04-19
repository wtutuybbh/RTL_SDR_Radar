#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QMutexLocker>
#include <chrono>
#include <atomic>
#include <memory>

#include "interface/IWorker.h"
#include "interface/IDSP.h"
#include "sdr_dev/include/constant.h"
/*!
 * \brief The DataWorker class
 * Реализация интерфейса получения и обработки данных от приемника
 * \author Данильченко Артём
 */
class DataWorker : public IWorker
{
    Q_OBJECT

protected:

    std::atomic<bool> _abort;
    QMutex _mutex;

    std::chrono::steady_clock::time_point _firstTimeBreakpoint;
    std::chrono::steady_clock::time_point _secondTimeBreakpoint;

    QSharedPointer<IReciverDevice> _device;
    QSharedPointer<IDemodulator> _demod;
    QSharedPointer<IDSP> _dsp;

    ///< размер данных для съема
    size_t _dataVectorSize = MODES_DATA_LEN + MODES_FULL_LEN_OFFS;
    ///< вектор для хранения массива
    QVector<uint8_t> _dataVector;

    ILogger* _log = nullptr;
    /*!
     * \brief processData  обработка данных
     * Производится чтение блока данных из приемника
     * и запуск обработки данных в отдельных потоках.
     * Обработка - демодуляция и выполнение ЦОС
     * \return результат операции
     */
    bool processData();

public:

    DataWorker(QSharedPointer<IReciverDevice> dev,
               QSharedPointer<IDemodulator> dem,
               size_t dataSize = MODES_DATA_LEN + MODES_FULL_LEN_OFFS);

    ~DataWorker() override;
    /*!
     * \brief setLogger - внедрение зависимости модуля логгирования
     */
    void setLogger(ILogger* log) override { _log = log; }
    /*!
     * \brief setReciverDevice внедрение зависимости модуля работы с приемником
     */
    void setReciverDevice(QSharedPointer<IReciverDevice> dev) override { _device = dev;}
    /*!
     * \brief setDemodulator - внедрение зависимости модуля
     * демодуляции входного сигнала
     */
    void setDemodulator(QSharedPointer<IDemodulator> dem) override { _demod = dem; }
    /*!
     * \brief setDSP внедрение зависимости модуля
     * алгоритмов ЦОС
     */
    void setDSP(QSharedPointer<IDSP> dsp) override { _dsp = dsp; }
    /*!
     * \brief abortExec - прекращение выполения цикла обработки данных
     */
    void abortExec() override { _abort = true; }
    /*!
     * \brief setTimeout задание периода съема и обработки данных с приёмника
     * \param msleep - время в мс
     */
    void setTimeout(uint64_t) override {}

public slots:
    /*!
    * \brief exec запуск цикла получения и обработки данных
    */
    void exec() override;
};

#endif // DATAWORKER_H
