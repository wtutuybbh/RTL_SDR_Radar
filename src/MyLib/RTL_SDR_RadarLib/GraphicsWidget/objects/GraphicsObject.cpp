#include "GraphicsObject.h"

GraphicsObject::GraphicsObject()
{
    //перемещаем в необходимую точку
    setPos(_screenPos);
    //загружаем иконку
    loadPixmap();
}

GraphicsObject::~GraphicsObject()
{

}

void GraphicsObject::setRotateAngle(float rta)
{
    _rtAngle = rta;
}


QRectF GraphicsObject::boundingRect() const
{
    return QRectF( - _sizeIcon.width()/2,
                   - _sizeIcon.width()/2,
                   _sizeIcon.width(),
                   _sizeIcon.width());
}

void GraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter->save();

    painter->rotate(_rtAngle + 90);

    painter->drawPixmap(boundingRect().x(),boundingRect().y(),_pixmapIcon);
    painter->restore();

    //    //выбран
    //    if(_isSelect)
    //    {
    //        painter->setPen(QPen(QBrush(QColor(250,0,0)),
    //                             2,
    //                             Qt::DashDotLine));

    //        painter->drawEllipse(boundingRect().center(),
    //                             boundingRect().left()-1,
    //                             boundingRect().top()-1);
    //    }
}



void GraphicsObject::loadPixmap()
{
    _pixmapIcon = QPixmap(_sizeIcon);
    _pixmapIcon.fill(Qt::transparent);
    _pixmapIcon.load(QString(":/icon/target/air.png"));
    _pixmapIcon = _pixmapIcon.scaled(_sizeIcon);
}


QVariant GraphicsObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged && scene())
    {
        _isSelect = value.toBool();
        if(_isSelect)
        {
            setZValue(1);
            setScale(1.2);
        }
        else
        {
            setZValue(0);
            setScale(1);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
