#ifndef NULLNAVIGATIONSYSYTEM_H
#define NULLNAVIGATIONSYSYTEM_H

#include "interface/INavigationSystem.h"
/*!
 * \brief The NullNavigationSystem class
 * Реализация паттерна null-object. Класс заглушка
 */
class Position;

class NullNavigationSystem final : public INavigationSystem
{
public:
    NullNavigationSystem();

    // курс
    void setCourse(double)  {}
    double getCourse()  { return 0.0; }

    //set/get координат
    void setGeoCoord(const Position &)  {}
    Position getGeoCoord();

    //set/get скорости
    void setSpeed(double) {}
    double getSpeed(){ return 0.0; }
};

#endif // NULLNAVIGATIONSYSYTEM_H
