#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <stdint.h>
#include <QString>
#include <math.h>
#include "objects/base/BaseObject.h"

class Aircraft : public BaseObject
{
    static constexpr char SIZE_TEXT = 9;
    const uint32_t VALUE_LSB = 100;
    const double LON_VALUE_LSB = 360.0 / pow(2, 31);
    const double LAT_VALUE_LSB = 180.0 / pow(2, 31);

    //uint32_t _icao;      /* ICAO address */
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
    Aircraft(uint32_t icao);
    ~Aircraft() = default;

    uint32_t getICAO() const { return getId() & 0xffffffff; }

    void incNumberMsg() { ++_messages; }
    uint64_t getNumberMsg() const { return _messages; }

    void setFlightInfo(char* val);
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

    void setLongitude(double lon);
    double getLongitude() { return _geoCoord.longitude(); }

    void setLatitude(double lat);
    double getLatitude() { return _geoCoord.latitude(); }

    QString toString();

    QByteArray serialize();

    bool unserialize(QByteArray array);

    static uint32_t  serializedFrameSize();
};

#endif // AIRCRAFT_H
