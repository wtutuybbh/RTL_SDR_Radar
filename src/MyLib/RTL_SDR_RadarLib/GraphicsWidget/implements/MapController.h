#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <memory>

#include "interface/IMapController.h"
#include "osm/OSMTileSource.h"
#include "coord/Position.h"
#include "coord/Conversions.h"

class MapController : public IMapController
{
    Q_OBJECT
    //карта
    std::unique_ptr<OSMTileSource> _tileSource = nullptr;

    QImage addFilterImage(const QPixmap &pxm,  FilterType type);

public:
    explicit MapController();
    ~MapController() override;

    QImage getImageMap(int w,
                        int h,
                        const Position &center,
                        int zoom,
                        FilterType filterType = FilterType::No) override;

    QImage getImageMap( int w,
                        int h,
                        int zoom,
                        FilterType type = FilterType::No) override;

    //центральная точка в географических координатах
    Position getCenterGeoPoint() const override;
    void setCenterGeoPoint(const Position &geoCenter) override;

    //экранные координаты в полярные c учетом масштаба
    SPolarCoord screenToRealPolar(const QPointF &xy) override;
    QPointF realPolarToScreen(const SPolarCoord &plr) override;

    Position screenToGeoCoordinates(const QPointF &point) override;
    QPointF geoToScreenCoordinates(const Position &sgc) override;

    double getDistanceRadarScale_KM() override;
    double getDistanceRadarScale_M() override;

    //масштаб
    double getScale() override;
    void setScale(double scale) override;

    //проверка попадает ли точка в область видимости
    bool isVisibleInCurrentScale(double dist) override;

signals:
    void updateTileGride();

public slots:
};

#endif // MAPCONTROLLER_H
