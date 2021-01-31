#ifndef NULLNAVIGATIONSYSYTEM_H
#define NULLNAVIGATIONSYSYTEM_H

#include "interface/INavigationSystem.h"
#include "osm/coordUtils/Position.h"
/*!
 * \brief The NullNavigationSystem class
 * Реализация паттерна null-object. Класс заглушка
 */

class NullNavigationSystem final : public INavigationSystem
{
    Position _position;
public:
    NullNavigationSystem();

    // курс
    void setCourse(double)  {}
    double getCourse()  { return 0.0; }

    //set/get координат
    void setGeoCoord(const Position & pos);
    Position getGeoCoord();

    //set/get скорости
    void setSpeed(double) {}
    double getSpeed(){ return 0.0; }
};

#endif // NULLNAVIGATIONSYSYTEM_H
