#ifndef FACTORYOBJECTS_H
#define FACTORYOBJECTS_H

#include "interface/IObject.h"

class FactoryObjects
{
    IObject* createAircraft(uint32_t icao,
                            QDateTime reg_time,
                            bool isImit = false,
                            Position geoPosition = Position());
public:
    FactoryObjects();
    IObject* createObject(OBJECT_TYPE type,
                         uint64_t id,
                         QDateTime reg_time,
                         bool isImit = false,
                         Position geoPosition = Position());
};

#endif // FACTORYOBJECTS_H
