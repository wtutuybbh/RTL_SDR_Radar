#include "MapController.h"

#include <QPixmap>
#include <QPointF>
#include <QPainter>

#include "coord/ScreenConversions.h"

MapController::MapController()
{
    _tileSource = std::unique_ptr<OSMTileSource>(new OSMTileSource());

    QObject::connect(_tileSource.get(), &OSMTileSource::endNetworkLoadTile,
                     this, &MapController::updateTileGride);
}

MapController::~MapController()
{

}

QImage MapController::getImageMap(const QSizeF &size,
                                  const Position &center,
                                  FilterType filterType)
{
    int zoomLevel = _zoom + 3;

    //переводим географические координаты в метры
    QPointF qgsPos = _tileSource->ll2qgs(center.lonLat(), zoomLevel);

    //весь мир у нас состоит из тайлов, размер одного тайла 256 пикселей
    const quint16 tileSize = _tileSource->tileSize();

    //узнаем сколько тайлов поместиться в нашем окне
    const qint32 tilesPerRow = size.width() / _tileSource->tileSize() + 4;
    const qint32 tilesPerCol = tilesPerRow;

    QPixmap pxm = QPixmap(qRound(size.width()),
                          qRound(size.height()));
    pxm.fill(Qt::black);

    QPainter p(&pxm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    int xxc = 0;
    int yyc = 0;

    //построение картинки происходит относительно центра,
    for (qint32 x = -tilesPerRow / 2; x < tilesPerRow / 2; x++)
    {
        for (qint32 y = -tilesPerCol / 2; y < tilesPerCol / 2; y++)
        {
            //используя координаты центра вычисляем номер тайла который нужно запросить
            int lon = (qgsPos.x() - x * tileSize) / tileSize;
            int lat = (qgsPos.y() - y * tileSize) / tileSize;

            //запрос тайла в необходимом нам масштабе
            QPixmap pmMap = _tileSource->startTileRequest(lon , lat, zoomLevel);

            if(!pmMap.isNull())
            {
                //тайл мы получили,теперь его надо отцентровать,для этого вычислим сдижку в пикселях
                double shift_x = (qgsPos.x() - ((int)qgsPos.x() / tileSize) * tileSize) ;
                double shift_y = (qgsPos.y() - ((int)qgsPos.y() / tileSize) * tileSize);
                //отрисуем карту
                p.drawPixmap( (double)size.width() / 2 - shift_x - x * tileSize,
                              (double)size.height() / 2 - shift_y - y * tileSize,
                              pmMap);
                yyc++;
            }
        }
        yyc = 0;
        xxc++;
    }

    return addFilterImage(pxm,filterType);
}


Position MapController::getCenterGeoPoint() const
{
    return Position();
}

void MapController::setCenterGeoPoint(const Position &geoCenter)
{

}

PolarCoord MapController::screenToRealPolar(const QSizeF &size,
                                            const Position &centerCoord,
                                            const QPointF &xy)
{
    return PolarCoord(0.0, 0.0);
}

QPointF MapController::realPolarToScreen(const QSizeF &size,
                                         const Position &centerCoord,
                                         const PolarCoord &plr)
{
    return QPointF();
}

Position MapController::screenToGeoCoordinates(const QSizeF &size,
                                               const Position &centerCoord,
                                               const QPointF &point)
{
    QPointF dot;
    int zoomLevel= _zoom + 3;
    QPointF temp = _tileSource->ll2qgs(centerCoord.lonLat(),zoomLevel);

    dot.setX(point.x() - size.width() / 2 + temp.x());
    dot.setY(point.y() - size.height() / 2 + temp.y());

    return Position (_tileSource->qgs2ll(dot,zoomLevel));
}

QPointF MapController::geoToScreenCoordinates(const QSizeF &size,
                                              const Position &centerCoord,
                                              const Position &position)
{
    QPointF dot;
    int zoomLevel = _zoom + 3;
    QPointF temp = _tileSource->ll2qgs(centerCoord.lonLat(), zoomLevel);

    dot.setX(_tileSource->ll2qgs(position.lonLat(),
                                 zoomLevel).x() + size.width() / 2 - temp.x());

    dot.setY(_tileSource->ll2qgs(position.lonLat(),
                                 zoomLevel).y() + size.height() / 2 - temp.y());
    return dot;
}

double MapController::getDistanceRadarScale_KM(const QSizeF &size,
                                               const Position &centerCoordinate,
                                               const QPointF mapBorderCordinate)
{
    return getDistanceRadarScale_M(size,centerCoordinate,mapBorderCordinate)/ 1000;
}

double MapController::getDistanceRadarScale_M(const QSizeF &size,
                                              const Position &centerCoordinate,
                                              const QPointF mapBorderCordinate)
{
    Position sp0 = screenToGeoCoordinates(size,
                                          centerCoordinate,
                                          mapBorderCordinate);

    return centerCoordinate.flatDistanceEstimate(sp0) * 2 ;
}

int MapController::getScale()
{
    return _zoom;
}


void MapController::setScale(int scale)
{
    _zoom = scale;
}

int MapController::incScale()
{
    return  (_zoom < 15) ? ++_zoom : _zoom;
}

int MapController::decScale()
{
     return  (_zoom > 0) ? --_zoom : _zoom;
}

bool MapController::isVisibleInCurrentScale(double dist)
{
    return false;
}

QImage MapController::addFilterImage(const QPixmap &pxm, FilterType type)
{
    QImage img = pxm.toImage();

    //ночной режим
    if(type == FilterType::Night)
    {
        int pixels = img.width() * img.height();
        if (pixels * (int)sizeof(QRgb) <= img.byteCount())
        {
            QRgb *data = (QRgb *)img.bits();
            int val = 0;
            for (int i = 0; i < pixels; i++)
            {
                val = qGray(data[i]);
                data[i] = qRgba(0 , -val, -val, qAlpha(data[i]));
            }
        }
    }
    return img;
}

double MapController::getDistanceObject_M(const Position& centerCoord,
                                        const Position& dot)
{
    return centerCoord.flatDistanceEstimate(dot);
}

double MapController::getDistanceObject_KM(const Position& centerCoord,
                                        const Position& dot)
{
    return getDistanceObject_M(centerCoord, dot) / 1000.0;
}
