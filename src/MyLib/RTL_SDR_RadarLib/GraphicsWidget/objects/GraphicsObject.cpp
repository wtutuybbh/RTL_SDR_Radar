#include "GraphicsObject.h"

GraphicsObject::GraphicsObject(OBJECT_TYPE type, bool imit) :
    _isImit(imit)
{
    //перемещаем в необходимую точку
    setPos(_screenPos);

    //загружаем иконку
    loadPixmap(type,imit);
}

GraphicsObject::~GraphicsObject()
{
    qDebug()<<"GraphicsObject delete";
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

    QFont font = painter->font();
    int captionWidth = QFontMetrics(font.family()).width(_text);
    int captionHeight = QFontMetrics(font.family()).height() + 2;


    QRectF rect(boundingRect().width()/2 - captionWidth/2,
                boundingRect().top() - QFontMetrics(font.family()).height(),
                captionWidth,
                captionHeight);

    painter->drawText(rect, Qt::AlignCenter, _text);

    if(!_isImit)
        painter->rotate(_rtAngle - 90);

    painter->drawPixmap(boundingRect().x(),boundingRect().y(),_pixmapIcon);


    painter->restore();
}



bool GraphicsObject::getNeedDelete() const
{
    return needDelete;
}

void GraphicsObject::setNeedDelete(bool value)
{
    needDelete = value;
}

float GraphicsObject::getAzimuth() const
{
    return _azim;
}

void GraphicsObject::setAzimuth(double azim)
{
    _azim = azim;
}

void GraphicsObject::setText(const QString &text)
{
    _text = text;
}

void GraphicsObject::loadPixmap(OBJECT_TYPE type, bool isImit)
{
    _pixmapIcon = QPixmap(_sizeIcon);
    _pixmapIcon.fill(Qt::transparent);

    if(type == OBJECT_TYPE::air && !isImit )
    {
        _pixmapIcon.load(QString(":/icon/target/air.png"));
    }
    if(type == OBJECT_TYPE::base || isImit)
    {
        drawObjectIcon(isImit);
    }
    _sizeIcon = _pixmapIcon.size();
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


void GraphicsObject::drawObjectIcon(bool imit)
{
    //qDebug()<<"GraphicsObject::drawObjectIcon()";

    QPainter p(&_pixmapIcon);
    p.setRenderHint(QPainter::Antialiasing);

    qreal cX = _pixmapIcon.width() / 2;
    qreal cY = _pixmapIcon.width() / 2;
    qreal rad = _pixmapIcon.width() / 2;

    p.setPen(QPen(QBrush(_colorIcon), 2));

    QVector<QPointF> ln;

    ln.append(QPointF(cX, cY - rad));
    ln.append(QPointF(cX - rad, cY));

    ln.append(QPointF(cX - rad, cY));
    ln.append(QPointF(cX , cY + rad));

    ln.append(QPointF(cX ,cY + rad));
    ln.append(QPointF(cX + rad, cY));

    ln.append(QPointF(cX + rad ,cY));
    ln.append(QPointF(cX, cY - rad));

    p.drawLines(ln);

    p.drawPoint(QPointF(cX, cY));

}
