#ifndef INAVIGATIONSYSTEM
#define INAVIGATIONSYSTEM

#include "coord/Position.h"
/*!
 * \brief The INavigationSystem class
 * Интерфейс для реализации работы с навигационной системой.
 *
 * \author Данильченко Артём
 *
 */
class INavigationSystem
{
public:
    virtual ~INavigationSystem(){}
    /*!
     * \brief setCourse установка курса.
     * Если такие данные предоставляет система
     */
    virtual void setCourse(double) = 0;
    /*!
     * \brief getCourse получение текущего курса
     * \return значение курса
     */
    virtual double getCourse() = 0;

    /*!
     * \brief setGeoCoord установка геокординат
     * \param pst - текущие координаты
     */
    virtual void setGeoCoord(const Position &pst) = 0;
    /*!
     * \brief getGeoCoord - получение текущих координат
     * \return  значение широты / долготы
     */
    virtual Position getGeoCoord() = 0;

    /*!
     * \brief setSpeed значение скорости.
     * Если такие данные предоставляет навигационная система
     * \param val - ед.измерения определяются конкретной реализацией
     */
    virtual void setSpeed(double val) = 0;
    /*!
     * \brief getSpeed текущая скорость
     * \return ед.измерения определяются конкретной реализацией
     */
    virtual double getSpeed() = 0;
};
#endif // INAVIGATIONSYSTEM

