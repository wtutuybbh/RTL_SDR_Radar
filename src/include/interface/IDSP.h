#ifndef IDSP_H
#define IDSP_H

#include <stdint.h>
#include <QVector>
#include <complex.h>

/*!
 * \brief The IDSP class
 * Интерфейс для реализации цифровой обработки информации
 * \author Данильченко Артём
 */

struct SrcDataAdc
{
    QVector<std::complex<int8_t>> rawDataVector;
    QVector<double> magnitudeVector;
    QVector<double> fftVector;
};

class IDSP
{

public:
    virtual ~IDSP(){}
    /*!
     * \brief makeFFT Рассчет БПФ
     * \param vector отсчёты 8 - битного АЦП rtl-sdr
     * \return блок данных после обработк
     */
    virtual SrcDataAdc makeFFT(const QVector<uint8_t>& vector) = 0;
    /*!
     * \brief makeMagnitude - вычисление огибающей импульсного сигнала
     * \param vector - отсчёты 8-битного АЦП rtl-sdr
     * \return блок данных после обработки
     */
    virtual SrcDataAdc makeMagnitude(const QVector<uint8_t>& vector) = 0;
    /*!
     * \brief makeAll выполнение всех доступных операция ЦОС
     * \param vector - отсчёты 8-битного АЦП rtl-sdr
     * \return блок данных после обработки
     */
    virtual SrcDataAdc makeAll(const QVector<uint8_t>& vector) = 0;

    virtual SrcDataAdc takeDataBlock() = 0;
    virtual void lockDataBuffer() = 0;
    virtual bool tryLockDataBuffer() = 0;
    virtual void unlockDataBuffer() = 0;
    virtual bool isEmptyDataBuffer() = 0;

};
#endif // IDSP_H
