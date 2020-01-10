#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <stdint.h>
#include <QString>
#include <math.h>
#include "objects/base/BaseObject.h"
/*!
 * \brief The Aircraft class
 * Класс объекта типа самолёт
 */
class Aircraft : public BaseObject
{
    static constexpr char SIZE_TEXT = 9;

    //ЦМР для сериализации данных
    const uint32_t VALUE_LSB = 100;
    const double LON_VALUE_LSB = 360.0 / pow(2, 31);
    const double LAT_VALUE_LSB = 180.0 / pow(2, 31);

    char _flight[SIZE_TEXT];     /* Flight number */
    uint64_t _messages;      /* Number of Mode S messages received. */
    /* Encoded latitude and longitude as extracted by odd and even
     * CPR encoded messages. */
    int _odd_cprlat;
    int _odd_cprlon;
    int _even_cprlat;
    int _even_cprlon;

    /* Coordinated obtained from CPR encoded data. */
    int64_t _odd_cprtime;
    int64_t _even_cprtime;

public:
    Aircraft(uint32_t icao, bool isImit = false);
    ~Aircraft() = default;
    /*!
     * \brief getICAO значение icao самолёта
     * \return
     */
    uint32_t getICAO() const { return getId() & 0xffffffff; }
    /*!
     * \brief incNumberMsg увеличение счётчика принятых сообщений
     * от самолёта
     */
    void incNumberMsg() { ++_messages; }
    /*!
     * \brief getNumberMsg текущее значение счётчика сообщений
     * \return
     */
    uint64_t getNumberMsg() const { return _messages; }
    /*!
     * \brief setFlightInfo имя рейса
     * \param val указатель на строку
     */
    void setFlightInfo(char* val);
    /*!
     * \brief getFlightInfo получение имени рейса
     * \return  строка
     */
    QString getFlightInfo() const;

    void setOddCprLat( int val ) { _odd_cprlat = val; }
    int  getOddCprLat() const { return _odd_cprlat; }

    void setOddCprLon( int val ) { _odd_cprlon = val; }
    int  getOddCprLon() const { return _odd_cprlon; }

    void setEvenCprLat( int val ) { _even_cprlat = val; }
    int  getEvenCprLat() const { return _even_cprlat; }

    void setEvenCprLon( int val ) { _even_cprlon = val; }
    int  getEvenCprLon() const { return _even_cprlon; }

    void setOddCprTime(int64_t val) { _odd_cprtime = val; }
    int64_t getOddCprTime() const { return _odd_cprtime; }

    void setEvenCprTime(int64_t val) { _even_cprtime = val; }
    int64_t getEvenCprTime() const { return _even_cprtime; }
    /*!
     * \brief setLongitude установка значения долготы
     * \param lon долгота
     */
    void setLongitude(double lon);
    /*!
     * \brief getLongitude получение значения долготы
     * \return долгота
     */
    double getLongitude() { return _geoCoord.longitude(); }
    /*!
     * \brief setLatitude установка широты
     * \param lat широта
     */
    void setLatitude(double lat);
    /*!
     * \brief getLatitude - значение широты
     * \return  широта
     */
    double getLatitude() { return _geoCoord.latitude(); }
    /*!
     * \brief toString сериализация данных в строку
     * \return
     */
    QString toString();

    /*!
     * \brief serialize сериализация объекта в байтовый массив
     * для передачи по сети
     * \return байтовый массив
     */
    QByteArray serialize();

    /*!
     * \brief unserialize ансериализация данных
     * \param array байтовый массив
     * \return результат работы
     */
    bool unserialize(QByteArray array);

    /*!
     * \brief serializedFrameSize размер блока данных
     * после сериализации
     * \return размер данных
     */
    static uint32_t  serializedFrameSize();
    /*!
     * \brief resetObjectData сброс значений на дефолт
     */
    void resetObjectData() override;
};

#endif // AIRCRAFT_H
