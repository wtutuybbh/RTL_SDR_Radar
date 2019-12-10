#ifndef POOLOBJECT_H
#define POOLOBJECT_H

#include "poolobject_global.h"

#include <QSharedPointer>
#include <QDateTime>
#include <QMutex>

#include "interface/IPoolObject.h"
#include "factory/FactoryObjects.h"


class POOLOBJECTSHARED_EXPORT PoolObject : public IPoolObject
{
    pHash _container;
    int32_t _timeActualData = 12000;
    QMutex _mutex;
    FactoryObjects _factory;
public:
    explicit PoolObject();
    ~PoolObject() override;
    QSharedPointer<IObject> createNewObject(OBJECT_TYPE type,
                                            uint64_t id,
                                            QDateTime reg_time,
                                            Position geoPosition = Position()) override;

    QList<QSharedPointer<IObject> > values() override;


    bool isExistsObject(uint64_t id) override;
    QSharedPointer<IObject> getObjectByID(uint64_t id) override;

    void prepareAllObjectToDelete() override;
    void deleteMarkedObjects() override;
    void deleteObject(uint64_t id) override;

    void lockPool() override { _mutex.lock(); }
    bool tryLockPool() override { return _mutex.tryLock(5);}
    void unlockPool() override { _mutex.unlock(); }
};

#endif // POOLOBJECT_H
