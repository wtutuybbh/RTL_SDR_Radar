#ifndef NAVIGATIONSYSTEM_H
#define NAVIGATIONSYSTEM_H

#include <QDateTime>

#include "interface/INavigationSystem.h"
#include "MapGraphics/coordUtils/Position.h"

/*!
 * \brief The NavigationSystem class
 * Реализация интерфейса навигационной системы
 *
 * \author Данильченко Артём
 *
 */
class NavigationSystem : public INavigationSystem
{
    //гео координаты
    Position _geoCoord;
    //курс
    double _course;
    //скорость
    double _speed;

public:
    explicit NavigationSystem();

    /*!
     * \brief setCourse установка курса.
     * Если такие данные предоставляет система
     */
    void setCourse(double course) { _course = course; }
    /*!
     * \brief getCourse получение текущего курса
     * \return значение курса
     */
    double getCourse() { return _course; }

    /*!
     * \brief setGeoCoord установка геокординат
     * \param pst - текущие координаты
     */
    void setGeoCoord(const Position & geo) { _geoCoord = geo; }
    /*!
     * \brief getGeoCoord - получение текущих координат
     * \return  значение широты / долготы
     */
    Position getGeoCoord() { return _geoCoord; }

    /*!
     * \brief setSpeed значение скорости.
     * Если такие данные предоставляет навигационная система
     * \param val - ед.измерения определяются конкретной реализацией
     */
    void setSpeed(double speed) { _speed = speed; }
    /*!
     * \brief getSpeed текущая скорость
     * \return ед.измерения определяются конкретной реализацией
     */
    double getSpeed() { return _speed; }
};

#endif // NAVIGATIONSYSTEM_H
