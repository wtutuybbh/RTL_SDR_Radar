#ifndef FACTORYOBJECTS_H
#define FACTORYOBJECTS_H

#include "interface/IObject.h"

class Position;

class FactoryObjects
{
    IObject* createAircraft(uint32_t icao,
                            QDateTime reg_time,
                            bool isImit,
                            Position geoPosition);
public:
    FactoryObjects();
    IObject* createObject(OBJECT_TYPE type,
                         uint64_t id,
                         QDateTime reg_time,
                         bool isImit,
                         Position geoPosition);
};

#endif // FACTORYOBJECTS_H
