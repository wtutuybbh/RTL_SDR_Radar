#ifndef CARRIER_H
#define CARRIER_H

#include "carrier_global.h"
#include "interface/ICarrierClass.h"

/*!
 * \brief The Carrier class
 * реализация класса носителя
 * \author Данильченко Артём
 */

class CARRIERSHARED_EXPORT Carrier: public ICarrierClass
{

    QSharedPointer<INavigationSystem> _ptrNavigation;

public:
     Carrier(bool isNavigation);
    ~Carrier() override;

     /*!
      * \brief setGeoCoord установка координат носителя
      * \param Position - геоокординаты носителя
      */
     void setGeoCoord(const Position &) override{}

     /*!
      * \brief getGeoCoord - получение координат носителя
      * \return значение геокоординат широта / долгота
      */
     Position getGeoCoord() override;

     /*!
      * \brief setCourse установка курса.
      * Если носитель имеет простарнственную ориентацию
      */
     void setCourse(double)override{}
     /*!
      * \brief getCourse получить текущее значение курса
      * Если носитель имеет пространственную ориентацию
      * \return значение 0 - 360 градусов относительно севера
      */
     double getCourse() override;

     /*!
      * \brief setSpeed - установка скорости движения носителя
      * Если носитель не стационарный объект
      */
     void setSpeed(double)override{}
     /*!
      * \brief getSpeed получение скорости движения носителя
      * \return ед.из определяются в наследниках
      */
     double getSpeed() override;

};

#endif
