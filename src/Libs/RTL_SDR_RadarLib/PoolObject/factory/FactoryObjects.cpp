#include "PoolObject/factory/FactoryObjects.h"

#include <QDateTime>

#include "../shared_classes/objects/air/Aircraft.h"

FactoryObjects::FactoryObjects()
{

}

IObject *FactoryObjects::createObject(OBJECT_TYPE type,
                                     uint64_t id,
                                     QDateTime reg_time,
                                     bool isImit,
                                     Position geoPosition)
{
    if(type == OBJECT_TYPE::air)
        return  createAircraft( (id & 0xffffffff),
                                reg_time,
                                isImit,
                                geoPosition);
    return nullptr;
}


IObject *FactoryObjects::createAircraft(uint32_t icao,
                                        QDateTime reg_time,
                                        bool isImit,
                                        Position geoPosition)
{
    IObject* object  = new Aircraft(icao, isImit);
    object->setDateTimeStart(reg_time);
    object->setGeoCoord(geoPosition);
    return  object;
}
