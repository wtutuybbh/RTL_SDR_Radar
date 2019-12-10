#include "PoolObject.h"


PoolObject::PoolObject():
    _container(pHash(new QHash<uint64_t,
                     QSharedPointer<IObject>> ()))
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


QSharedPointer<IObject> PoolObject::createNewObject(OBJECT_TYPE type,
                                                    uint64_t id,
                                                    QDateTime reg_time,
                                                    Position geoPosition)
{
    uint64_t t_id = 0;
    for(auto &iter:_container->values())
        if(!iter->getInUse())
        {
            t_id = _container->key(iter);
            break;
        }

    QSharedPointer<IObject> rto;
    if(t_id == 0)
    {
        if(type == OBJECT_TYPE::air)
            rto = QSharedPointer<IObject>(_factory.createObject(type,
                                                                id,
                                                                reg_time,
                                                                geoPosition));
    }
    else
    {
        rto = _container->take(t_id);
        rto->setId(id);
    }

    if(!rto.isNull())
        _container->insert(id,rto);

    return rto;
}

QList<QSharedPointer<IObject> > PoolObject::values()
{
    QList<QSharedPointer<IObject> > list;
    for(auto & obj : _container->values())
    {
        if(obj->getInUse())
            list.append(obj);
    }
    return list;
}

bool PoolObject::isExistsObject(uint64_t id)
{
    return _container->contains(id);
}

QSharedPointer<IObject> PoolObject::getObjectByID(uint64_t id)
{
    return _container->value(id);
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

