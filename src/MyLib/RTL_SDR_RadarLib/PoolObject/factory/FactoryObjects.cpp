#include "FactoryObjects.h"

#include <QDateTime>

#include "objects/air/Aircraft.h"

FactoryObjects::FactoryObjects()
{

}

IObject *FactoryObjects::createObject(OBJECT_TYPE type,
                                     uint64_t id,
                                     QDateTime reg_time,
                                     Position geoPosition)
{
    if(type == OBJECT_TYPE::air)
        return  createAircraft( (id & 0xffffffff),
                                reg_time,
                                geoPosition);
    return nullptr;
}


IObject *FactoryObjects::createAircraft(uint32_t icao,
                                        QDateTime reg_time,
                                        Position geoPosition)
{
    IObject* object  = new Aircraft(icao);
    object->setDateTimeStart(reg_time);
    object->setGeoCoord(geoPosition);
    return  object;
}
