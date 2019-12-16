#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QString>
#include <QFile>

#include "interface/IObject.h"

class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT

    QSharedPointer<IObject> _prtObject;
    double _rtAngle = 0;
    bool _isExistIcon = false;

    virtual void drawObjectIcon();

    //загрузка иконки объекта
    void loadPixmap();

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;
public:
    explicit GraphicsObject();
    virtual ~GraphicsObject() override;

    void setRotateAngle(float rta);
    //установить объект текущим или нет
    void setSelectObject(bool value);
    bool isSelectedObject() const;

 protected:

    QImage img;
    int _sizeIcon = 20;
    QColor _colorIcon =  QColor(0,250,0);
    bool _drawId = false;
    bool _drawName = false;
    bool _isSelect = false;
    QPointF _screenPos = QPointF(-10,-10);

    //задаем область отрисовки
    QRectF boundingRect() const override;
    //метод рисования
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget = 0) override;

    void resetIconState();
};

#endif // GRAPHICSOBJECT_H
