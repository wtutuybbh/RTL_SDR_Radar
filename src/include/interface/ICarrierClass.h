#ifndef ICARRIERCLASS_H
#define ICARRIERCLASS_H

#include "INavigationSystem.h"
class Position;

class ICarrierClass
{

public:
    virtual ~ICarrierClass(){}
    /*!
     * \brief setGeoCoord установка координат носителя
     * \param Position - геоокординаты носителя
     */
    virtual void setGeoCoord(const Position &) = 0;
    /*!
     * \brief getGeoCoord - получение координат носителя
     * \return значение геокоординат широта / долгота
     */
    virtual Position getGeoCoord() = 0;

    /*!
     * \brief setCourse установка курса.
     * Если носитель имеет простарнственную ориентацию
     */
    virtual void setCourse(double) = 0;
    /*!
     * \brief getCourse получить текущее значение курса
     * Если носитель имеет пространственную ориентацию
     * \return значение 0 - 360 градусов относительно севера
     */
    virtual double getCourse() = 0;

    /*!
     * \brief setSpeed - установка скорости движения носителя
     * Если носитель не стационарный объект
     */
    virtual void setSpeed(double) = 0;
    /*!
     * \brief getSpeed получение скорости движения носителя
     * \return ед.из определяются в наследниках
     */
    virtual double getSpeed() = 0;
};



#endif
