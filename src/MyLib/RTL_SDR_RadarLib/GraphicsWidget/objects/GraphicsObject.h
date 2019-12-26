#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QString>

class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    float _rtAngle = 0;

    //загрузка иконки объекта
    void loadPixmap();

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;
public:
    explicit GraphicsObject();
    virtual ~GraphicsObject() override;

    void setRotateAngle(float rta);

 protected:

    QPixmap _pixmapIcon;
    const QSize _sizeIcon = QSize(30,30);
    bool _drawId = false;
    bool _drawName = false;
    bool _isSelect = false;
    QPointF _screenPos = QPointF(-10,-10);

    //задаем область отрисовки
    QRectF boundingRect() const override;
    //метод рисования
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget = 0) override;

};

#endif // GRAPHICSOBJECT_H
