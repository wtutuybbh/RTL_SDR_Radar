#include <QGraphicsItem>
#include <QDebug>
#include <QThread>
#include <QGLWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <math.h>

#include "GraphicsWidget/GraphicsWidget.h"
#include "interface/IPoolObject.h"
#include "interface/ISubject.h"
#include "implements/MapController.h"

#include "osm/coordUtils/ScreenConversions.h"
#include "Carrier/ServiceLocator.h"
#include "objects/GraphicsObject.h"

GraphicsWidget::GraphicsWidget(uint32_t widthRect,
                               QWidget *parent):
    QGraphicsView(parent)
{

    initWidget(QRect(0,0,widthRect,widthRect),true);
    initCursors();

    _ptrMapController = QSharedPointer<IMapController>(new MapController());

    connect(_ptrMapController.data(), SIGNAL(updateTileGride()),
            this, SLOT(updateScene()));

    _ptrCarrier = ServiceLocator::getCarrier();
    _centerCoord = ServiceLocator::getCarrier()->getGeoCoord();
    //обновление сектора
    connect(&_timer, &QTimer::timeout, this, &GraphicsWidget::timeout);

    if(_updateInSector)
        _timer.start(TIMEOUT);

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

GraphicsWidget::~GraphicsWidget()
{
    qDebug()<<" ~GraphicsWidget() -> start delete scene";
    if(_scene)
        for (auto &iter:_scene->items())
        {
            if(iter)
                _scene->removeItem(iter);
        }
    delete _scene;
    _scene = nullptr;
    qDebug()<<"~GraphicsWidget() -> clear and delete scene";
}

void GraphicsWidget::subscribe(QSharedPointer<ISubject> subject)
{
    if(subject.isNull())
    {
        qDebug()<<"[GraphicsWidget::subscribe] : try subscribe nullptr";
        return;
    }

    subject->Attach(this);
}



void GraphicsWidget::unsubscribe(QSharedPointer<ISubject> subject)
{
    qDebug()<<"unsubscribe GraphicsWidget";

    if(!subject.isNull())
        subject->Deatach(this);

    subject.clear();
}


void GraphicsWidget::initWidget(const QRect &rect, bool enableOpenGL)
{
    this->setOptimizationFlags( QGraphicsView::DontClipPainter  |
                                QGraphicsView::DontSavePainterState |
                                QGraphicsView::DontAdjustForAntialiasing );

    this->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );

    this->setGeometry(rect);

    if(enableOpenGL)
        setViewport(new QGLWidget);

    setAlignment(Qt::AlignCenter);
    setCacheMode(CacheBackground);

    //BoundingRectViewportUpdate - обновлять в области указаной пользователем
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

    //как позиционировать сцену во время трансформации
    setTransformationAnchor(AnchorViewCenter);

    _scene = new QGraphicsScene(this);

    //отслеживания события движения мыши-с кликом или без
    setMouseTracking (true);

    //не индексируем itemы, так будет быстрей работать
    _scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    _scene->setSceneRect(0,0,this->width(),this->width());

    //установка сцены
    this->setScene(_scene);

    //убрать скролл
    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    setDisplayMode(DisplayMode::RADAR);

}

void GraphicsWidget::initCursors()
{
    //курсор для наведения
    _pxmCursor = QPixmap(_cursorSize);
    _pxmCursor.fill(Qt::transparent);
    _pxmCursor.load(QString(":/icon/app/pxmCursor.png"));

    //курсо для выделения
    _pxmSelect = QPixmap(_cursorSize);
    _pxmSelect.fill(Qt::transparent);
    _pxmSelect.load(QString(":/icon/app/pxmSelect.png"));

    changeCursorType(false);
    _fixCursor = false;
}

void GraphicsWidget::changeCursorType(bool enableSystem)
{
    if(enableSystem)
        this->setCursor(Qt::ArrowCursor);
    else
        this->setCursor(QCursor(_pxmCursor));
}


void GraphicsWidget::update(QSharedPointer<IPoolObject> pool)
{
    if( pool.isNull() )
    {
        qDebug()<<"GraphicsWidget::update() :: nullptr";
        return;
    }

    slotUpdateData(pool);
}

