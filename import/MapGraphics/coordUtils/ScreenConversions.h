#ifndef SCREENCONVERSIONS_H
#define SCREENCONVERSIONS_H
#include <QPointF>
#include <math.h>

struct PolarCoord
{
    PolarCoord(double p, double r):phi(p),range(r){}
    double phi;      //угол
    double range;    //радиус вектор
};

class ScreenConversions
{
public:
    ScreenConversions() = delete;
    static QPointF screenToDecart(const QPointF &screenCenter, const QPointF &xy);
    static PolarCoord screenToPolar(const QPointF &screenCenter, const QPointF &xy);
    static QPointF decartToScreen(const QPointF &screenCenter, const QPointF &xy);
    static PolarCoord decartToPolar(const QPointF &p, double toNorth = 90.0);
    static QPointF polarToDecart(const PolarCoord &plr);
    static QPointF polarToScreen(const QPointF &screenCenter, const PolarCoord &plr);
    static QPointF polarToScreen(const QPointF &screenCenter, qreal angl, qreal r);
};

#endif // SCREENCONVERSIONS_H
