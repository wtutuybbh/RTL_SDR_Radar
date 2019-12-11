#include "PoolObject.h"


PoolObject::PoolObject(OBJECT_TYPE type):
    _container(pHash(new QHash<uint64_t,
                     QSharedPointer<IObject>> ())),
    _type(type)
{
    qDebug()<<"PoolObject() -> create";
}

PoolObject::~PoolObject()
{
    qDebug()<<"~PoolObject() -> start delete QHash<id,IObject>";
    for(auto & iter: _container->values())
        iter.clear();
    _container->clear();
    _container.clear();
    qDebug()<<"~PoolObject() -> delete QHash<id,IObject>";
}


QSharedPointer<IObject> PoolObject::createNewObject(uint64_t id,
                                                    QDateTime reg_time,
                                                    Position geoPosition)
{
    if(id == 0)
        return  QSharedPointer<IObject>();

    uint64_t t_id = 0;
    QSharedPointer<IObject> object = {nullptr};
    if(!_container->contains(id))
    {
        for(auto &iter:_container->values())
        {
            if(!iter->getInUse())
            {
                t_id = _container->key(iter);
                object = _container->take(t_id);
                break;
            }
        }
    }
    else
        object = _container->value(id);

    if(object.isNull())
    {
            object = QSharedPointer<IObject>(_factory.createObject(_type,
                                                                id,
                                                                reg_time,
                                                                geoPosition));
    }
    else
    {
        if(object->getInUse())
            object->resetObjectData();

        object->setId(id);
        object->setObjectState(OBJECT_STATE::NEW_OBJECT);
        object->setDateTimeStart(reg_time);
        object->setDateTimeStop(reg_time);
        object->setGeoCoord(geoPosition);
    }

    if(!object.isNull())
        _container->insert(id,object);

   return object;
}

QList<QSharedPointer<IObject> > PoolObject::values()
{
    QList<QSharedPointer<IObject> > list;
    for(auto & obj : _container->values())
    {
        if(obj->getInUse() && (obj->getId() != 0))
            list.append(obj);
    }
    return list;
}

QList<QSharedPointer<IObject> > PoolObject::allValues()
{
    return _container->values();
}

int PoolObject::getObjectsCount()
{
    int counter = 0;
    for(auto & obj : _container->values())
    {
        if(obj->getInUse() && (obj->getId() != 0))
           ++counter;
    }
    return counter;
}

bool PoolObject::isExistsObject(uint64_t id)
{
    return (_container->contains(id) &&_container->value(id)->getInUse());
}

QSharedPointer<IObject> PoolObject::getObjectByID(uint64_t id)
{
    if(_container->contains(id) && _container->value(id)->getInUse())
        return _container->value(id);
    else
      return QSharedPointer<IObject>();
}


void PoolObject::prepareAllObjectToDelete()
{
    std::for_each(_container->begin(),
                  _container->end(),
                  [](QSharedPointer<IObject> iter)
    { 
        iter->setObjectState(OBJECT_STATE::DELETE_OBJECT);
    });
}

void PoolObject::deleteMarkedObjects()
{

    std::for_each(_container->begin(),
                  _container->end(),
                  [](QSharedPointer<IObject> iter)
    {
        if(iter->getObjectState() == OBJECT_STATE::DELETE_OBJECT)
            iter->resetObjectData();
    });
}

void PoolObject::deleteObject(uint64_t id)
{
    if(_container->contains(id))
        _container->value(id)->resetObjectData();
}