void GraphicsWidget::setDisplayMode(DisplayMode mode)
{
    _displayMode = mode;

    if(mode == DisplayMode::RADAR)
    {
        _radRadar = _scene->sceneRect().width()/2.0 - _textBorder;
        setDragMode(QGraphicsView::NoDrag);
    }
    else
        setDragMode(QGraphicsView::ScrollHandDrag);


    _distToBorderMap = (mode == DisplayMode::RADAR) ?
                (_scene->sceneRect().width()/2.0 - _textBorder)  :
                0;

    _centerCoord = ServiceLocator::getCarrier()->getGeoCoord();

    recalculateCoordObjects();
    //обновление кешированных данных
    QGraphicsView::resetCachedContent ();
}

GraphicsObject* GraphicsWidget::getGraphicsItem(QSharedPointer<IObject> &object)
{
    GraphicsObject* graphItem = nullptr;
    if(!_hashTable.contains(object->getUuid()) && object->getInUse())
    {
        graphItem = new GraphicsObject(object->getTypeObject(),
                                       object->isImitated());

        graphItem->setFlag(QGraphicsItem::ItemIsSelectable);

        _hashTable.insert(object->getUuid(),graphItem);
    }
    else
    {
        graphItem = _hashTable.value(object->getUuid());
        if(graphItem == nullptr)
            _hashTable.remove(object->getUuid());
    }

    return graphItem;
}

bool GraphicsWidget::needUpdateGraphicsObject(QSharedPointer<IObject> &object,
                                              GraphicsObject* graphItem)
{
    if(object.isNull() || graphItem == nullptr)
        return false;

    if( object->getObjectState() == OBJECT_STATE::DELETE_OBJECT ||
            (!object->getInUse()))
    {
        if(scene()->items().contains(graphItem))
            graphItem->setNeedDelete(true);
        return false;
    }

    if(!object->isValidGeoCoord() && scene()->items().contains(graphItem))
    {
        graphItem->setNeedDelete(true);
        return false;
    }

    return true;
}

void GraphicsWidget::updatePositionOnScene(GraphicsObject *graphItem)
{

    QPointF dot = { -10.0, -10.0 };

    double dist = _ptrMapController->getDistanceRadarScale_KM(_scene->sceneRect().size(),
                                                              _ptrCarrier->getGeoCoord(),
                                                              QPointF(_scene->width()/2 + _distToBorderMap,
                                                                      _scene->width()/2 ));

    double objectDist = _ptrMapController->getDistanceObject_KM(_ptrCarrier->getGeoCoord(),
                                                                graphItem->getGeoPosition());

    if(Conversions::isValidGeoCoord(graphItem->getGeoPosition()) && !scene()->items().contains(graphItem))
        _scene->addItem(graphItem);


    if( objectDist < dist / 2 )
    {
        dot = _ptrMapController->geoToScreenCoordinates( scene()->sceneRect().size(),
                                                         _ptrCarrier->getGeoCoord(),
                                                         graphItem->getGeoPosition());

        graphItem->setPos(dot);
        graphItem->setOpacity(1);

        if(!graphItem->isVisible())
            graphItem->show();
    }
    else
    {
        _vHiddenObject.append(graphItem->getAzimuth());
        graphItem->hide();
    }

    //если графический объект выбран текущим
    if(graphItem->isSelected() && _fixCursor)
        _fixCursorCoord = dot;
}

void GraphicsWidget::updateObjectOnScene(QSharedPointer<IObject> &object)
{
    if(_ptrMapController.isNull() || object.isNull())
        return;

    GraphicsObject* graphItem = getGraphicsItem(object);

    if(graphItem == nullptr)
        return;

    if(!needUpdateGraphicsObject(object,graphItem))
        return;

    graphItem->setRotateAngle(object->getCourse());
    graphItem->setGeoPosition(object->getGeoCoord());
    graphItem->setAzimuth(object->getAzimuth());
    graphItem->setText(QString::number(object->getId(),16) + "/" + object->getObjectName());
}

void GraphicsWidget::recalculateCoordObjects()
{
    _vHiddenObject.clear();

    for(auto &iter : _hashTable.values())
        updatePositionOnScene(iter);

    updateScene();
}

