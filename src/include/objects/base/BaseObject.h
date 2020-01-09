#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QDateTime>
#include <QObject>
#include "interface/IObject.h"

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


    QUuid getUuid() override;

    void setId(uint64_t id) override;
    //id из
    uint64_t getId() const override;

    //set/get время регистрации
    void setDateTimeStart(const QDateTime &dt) override;
    QDateTime getDateTimeStart() const override;

    //set/get время последнего обнаружения
    void setDateTimeStop(const QDateTime &dt) override;
    QDateTime getDateTimeStop() const override;

    //set/get время регистрации в мсек с начала эпохи
    void setMSecStart(const qint64 dt) override;
    qint64 getMSecStart() const override;

    //set/get время последнего обнаружения в мсек с начала эпохи
    void setMSecStop(const qint64 dt) override;
    qint64 getMSecStop() const override;

    //set/get состояние объекта
    void setObjectState(OBJECT_STATE state) override;
    OBJECT_STATE getObjectState() override;

    // имя носителя
    void setObjectName(const QString& str) override;
    QString getObjectName() override;

    //пеленг
    void setAzimuth(double azim) override;
    double getAzimuth() const override;

    //угол места
    void setElevation(double elev) override;
    double getElevation() const override;

    //set/get дистанции
    void setDistance_KM(double dist) override;
    double getDistance_KM() override;

    void setDistance_M(double dist) override;
    double getDistance_M() const  override;

    //set/get гео координаты
    void setGeoCoord(const Position &gp) override;
    Position getGeoCoord() const override;
    bool isValidGeoCoord() override;

    //set/get скорость
    void setSpeed(float value) override;
    float getSpeed() const override;

    //курс
    void setCourse(float crs) override;
    float getCourse() override;

    bool getInUse() override;

    void resetObjectData() override;

    float getAltitude() const override;
    void setAltitude(float altitude) override;

    bool isImitated() override;

    OBJECT_TYPE getTypeObject() override;

signals:

public slots:
};

#endif
