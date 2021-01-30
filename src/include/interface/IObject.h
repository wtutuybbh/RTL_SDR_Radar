#ifndef IOBJECT
#define IOBJECT

#include <QObject>
#include <QUuid>


enum class OBJECT_STATE
{
    NEW_OBJECT = 0,
    UPDATE_OBJECT,
    DELETE_OBJECT,
    NOT_UPDATE_OBJECT,
};

enum class OBJECT_TYPE
{
    base = 0,
    air,
    surface,
};

/*!
 * \brief The IObject class
 * Интерфейсный класс для описания
 * радиотехнического объекта
 */

class Position;

class IObject : public QObject
{
    Q_OBJECT
public:
    virtual ~IObject() {}

    /*!
     * \brief getUuid получение uuid
     * uuid генерируется при создании объекта
     * \return значение uuid
     */
    virtual QUuid getUuid() = 0;

    /*!
     * \brief setId установка идентификатора объекта
    */
    virtual void setId(uint64_t) = 0 ;
    /*!
     * \brief getId получение идентияикатора объекта
     * \return
     */
    virtual uint64_t getId() const = 0;

    /*!
     * \brief setDateTimeStart установка времени регистрации орбъекта
     * \param dt - локальное время
     */
    virtual void setDateTimeStart(const QDateTime &dt) = 0;
    /*!
     * \brief getDateTimeStart получение время регистрации объекта
     * \return локальное ремя начала регистрации объекта
     */
    virtual QDateTime getDateTimeStart() const = 0;

    /*!
     * \brief setDateTimeStop - время последней регистрации объекта
     * \param dt - локальное время
     */
    virtual void setDateTimeStop(const QDateTime &dt) = 0;
    /*!
     * \brief getDateTimeStop - получение времени последней регистрации объекта
     * \return
     */
    virtual QDateTime getDateTimeStop() const = 0;

    /*!
     * \brief setMSecStart время начало регистрации с начало эпохи
     * \param dt - время в мс с начала эпохи
     */
    virtual void setMSecStart(const qint64 dt) = 0;
    /*!
     * \brief getMSecStart - время начала регистрации с начала эпохи
     * \return - время в мс с начала эпохи
     */
    virtual qint64 getMSecStart() const = 0;

    /*!
     * \brief setMSecStop - время последней регистрации
     * \param dt - время с начала эпохи в мс
     */
    virtual void setMSecStop(const qint64 dt) = 0;
    /*!
     * \brief setMSecStop - время последней регистрации
     * \return время с начала эпохи в мс
     */
    virtual qint64 getMSecStop() const = 0;

    /*!
     * \brief setObjectState -установка текущего состояни объекта
     * \param state - состояние - новый / обновлен / удаление / не обновлен
     */
    virtual void setObjectState(OBJECT_STATE state) = 0;
    /*!
     * \brief getObjectState получение текущего состояния объекта
     * \return состояние - новый / обновлен / удаление / не обновлен
     */
    virtual OBJECT_STATE getObjectState() = 0;

    /*!
     * \brief setObjectName установка имени объекта
     * \param str - строка
     */
    virtual void setObjectName(const QString& str) = 0;
    /*!
     * \brief getObjectName получение имени объекта
     * \return строка
     */
    virtual QString getObjectName() = 0;

    /*!
     * \brief setAzimuth установка значения пеленга
     * \param azim - значения 0 - 360 относительно севера
     */
    virtual void setAzimuth(double azim) = 0;
    /*!
     * \brief getAzimuth получение значения пеленга
     * \return значения 0 - 360 относительно севера
     */
    virtual double getAzimuth() const = 0;

    /*!
     * \brief setElevation угол места
     * \param elev значения 0 - 90
     */
    virtual void setElevation(double elev) = 0;
    /*!
     * \brief getElevation получение значения угола места
     * \param elev значения 0 - 90
     */
    virtual double getElevation() const = 0;

    /*!
     * \brief setDistance_KM - установка значения дистанции
     * \param dist - дистанция в км
     */
    virtual void setDistance_KM(double dist) = 0;
    /*!
     * \brief getDistance_KM - получение значения дистанции
     * \return дистанция в км
     */
    virtual double getDistance_KM() = 0;
    /*!
     * \brief setDistance_M установка значения дистанции
     * \param dist - дистанция в метрах
     */
    virtual void setDistance_M(double dist) = 0;
    /*!
     * \brief getDistance_M - получение значения дистанции
     * \return дистанция в метрах
     */
    virtual double getDistance_M() const = 0;
    /*!
     * \brief setGeoCoord - узтановка значения геокоординат
     * \param gp геоокординаты - широта / долгота
     */
    virtual void setGeoCoord(const Position &gp) = 0;
    /*!
     * \brief getGeoCoord получение геокоординат
     * \return значение широты / долготы
     */
    virtual Position getGeoCoord() const = 0;
    /*!
     * \brief isValidGeoCoord - проверка на корректность геокоординат
     * \return true - координаты корректны
     */
    virtual bool isValidGeoCoord() = 0;

    /*!
     * \brief setSpeed - скорость движения
     * \param value - значение скорости движения
     */
    virtual void setSpeed(float value) = 0;
    /*!
     * \brief getSpeed - получение яскорости движения
     * \return  скорость
     */
    virtual float getSpeed() const = 0;

    /*!
     * \brief setCourse - значение курса
     * \param crs курс задается в диапазоне
     * 0 - 360 относительно севера
     */
    virtual void setCourse(float crs) = 0;
    /*!
     * \brief getCourse - получение значения курса
     * \return  значение в диапазоне 0-360 относительно севера
     */
    virtual float getCourse() = 0;

    /*!
     * \brief setAltitude - высота над уровнем моря
     * \param altitude значение в метрах
     */
    virtual void setAltitude(float altitude) = 0 ;
    /*!
     * \brief getAltitude высота над уровнем моря
     * \return значение высоты с метрах
     */
    virtual float getAltitude() const = 0;

    /*!
     * \brief setInUse флаг актуальности объекта
     * Используется в пулле объектов
     * \param value - true - объект используется
     */
    virtual void setInUse(bool value) = 0;
    /*!
     * \brief setInUse флаг актуальности объекта
     * Используется в пулле объектов
     * \return value - true - объект используется
     */
    virtual bool getInUse() = 0;
    /*!
     * \brief resetObjectData -сброс параметров объета.
     * Для повторного использования в пулле объектов
     */
    virtual void resetObjectData() = 0;

    /*!
     * \brief isImitated - проверка является ли объект имитированным
     * \return true - имитированный объект
     */
    virtual bool isImitated() = 0;
    /*!
     * \brief getTypeObject - получение типа объекта
     * воздушный,наземный, неизвестный
     * \return
     */
    virtual OBJECT_TYPE getTypeObject() = 0;


    /*!
     * \brief serialize сериализация объекта в байтовый массив
     * для передачи по сети
     * \return байтовый массив
     */
    virtual QByteArray serialize() = 0;

    /*!
     * \brief unserialize ансериализация данных
     * \param array байтовый массив
     * \return результат работы
     */
    virtual bool unserialize(QByteArray array) = 0;

    /*!
     * \brief serializedFrameSize размер блока данных
     * после сериализации
     * \return размер данных
     */
    virtual uint32_t  serializedFrameSize() = 0 ;
};
#endif // IOBJECT

