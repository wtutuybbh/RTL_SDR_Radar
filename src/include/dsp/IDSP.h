#ifndef IDSP_H
#define IDSP_H

#include "SrcDataAdc.h"
/*!
 * \brief The IDSP class
 * Интерфейс для реализации цифровой обработки информации
 * \author Данильченко Артём
 */

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
};
#endif // IDSP_H