QStringList GraphicsWidget::getDataForTable(IObject* object)
{
    QStringList list;
    if(!object)
        return list;

    list << QString::number(object->getId())
         << object->getDateTimeStart().toString("dd:MM:yy hh:mm:ss")
         << object->getDateTimeStop().toString("dd:MM:yy hh:mm:ss")
         << QString::number(object->getAzimuth())
         << QString::number(object->getElevation())
         << object->getObjectName()
         << QString::number(object->getDistance_KM())
         << QString::number(object->getGeoCoord().latitude())
         << QString::number(object->getGeoCoord().longitude());

    return list;
}

void GraphicsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons()==Qt::LeftButton &&
            getDisplayMode() == DisplayMode::CARTESIAN)
    {

        moveMap(_oldPos.x()-event->pos().x(), _oldPos.y()-event->pos().y());
        _oldPos=event->pos();

    }

    _cursorCoord = mapToScene(event->pos());
    if(_fixCursor)
        return;

    QGraphicsItem * litem = nullptr;
    QTransform trans;
    //получаем значение элемента над которым находится перекрестие курсора
    litem = _scene->itemAt(mapToScene(event->pos()),trans);

    if(litem)
    {
        scene()->clearSelection();
        litem->setSelected(true);
    }

}

void GraphicsWidget::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) &&
            (getDisplayMode() == DisplayMode::CARTESIAN))
    {
        _oldPos = event->pos();
        return;
    }

    if(event->button() != Qt::LeftButton)
        return;

    _fixCursor = !_fixCursor;

    if(_fixCursor)
    {
        QGraphicsItem * litem = nullptr;
        QTransform trans;
        //получаем значение элемента над которым находится перекрестие курсора
        litem = _scene->itemAt(mapToScene(event->pos().x(),
                                          event->pos().y()),
                               trans);
        if(litem)
        {
            litem->setSelected(true);
            _fixCursorCoord = litem->pos();
        }
        else
            _fixCursorCoord = mapToScene(event->pos());
    }
    changeCursorType(_fixCursor);
    _scene->update();
}

void GraphicsWidget::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
}

void GraphicsWidget::wheelEvent(QWheelEvent * event)
{
    if (event->delta() < 0)
        RadarScalePlus();
    if (event->delta() > 0)
        RadarScaleMinus();
}


QPointF GraphicsWidget::getSceneCenterPont()
{
    QPointF dot = _ptrMapController->geoToScreenCoordinates(_scene->sceneRect().size(),
                                                            _centerCoord,
                                                            _ptrCarrier->getGeoCoord());
    return dot;
}

void GraphicsWidget::drawMap(QPainter* painter,bool isDraw)
{
    if(_ptrMapController.isNull() || (!isDraw) || (painter == nullptr))
        return;

    FILTER_TYPE type = (getDisplayMode() == DisplayMode::RADAR) ?
                FILTER_TYPE::NIGHT_AND_CIRCLE :
                FILTER_TYPE::NIGHT;

    QImage img = _ptrMapController->getImageMap(_scene->sceneRect().size(),
                                                _centerCoord,
                                                _distToBorderMap,
                                                type);

    painter->drawImage(0, 0, img);
}

void GraphicsWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->setRenderHint(QPainter::Antialiasing);

    drawMap(painter);

    painter->setPen(QPen(QBrush(_clrTron),1));

    painter->drawRect(_scene->sceneRect().toRect());

    //внешние точки, большие через 5 градусов
    double rad = _scene->sceneRect().width()/2 - _textBorder + 5;

    drawDotCicleWithLabel(painter, rad);
    initDrawText(painter);
    printMapScale(painter);
}

