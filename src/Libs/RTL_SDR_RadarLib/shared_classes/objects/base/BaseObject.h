#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QDateTime>
#include <QObject>
#include "interface/IObject.h"
#include "../shared_classes/coord/Position.h"

/*!
 * \brief The BaseObject class
 * Базовый класс для всех радиотехнических объектов
 * \author Данильченко Артем
 */
class BaseObject :  public IObject
{
    Q_OBJECT

    ///< внешний идентификатор  - icao,id DB,hash и тд.
    uint64_t _id = 0;
    ///< состояние объекта
    OBJECT_STATE _state = OBJECT_STATE::NEW_OBJECT;
    ///< флаг использования объекта
    /// true - испозьзуется
    /// false - можно перезаписывать параметры и использовать
    /// повторно
    bool _inUse = true;
    ///< уникальный идентификатор объекта
    /// никогда не изменяется
    QUuid _uuid;
    ///< тип объекта
    OBJECT_TYPE _typeObject;
    ///< флаг имитированного объекта
    bool _isImitate = false;
protected:

    QString _nameObject = QString("--");

    //время когда был зарегестрирован объект
    int64_t _ms_tstart = 0;
    QDateTime  _tstart;

    //время когда последний раз были обновлены данные
    int64_t _ms_tstop = 0;
    QDateTime  _tstop;

    Position _geoCoord;
    //скорость
    float _speed = 0.0;
    //курс
    float _course = 0.0;
    //пеленг
    double _azimuth = 0.0;
    //угол места
    double _elevation = 0.0;
    //дальность
    double _distance = 0.0;
    // высота
    float _altitude = 0.0;
    /*!
     * \brief setInUse флаг актуальности объекта
     * Используется в пулле объектов
     * \param value - true - объект используется
     */
    void setInUse(bool value) override;
public:
    BaseObject(uint64_t id,
               QDateTime tstart,
               double azimuth,
               double elevation,
               OBJECT_TYPE type = OBJECT_TYPE::base,
               bool isImit = false);

    BaseObject(uint64_t id,
               QDateTime tstart,
               OBJECT_TYPE type = OBJECT_TYPE::base,
               bool isImit = false,
               Position geoPosition = Position());

    ~BaseObject() override;

    /*!
     * \brief getUuid получение uuid
     * uuid генерируется при создании объекта
     * \return значение uuid
     */
    QUuid getUuid() override;
    /*!
     * \brief setId установка идентификатора объекта
    */
    void setId(uint64_t id) override;
    /*!
     * \brief getId получение идентияикатора объекта
     * \return
     */
    uint64_t getId() const override;

    /*!
     * \brief setDateTimeStart установка времени регистрации орбъекта
     * \param dt - локальное время
     */
    void setDateTimeStart(const QDateTime &dt) override;
    /*!
     * \brief getDateTimeStart получение время регистрации объекта
     * \return локальное ремя начала регистрации объекта
     */
    QDateTime getDateTimeStart() const override;

    /*!
     * \brief setDateTimeStop - время последней регистрации объекта
     * \param dt - локальное время
     */
    void setDateTimeStop(const QDateTime &dt) override;
    /*!
     * \brief getDateTimeStop - получение времени последней регистрации объекта
     * \return
     */
    QDateTime getDateTimeStop() const override;

    /*!
     * \brief setMSecStart время начало регистрации с начало эпохи
     * \param dt - время в мс с начала эпохи
     */
    void setMSecStart(const qint64 dt) override;
    /*!
     * \brief getMSecStart - время начала регистрации с начала эпохи
     * \return - время в мс с начала эпохи
     */
    qint64 getMSecStart() const override;

    /*!
     * \brief setMSecStop - время последней регистрации
     * \param dt - время с начала эпохи в мс
     */
    void setMSecStop(const qint64 dt) override;
    /*!
     * \brief setMSecStop - время последней регистрации
     * \return время с начала эпохи в мс
     */
    qint64 getMSecStop() const override;

