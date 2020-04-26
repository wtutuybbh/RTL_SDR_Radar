#include "MapController.h"

#include <QPixmap>
#include <QPointF>
#include <QPainter>

#include "osm/coordUtils/ScreenConversions.h"

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
                                  double radius,
                                  FILTER_TYPE filterType)
{
    uint8_t zoomLevel = _zoom + 3;

    //переводим географические координаты в метры
    QPointF qgsPos = _tileSource->ll2qgs(center.lonLat(), zoomLevel);

    //весь мир у нас состоит из тайлов, размер одного тайла 256 пикселей
    const quint16 tileSize = _tileSource->tileSize();

    //узнаем сколько тайлов поместиться в нашем окне
    const qint32 tilesPerRow = uint32_t(size.width()) / _tileSource->tileSize() + 4;
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
            int lon = qRound(qgsPos.x() - x * tileSize) / tileSize;
            int lat = qRound(qgsPos.y() - y * tileSize) / tileSize;

            //запрос тайла в необходимом нам масштабе
            QPixmap pmMap = _tileSource->startTileRequest(uint32_t(lon) , uint32_t(lat), zoomLevel);

            if(!pmMap.isNull())
            {
                //тайл мы получили,теперь его надо отцентровать,для этого вычислим сдижку в пикселях
                double shift_x = (qgsPos.x() - (qRound(qgsPos.x()) / tileSize) * tileSize) ;
                double shift_y = (qgsPos.y() - (qRound(qgsPos.y()) / tileSize) * tileSize);
                //отрисуем карту
                p.drawPixmap( qRound(size.width() / 2 - shift_x - x * tileSize),
                              qRound(size.height() / 2 - shift_y - y * tileSize),
                              pmMap);
                yyc++;
            }
        }
        yyc = 0;
        xxc++;
    }

    return addFilterImage(pxm, size, filterType, radius);
}


Position MapController::getCenterGeoPoint() const
{
    return _centralGeoPosition;
}

void MapController::setCenterGeoPoint(const Position &geoCenter)
{
    _centralGeoPosition = geoCenter;
}

PolarCoord MapController::screenToRealPolar(const QSizeF &size,
                                            const Position &centerCoord,
                                            const QPointF &xy)
{
    Q_UNUSED(size)
    Q_UNUSED(centerCoord)
    Q_UNUSED(xy)
    return PolarCoord(0.0, 0.0);
}

QPointF MapController::realPolarToScreen(const QSizeF &size,
                                         const Position &centerCoord,
                                         const PolarCoord &plr)
{
    Q_UNUSED(size)
    Q_UNUSED(centerCoord)
    Q_UNUSED(plr)
    return QPointF();
}

Position MapController::screenToGeoCoordinates(const QSizeF &size,
                                               const Position &centerCoord,
                                               const QPointF &point)
{
    QPointF dot;
    uint8_t zoomLevel = _zoom + 3;
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
    uint8_t zoomLevel = _zoom + 3;
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


void MapController::setScale(uint8_t scale)
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
    Q_UNUSED(dist)
    return false;
}

//TODO : вынести графическую обработку изображения в отдельный поток
void MapController::createRadarImage(QImage & img, const QSizeF &size, double radius)
{
    QImage shapeImg(size.toSize(),
                    QImage::Format_ARGB32_Premultiplied);

    QRectF mapRect(QPointF(0.0, 0.0), size);
    shapeImg.fill(QColor(0, 0, 0, 0).rgba());
    QPainter sp(&shapeImg);
    sp.setRenderHint(QPainter::Antialiasing);
    sp.fillRect(QRectF(0.0,0.0,size.width(),size.height()), Qt::transparent);
    sp.setPen(QPen(Qt::black));
    sp.setBrush(QBrush(Qt::black));
    double center = size.width() / 2;

    sp.drawEllipse(QPointF(center, center),
                   radius,
                   radius);
    sp.end();

    QImage roundSquaredImage(size.toSize(),
                             QImage::Format_ARGB32_Premultiplied);
    roundSquaredImage.fill(QColor(0, 0, 0, 0).rgba());

    QPainter p(&roundSquaredImage);
    p.fillRect(QRectF(0.0,0.0,size.width(),size.height()), Qt::transparent);
    p.drawImage(0, 0, shapeImg);
    p.setPen(QPen(Qt::black));
    p.setBrush(QBrush(Qt::black));
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.drawImage(0, 0, img);
    p.end();

    img = roundSquaredImage;
}

//TODO : вынести графическую обработку изображения в отдельный поток
void MapController::createGrayImage(QImage& img)
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

QImage MapController::addFilterImage(const QPixmap &pxm,
                                     const QSizeF &size,
                                     FILTER_TYPE type,
                                     double radius)
{
    QImage img = pxm.toImage();

    if (img.isNull())
        return QImage();

    //ночной режим
    if(( type == FILTER_TYPE::NIGHT ) || (type == FILTER_TYPE::NIGHT_AND_CIRCLE))
        createGrayImage(img);

    //наложение круга
    if((type == FILTER_TYPE::CIRCLE) ||  (type == FILTER_TYPE::NIGHT_AND_CIRCLE))
        createRadarImage(img, size, radius);

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