void GraphicsWidget::drawRadarSector(QPainter *painter)
{
    QPointF dot = getSceneCenterPont();

    QRectF drawingRect(dot.x() - _radRadar,
                       dot.y() - _radRadar,
                       _radRadar * 2,
                       _radRadar * 2);


    gradient.setCenter(drawingRect.center());
    gradient.setAngle(-_angleGradientSector);
    gradient.setColorAt(0, _clrTronAlpha);
    gradient.setColorAt(0.2, QColor(0,0,0,0));

    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(Qt::black));
    painter->setBrush(QBrush(gradient));

    painter->drawEllipse(drawingRect);
    painter->setPen(_clrTron);

    painter->drawLine(drawingRect.center(),
                      QPointF(ScreenConversions::polarToScreen(getSceneCenterPont(),
                                                               _angleGradientSector + _sectorSize * 2,
                                                               _radRadar)));

    for(auto &iter : _hashTable.values())
    {
        if(iter == nullptr)
            continue;

        if(iter->getAzimuth() > ((_angleGradientSector + _sectorSize) % 360) &&
                iter->getAzimuth() < ((_angleGradientSector + _sectorSize * 2) % 360))
        {
            if(iter->getNeedDelete())
            {
                _scene->removeItem(iter);
                auto key = _hashTable.key(iter);
                delete  iter;

                _hashTable.remove(key);
            }
            else
                updatePositionOnScene(iter);
        }
    }
}

void GraphicsWidget::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    painter->setRenderHint(QPainter::Antialiasing);

    //зафиксированный на карте курсо необходимо отрисовывать отдельно
    if(_fixCursor)
    {
        painter->drawPixmap(static_cast<int32_t>(_fixCursorCoord.x()-_pxmSelect.width()/2),
                            static_cast<int32_t>(_fixCursorCoord.y()-_pxmSelect.height()/2),
                            _pxmSelect.width(),
                            _pxmSelect.height(),
                            _pxmSelect);
    }



    initDrawText(painter);
    printScreenCoord(painter);
    printGeoCoord(painter);
    printCountObject(painter);

    drawHiddenObject(painter);

    drawCarrier(painter);
    if(!_updateInSector)
        return;

    drawRadarSector(painter);
}

void GraphicsWidget::resizeEvent(QResizeEvent *event)
{
    int h = event->size().height();
    int w = event->size().width();


    if( w < 600 || h < 600 )
    {

        QWidget::resizeEvent(event);
        return;
    }

    if(w > h)
        w = h;
    else
        h = w;

    const uint64_t dimension = static_cast<uint64_t>(sqrt(w * h));
    _scene->setSceneRect(0, 0, dimension, dimension);

    _radRadar = _scene->sceneRect().width()/2.0 - _textBorder;
    _distToBorderMap = (getDisplayMode() == DisplayMode::RADAR) ? _radRadar  : 0;

    recalculateCoordObjects();

    QWidget::resizeEvent(event);
}


void GraphicsWidget::drawCarrier(QPainter *p)
{
    QPointF dot = getSceneCenterPont();
    p->save();
    p->setPen(QPen(QBrush(QColor(0,250,0)),5));
    p->drawPoint(dot);
    p->restore();
}


void GraphicsWidget::drawHiddenObject(QPainter *p)
{
    //кто-то за масштабом
    static const QPointF points[3] =
    {
        QPointF(6, 0.0),
        QPointF(0.0, 12.0),
        QPointF(12.0, 12.0)
    };

    for(auto & iter: _vHiddenObject)
    {
        PolarCoord crd(iter,_distToBorderMap);

        QPointF dot = ScreenConversions::polarToScreen(
                    getSceneCenterPont(),
                    iter - 1,
                    _distToBorderMap);

        QPen pen(p->pen());
        p->setPen(QPen(QBrush(_clrTron),2));
        p->save();
        p->translate(dot);

        p->rotate(crd.phi);

        p->setBrush(QColor(34,139,34));
        p->setPen(QColor(0,255,127));

        p->drawPolygon(points, 3);
        p->restore();
        p->setPen(pen);
    }
}

void GraphicsWidget::drawText(QPainter *p,
                              int X,
                              int Y,
                              const QString &str)
{
    QFont font = p->font();
    int captionWidth = QFontMetrics(font.family()).width(str);
    int captionHeight = QFontMetrics(font.family()).height() + 2;

    if(X > _scene->width()/2)
        X -= captionWidth;

    if(Y > _scene->width()/2)
        Y -= captionHeight;

    QRectF rect(X,
                Y + QFontMetrics(font.family()).height() + 5.0,
                captionWidth,
                captionHeight);

    p->drawText(rect,Qt::AlignCenter,str);
}