    /*!
     * \brief setObjectState -установка текущего состояни объекта
     * \param state - состояние - новый / обновлен / удаление / не обновлен
     */
    void setObjectState(OBJECT_STATE state) override;
    /*!
     * \brief getObjectState получение текущего состояния объекта
     * \return состояние - новый / обновлен / удаление / не обновлен
     */
    OBJECT_STATE getObjectState() override;

    /*!
     * \brief setObjectName установка имени объекта
     * \param str - строка
     */
    void setObjectName(const QString& str) override;
    /*!
     * \brief getObjectName получение имени объекта
     * \return строка
     */
    QString getObjectName() override;

    /*!
     * \brief setAzimuth установка значения пеленга
     * \param azim - значения 0 - 360 относительно севера
     */
    void setAzimuth(double azim) override;
    /*!
     * \brief getAzimuth получение значения пеленга
     * \return значения 0 - 360 относительно севера
     */
    double getAzimuth() const override;

    /*!
     * \brief setElevation угол места
     * \param elev значения 0 - 90
     */
    void setElevation(double elev) override;
    /*!
     * \brief getElevation получение значения угола места
     * \param elev значения 0 - 90
     */
    double getElevation() const override;

    /*!
     * \brief setDistance_KM - установка значения дистанции
     * \param dist - дистанция в км
     */
    void setDistance_KM(double dist) override;
    /*!
     * \brief getDistance_KM - получение значения дистанции
     * \return дистанция в км
     */
    double getDistance_KM() override;
    /*!
     * \brief setDistance_M установка значения дистанции
     * \param dist - дистанция в метрах
     */
    void setDistance_M(double dist) override;
    /*!
     * \brief getDistance_M - получение значения дистанции
     * \return дистанция в метрах
     */
    double getDistance_M() const  override;

    /*!
     * \brief setGeoCoord - узтановка значения геокоординат
     * \param gp геоокординаты - широта / долгота
     */
    void setGeoCoord(const Position &gp) override;
    /*!
     * \brief getGeoCoord получение геокоординат
     * \return значение широты / долготы
     */
    Position getGeoCoord() const override;
    /*!
     * \brief isValidGeoCoord - проверка на корректность геокоординат
     * \return true - координаты корректны
     */
    bool isValidGeoCoord() override;

    /*!
     * \brief setSpeed - скорость движения
     * \param value - значение скорости движения
     */
    void setSpeed(float value) override;
    /*!
     * \brief getSpeed - получение яскорости движения
     * \return  скорость
     */
    float getSpeed() const override;

    /*!
     * \brief setCourse - значение курса
     * \param crs курс задается в диапазоне
     * 0 - 360 относительно севера
     */
    void setCourse(float crs) override;
    /*!
     * \brief getCourse - получение значения курса
     * \return  значение в диапазоне 0-360 относительно севера
     */
    float getCourse() override;
    /*!
     * \brief setInUse флаг актуальности объекта
     * Используется в пулле объектов
     * \return value - true - объект используется
     */
    bool getInUse() override;
    /*!
     * \brief resetObjectData -сброс параметров объета.
     * Для повторного использования в пулле объектов
     */
    void resetObjectData() override;
    /*!
     * \brief getAltitude высота над уровнем моря
     * \return значение высоты с метрах
     */
    float getAltitude() const override;
    /*!
     * \brief setAltitude - высота над уровнем моря
     * \param altitude значение в метрах
     */
    void setAltitude(float altitude) override;
    /*!
     * \brief isImitated - проверка является ли объект имитированным
     * \return true - имитированный объект
     */
    bool isImitated() override;
    /*!
     * \brief getTypeObject - получение типа объекта
     * воздушный,наземный, неизвестный
     * \return
     */
    OBJECT_TYPE getTypeObject() override;

signals:

public slots:
};

#endif
