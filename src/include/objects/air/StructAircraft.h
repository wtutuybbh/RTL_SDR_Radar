#ifndef STRUCTAIRCRAFT_H
#define STRUCTAIRCRAFT_H

#include <stdint.h>

constexpr char SIZE_TEXT = 9;

/*!
 * @brief  Характеристики объекта.
 */
#pragma pack(push,1)
struct StructAircraft
{
    /* ICAO address */
    uint32_t icao;
    /* Flight number */
    char flight[SIZE_TEXT];
    /* Altitude */
    uint32_t altitude;
    /* Velocity computed from EW and NS components. */
    uint32_t speed;
    /* Angle of flight. */
    uint32_t course;
    /* latitude */
    int32_t lat;
    /* longitude */
    int32_t lon;
    /* Time at which the last packet was received. */
    int64_t seen;
    /* Number of Mode S messages received. */
    uint32_t messages;
};
#pragma pack(pop)

#endif // STRUCTAIRCRAFT_H
