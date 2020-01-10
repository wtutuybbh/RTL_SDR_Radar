#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <memory>

#include "interface/IMapController.h"
#include "osm/OSMTileSource.h"
#include "coord/Position.h"
#include "coord/Conversions.h"
/*!
 * \brief The MapController class
 * реализация интерфеса контроллера  карты
 */
class MapController : public IMapController
{
    Q_OBJECT

    //карта
    std::unique_ptr<OSMTileSource> _tileSource = nullptr;

    Position _centralGeoPosition;

    uint8_t _zoom = 8;
    /*!
     * \brief addFilterImage - наложение фильтра на изображение
     * \param pxm - QPixmap растрового изображения карты
     * \param type - тип фильтра
     * \param radius - радиус изображения в режиме круга
     * \return растровое изображение карты
     */
    QImage addFilterImage(const QPixmap &pxm,
                          const QSizeF &size,
                          FILTER_TYPE type,
                          double radius = 0);
    /*!
     * \brief createRadarImage - преобразование изображение в форму радара
     * \param img - исходное изображение QImage
     * \param size - размер области отрисовки
     * \param radius - радиус радара в пикселях
     */
    void createRadarImage(QImage & img,
                          const QSizeF &size,
                          double radius);
    /*!
     * \brief createGrayImage - преобразование изображения в ночной режим
     * \param img - исходное изображение для изменения
     */
    void createGrayImage(QImage &img);

public:
    explicit MapController();
    ~MapController() override;

    /*!
     * \brief getImageMap - получение картографической основы в растровом формате
     * \param size - размер изображения
     * \param center - центральная точка карты (широта и долгота)
     * \param radius - радиус круга для режима локатора, учитывается только при
     * FILTER_TYPE  - NIGHT_AND_CIRCLE, CIRCLE
     * \param type - тип изображения (цветоное или наложение фильтра)
     * \return растровое изображение карты
     */
    QImage getImageMap( const QSizeF &size,
                        const Position &center,
                        double radius = 0,
                        FILTER_TYPE filterType = FILTER_TYPE::NO) override;

    /*!
     * \brief getCenterGeoPoint центральная точка в географических координатах
     * \return текущее значение
     */
    Position getCenterGeoPoint() const override;

    /*!
     * \brief setCenterGeoPoint - установка текущего значения центральной точки
     *  в географических координатах
     * \param geoCenter - значение широты и долготы
     */
    void setCenterGeoPoint(const Position &geoCenter) override;

    /*!
     * \brief screenToRealPolar перевод экранных координат
     * в значения пеленг-дальность c учетом масштаба
     * пеленг в градусах, дальность в метрах
     * \param xy - значение экранных координат
     * \return  пеленг - дальность
     */
    PolarCoord screenToRealPolar(const QSizeF &size,
                                 const Position &centerCoord,
                                 const QPointF &xy) override;

    /*!
     * \brief realPolarToScreen - перевод значения пеленг - дальность
     * в экранные координаты
     * пеленг в градусах, дальностьв метрах
     * \param size - размер картографической основы в пикселя
     * \param centerCoord - центральная геокоордината
     * \param plr - значение пеленг-дальность
     * \return экранные координаты
     */
    QPointF realPolarToScreen(const QSizeF &size,
                              const Position &centerCoord,
                              const PolarCoord &plr) override;

    /*!
     * \brief screenToGeoCoordinates перевод экранных координат в
     * географические (широта и долгота)
     * \param size - размер картографической основы в пикселя
     * \param centerCoord - центральная геокоордината
     * \param point - экранная точка
     * \return значение геоокординаты
     */
    Position screenToGeoCoordinates(const QSizeF &size,
                                    const Position &centerCoord,
                                    const QPointF &point) override;
    /*!
     * \brief geoToScreenCoordinates - перевод географисеких координат в
     * экранные (широта и догота)
     * \param size - размер картографической основы в пикселях
     * \param centerCoord - центральная геоточка
     * \param position - геокоордината точки для перевода
     * \return значений экранной координаты в пикселях
     */
    QPointF geoToScreenCoordinates(const QSizeF &size,
                                   const Position &centerCoord,
                                   const Position &position) override;

    /*!
     * \brief getDistanceRadarScale_KM - дистанция в километрах от края до края карты
     * в текущем масштабе
     * \return дистанция в километрах
     */
    double getDistanceRadarScale_KM(const QSizeF &size,
                                    const Position &centerCoordinate,
                                    const QPointF mapBorderCordinate) override;
    /*!
     * \brief getDistanceRadarScale_M - дистанция в метрах от края до края карты
     * в текущем масштабе
     * \return дистанция в километрах
     */
    double getDistanceRadarScale_M(const QSizeF &size,
                                   const Position &centerCoordinate,
                                   const QPointF mapBorderCordinate) override;

    //масштаб
    int getScale() override;
    void setScale(uint8_t scale) override;
    int incScale() override;
    int decScale() override;

    /*!
     * \brief isVisibleInCurrentScale
     *  проверка попадает ли точка в область видимости
     * \param dist - дистанци в метрах
     * \return true если точка попадает в зону видимости
     */
    bool isVisibleInCurrentScale(double dist) override;

    /*!
     * \brief getDistanceObject_KM - рассчёт дистанции
     * между точками в километрах
     * \param centerCoord - начальная координата
     * \param dot - конечная координата
     * \return  дистанция в км
     */
    double getDistanceObject_KM(const Position &centerCoord,
                             const Position &dot) override;
    /*!
     * \brief getDistanceObject_M - рассчёт дистанции
     * между точками в метрах
     * \param centerCoord - начальная координата
     * \param dot - конечная координата
     * \return  дистанция в метрах
     */
    double getDistanceObject_M(const Position &centerCoord,
                             const Position &dot) override;

signals:
    void updateTileGride();

public slots:
};

#endif // MAPCONTROLLER_H
