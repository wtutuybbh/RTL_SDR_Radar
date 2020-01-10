#ifndef NULLCARRIER_H
#define NULLCARRIER_H

#include <QSharedPointer>

#include "interface/ICarrierClass.h"
#include "NullNavigationSystem.h"

/*!
 * \brief The NullCarrier class
 * Реализация паттерна null-object. Класс заглушка
 */

class NullCarrier final : public ICarrierClass
{
    QSharedPointer<INavigationSystem> _ptrNavigation;

public:
    NullCarrier();
    ~NullCarrier();

    /*!
     * \brief setGeoCoord установка геокординат
     * \param pst - текущие координаты
     */
     void setGeoCoord(const Position&){}
     /*!
      * \brief getGeoCoord - получение текущих координат
      * \return  значение широты / долготы Ростова-на-Дону
      */
     Position getGeoCoord() { return _ptrNavigation->getGeoCoord(); }

     /*!
      * \brief setCourse установка курса.
      * Если такие данные предоставляет система
      */
     void setCourse(double) {}
     /*!
      * \brief getCourse получение текущего курса
      * \return значение курса - 0
      */
     double getCourse() { return _ptrNavigation->getCourse(); }

     /*!
      * \brief setSpeed значение скорости.
      * Если такие данные предоставляет навигационная система
      * \param val
      */
     void setSpeed(double) {}
     /*!
      * \brief getSpeed текущая скорость
      * \return скорость 0
      */
     double getSpeed() { return _ptrNavigation->getSpeed(); }

};

#endif // NULLCORVETTE_H
