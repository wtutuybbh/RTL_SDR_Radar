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

QImage MapController::getImageMap(int w,
                                  int h,
                                  const Position &center,
                                  int zoom,
                                  FilterType filterType)
{
    int zoomLevel = zoom + 3;

    //переводим географические координаты в метры
    QPointF qgsPos = _tileSource->ll2qgs(center.lonLat(), zoomLevel);

    //весь мир у нас состоит из тайлов, размер одного тайла 256 пикселей
    const quint16 tileSize = _tileSource->tileSize();

    //узнаем сколько тайлов поместиться в нашем окне
    const qint32 tilesPerRow = w / _tileSource->tileSize() + 4;
    const qint32 tilesPerCol = tilesPerRow;

    QPixmap pxm = QPixmap(w, h);
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
                p.drawPixmap( (double)w / 2 - shift_x - x * tileSize,
                              (double)w / 2 - shift_y - y * tileSize,
                              pmMap);
                yyc++;
            }
        }
        yyc = 0;
        xxc++;
    }

    return addFilterImage(pxm,filterType);
}

QImage MapController::getImageMap(int w, int h, int zoom, FilterType type)
{
    return QImage();
}

Position MapController::getCenterGeoPoint() const
{
    return Position();
}

void MapController::setCenterGeoPoint(const Position &geoCenter)
{

}

SPolarCoord MapController::screenToRealPolar(const QPointF &xy)
{
    return SPolarCoord(0.0, 0.0);
}

QPointF MapController::realPolarToScreen(const SPolarCoord &plr)
{
    return QPointF();
}

Position MapController::screenToGeoCoordinates(int w,
                                               int h,
                                               const Position &centerCoord,
                                               const QPointF &point,
                                               int zoom)
{
    QPointF dot;
    int _zoomLevel= zoom +3;
    QPointF temp = _tileSource->ll2qgs(centerCoord.lonLat(),_zoomLevel);

    dot.setX(point.x() - w / 2 + temp.x());
    dot.setY(point.y() - h / 2 + temp.y());

    return Position (_tileSource->qgs2ll(dot,_zoomLevel));
}

QPointF MapController::geoToScreenCoordinates(int w,
                                              int h,
                                              const Position &centerCoord,
                                              const Position &position,
                                              int zoom )
{
    QPointF dot;
    int _zoomLevel = zoom + 3;
    QPointF temp = _tileSource->ll2qgs(centerCoord.lonLat(), _zoomLevel);

    dot.setX(_tileSource->ll2qgs(position.lonLat(),
                                 _zoomLevel).x() + w / 2 - temp.x());

    dot.setY(_tileSource->ll2qgs(position.lonLat(),
                                 _zoomLevel).y() + h / 2 - temp.y());
    return dot;
}

double MapController::getDistanceRadarScale_KM()
{
    return 0.0;
}

double MapController::getDistanceRadarScale_M()
{
    return 0.0;
}

double MapController::getScale()
{
    return 0.0;
}

void MapController::setScale(double scale)
{

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

