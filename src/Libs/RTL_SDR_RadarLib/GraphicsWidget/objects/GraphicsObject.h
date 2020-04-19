#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QString>

#include "interface/IObject.h"
#include "../shared_classes/coord/Position.h"

class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    float _rtAngle = 0;
    bool _isImit = false;
    Position _geoPos;
    bool needDelete = false;
    double _azim;
    QString _text;
    //загрузка иконки объекта
    void loadPixmap(OBJECT_TYPE type, bool isImit);

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;
    void drawObjectIcon(bool imit);

public:
    explicit GraphicsObject(OBJECT_TYPE type = OBJECT_TYPE::base, bool imit = false);
    virtual ~GraphicsObject() override;

    void setRotateAngle(float rta);
    void setGeoPosition(const Position& pos) { _geoPos = pos; }
    Position getGeoPosition() { return _geoPos; }

    bool getNeedDelete() const;
    void setNeedDelete(bool value);

    float getAzimuth() const;
    void setAzimuth(double azim);

    void setText(const QString &text);

protected:

    QPixmap _pixmapIcon;
    QSize _sizeIcon = QSize(25,25);
    bool _drawId = false;
    bool _drawName = false;
    bool _isSelect = false;
    QPointF _screenPos = QPointF(-10,-10);
    QColor _colorIcon =  QColor(0,250,0);

    //задаем область отрисовки
    QRectF boundingRect() const override;
    //метод рисования
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget = 0) override;

};

#endif // GRAPHICSOBJECT_H