void GraphicsWidget::drawText(QPainter *p,
                              int X,
                              int Y,
                              const QStringList &strList)
{
    QFont font = p->font();
    int captionWidth = 90;
    int captionHeight = QFontMetrics(font.family()).height();

    int tW = 0;
    for(auto & iter: strList)
    {
        tW = std::max(captionWidth,
                      QFontMetrics(font.family()).width(iter));
        captionWidth = (captionWidth < tW) ? tW :captionWidth;
    }
    p->setBrush(_clrTronAlpha);
    int YY = Y;
    int XX = X;
    for(int i = strList.size(); i > 0 ; --i)
    {
        XX = (X > _scene->width() / 2) ?
                    (X - captionWidth  - _textBorder / 5) :
                    (X + _textBorder / 5);

        YY = (Y > _scene->width() / 2) ?
                    (Y - i * captionHeight -_textBorder/5) :
                    (Y + i * captionHeight - captionHeight / 2);

        QRectF rect(XX,
                    YY,
                    captionWidth,
                    captionHeight);

        p->drawText(rect,Qt::AlignCenter,strList.at(strList.size() - i));
        p->drawRect(rect);
    }
}

void GraphicsWidget::initDrawText(QPainter *p)
{
    QFont font = p->font();
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setStyleHint(QFont::System,QFont::PreferAntialias);
    font.setBold(true);
    p->setFont(font);

    QPen pen(p->pen());
    pen.setCosmetic(true);
    p->setPen(QPen(QBrush(_clrWhite),2));
}

void GraphicsWidget::printScreenCoord(QPainter *p)
{
    if(p == nullptr )
        return;

    int X = _scene->sceneRect().toRect().bottomLeft().x();
    int Y = _scene->sceneRect().toRect().bottomLeft().y();

    QStringList list;
    list.append(QObject::tr("ЭК"));
    list.append(QString("X = %1").arg(_cursorCoord.x()));
    list.append(QString("Y = %1").arg(_cursorCoord.y()));

    drawText(p, X, Y,list);
}

void GraphicsWidget::drawCursorText(QPainter *p)
{
    if(p == nullptr )
        return;

    QPointF dot = (!_fixCursor) ? _cursorCoord : _fixCursorCoord;

    //перевод в полярную систему для рассчета пеленга и дальности
    PolarCoord plr = ScreenConversions::screenToPolar(getSceneCenterPont(),
                                                      dot);

    if(plr.range < _distToBorderMap)
    {
        Position sg = _ptrMapController->screenToGeoCoordinates(scene()->sceneRect().size(),
                                                                _ptrCarrier->getGeoCoord(),
                                                                dot);

        plr.range = _ptrMapController->getDistanceObject_KM(_ptrCarrier->getGeoCoord(),
                                                            sg);

        p->drawText(qRound(dot.x() - _cursorSize.width() / 2),
                    qRound(dot.y() - _cursorSize.height() / 2 - 2),
                    QString("П=%1").arg(plr.phi, 0,'f',1));

        p->drawText(qRound(dot.x() - _cursorSize.width() / 2),
                    qRound(dot.y() + _cursorSize.height() / 2 + 12),
                    QString("Д=%1").arg(plr.range,0,'f',2));
    }
}

void GraphicsWidget::printGeoCoord(QPainter *p)
{
    if(p == nullptr )
        return;

    Position ps = _ptrMapController->screenToGeoCoordinates(_scene->sceneRect().size(),
                                                            _ptrCarrier->getGeoCoord(),
                                                            _cursorCoord);

    int X = _scene->sceneRect().toRect().bottomRight().x();
    int Y = _scene->sceneRect().toRect().bottomRight().y();
    QStringList list;
    list.append(QObject::tr("ГК"));
    list.append(QString(QObject::tr("Ш = %1").arg(ps.latitude())));
    list.append(QString(QObject::tr("Д = %1").arg(ps.longitude())));

    drawText(p, X, Y,list);

    drawCursorText(p);
}

void GraphicsWidget::printMapScale(QPainter *p)
{
    int X = _scene->sceneRect().toRect().topLeft().x();
    int Y = _scene->sceneRect().toRect().topLeft().y();

    double dist = 0.0;
    if(!_ptrMapController.isNull())
    {
        dist = _ptrMapController->getDistanceRadarScale_KM(_scene->sceneRect().size(),
                                                           _ptrCarrier->getGeoCoord(),
                                                           QPointF(_scene->width()/2 + _distToBorderMap,
                                                                   _scene->width()/2 ));
    }

    QStringList list;
    list.append(QString("М = %1 км").arg(dist, 0 , 'f', 2));
    list.append(QObject::tr("Масштаб"));

    drawText(p, X, Y,list);
}

void GraphicsWidget::printCountObject(QPainter *p)
{
    int X = _scene->sceneRect().toRect().topRight().x();
    int Y = _scene->sceneRect().toRect().topRight().y();

    QStringList list;
    list.append(QString("N = %1 ").arg(_scene->items().count()));
    list.append(QObject::tr("Отметок"));

    drawText(p, X, Y,list);
}

void GraphicsWidget::timeout()
{
    _angleGradientSector += 2;

    _scene->update();
}

void GraphicsWidget::slotUpdateData(QSharedPointer<IPoolObject> pool)
{
    _vHiddenObject.clear();
    for (auto &iter: pool->allValues())
    {
        if(!iter.isNull())
            updateObjectOnScene(iter);
    }

    if(!_updateInSector)
        _scene->update();
}

void GraphicsWidget::updateScene()
{
    QGraphicsView::resetCachedContent();
    _scene->update();
}

void GraphicsWidget::drawDotCicleWithLabel(QPainter *p, const qreal rad)
{
    int w = 22 , h = 20;
    p->save();
    //с шагом grad
    for (int i = 0; i < grad; ++i)
    {
        p->setPen(QPen(QBrush(_clrTron), 2));
        if((i % 5) == 0)
            p->setPen(QPen(QBrush(_clrTron), 5));
        //вычисляем координату точки и рисуем её
        QPointF dot = ScreenConversions::polarToScreen(getSceneCenterPont(), i, rad);

        p->drawPoint(dot);

        //здесь делаем надпись каждые 10 градусов
        if((i % 10) != 0)
            continue;

        p->setPen(QPen(QBrush(_clrWhite), 1));

        if((i >= 0) && (i <= 90))
            dot.setY(dot.y() - h);

        if((i >= 180) && (i <= 270))
            dot.setX(dot.x() - w - 2);

        if((i > 270))
        {
            dot.setX(dot.x() - h);
            dot.setY(dot.y() - h);
        }

        p->drawText(QRectF(dot.x(), dot.y(), w , h ),
                    Qt::AlignCenter,
                    QString::number(i));
    }
    p->restore();
}

void GraphicsWidget::RadarScalePlus()
{
    if (_ptrMapController.isNull())
        return;

    if(_ptrMapController->getScale() == _ptrMapController->decScale())
        return;

    recalculateCoordObjects();
}


void GraphicsWidget::RadarScaleMinus()
{
    if (_ptrMapController.isNull())
        return;

    if(_ptrMapController->getScale() == _ptrMapController->incScale())
        return;

    recalculateCoordObjects();
}

void GraphicsWidget::moveMap(int deltaX, int deltaY)
{

    if(_ptrMapController.isNull())
        return;

    _centerCoord = _ptrMapController->screenToGeoCoordinates(scene()->sceneRect().size(),
                                                             _centerCoord,
                                                             QPointF(rect().width()/2.0 + deltaX,
                                                                     rect().height()/2.0 + deltaY));

    //coordConv->setCenterPoint(geoToScreen(_ourPosition));
    recalculateCoordObjects();
    QGraphicsView::resetCachedContent();
    _scene->update();
}

void GraphicsWidget::slotSetObjectCurrent(QUuid id)
{
    scene()->clearSelection();

    if(_hashTable.contains(id))
        _hashTable.value(id)->setSelected(true);

    //если графический объект выбран текущим
    if(_hashTable.value(id)->isSelected() && _fixCursor)
        _fixCursorCoord = _hashTable.value(id)->pos();
    _scene->update();
}
