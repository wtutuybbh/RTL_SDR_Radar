#include "PoolObjectsTest.h"

#include "PoolObject/PoolObject.h"
#include "osm/coordUtils/Position.h"

PoolObjectsTestTest::PoolObjectsTestTest()
{
    _pool = new PoolObject(OBJECT_TYPE::air);
}

PoolObjectsTestTest::~PoolObjectsTestTest()
{
    delete _pool;
}

void PoolObjectsTestTest::initTestCase()
{
    for(uint32_t i = 0; i < 10; i++)
        vectorId.append(i);
}

void PoolObjectsTestTest::addZeroIdTest()
{
    QSharedPointer<IObject> _ptrObject = _pool->createNewObject(0,
                                                                QDateTime::currentDateTime(),
                                                                Position(),
                                                                false);
    QVERIFY(_ptrObject.isNull() == true);
    QVERIFY(_pool->getObjectsCount() == 0);
}

void PoolObjectsTestTest::addEqualIdTest()
{
    QSharedPointer<IObject> ptrObject_1 = _pool->createNewObject(1,
                                                                 QDateTime::currentDateTime(),
                                                                 Position(),
                                                                 false);

    QVERIFY(ptrObject_1.isNull() != true);
    QVERIFY(_pool->getObjectsCount() == 1);
    QCOMPARE(ptrObject_1->getInUse(),true);
    QCOMPARE(ptrObject_1->getObjectState(),OBJECT_STATE::NEW_OBJECT);

    QSharedPointer<IObject> ptrObject_2 = _pool->createNewObject(1,
                                                                 QDateTime::currentDateTime(),
                                                                 Position(),
                                                                 false);

    QVERIFY(ptrObject_2.isNull() != true);
    QCOMPARE(ptrObject_2->getInUse(),true);
    QCOMPARE(ptrObject_2->getObjectState(),OBJECT_STATE::NEW_OBJECT);

    QVERIFY(_pool->getObjectsCount() == 1);

    QVERIFY(ptrObject_1.data() ==ptrObject_2.data());
}

void PoolObjectsTestTest::addObjectsToPoolTest()
{
    int size = vectorId.size();

    for (int i = 0; i < size; ++i)
    {
        QSharedPointer<IObject> _ptrObject = _pool->createNewObject(vectorId.at(i)+1,
                                                                    QDateTime::currentDateTime().addSecs(i),
                                                                    Position(),
                                                                    false);
        QVERIFY(_ptrObject.isNull() != true);
        QCOMPARE(_ptrObject->getInUse(),true);
        QCOMPARE(_ptrObject->getObjectState(),OBJECT_STATE::NEW_OBJECT);
    }
}

void PoolObjectsTestTest::getSizePoolTest()
{
    QCOMPARE(_pool->getObjectsCount(),vectorId.size());
}

void PoolObjectsTestTest::deleteObjectFromPoolTest()
{
    QCOMPARE(_pool->isExistsObject(2),true);
    _pool->deleteObject(2);
    QCOMPARE(_pool->isExistsObject(2),false);
    QCOMPARE(_pool->getObjectsCount(),vectorId.size()-1);
}

void PoolObjectsTestTest::deleteAllObjectsFromPoolTest()
{
    _pool->prepareAllObjectToDelete();
    _pool->deleteMarkedObjects();
    QCOMPARE(_pool->getObjectsCount(),0);
    QVERIFY(_pool->allValues().count() != 0);

}

void PoolObjectsTestTest::deleteAddDeletObjectTest()
{
    uint64_t id = 1;
    QSharedPointer<IObject> ptrObject = _pool->createNewObject(id,
                                                               QDateTime::currentDateTime(),
                                                               Position(),
                                                               false);

    QVERIFY(ptrObject.isNull() != true);
    QVERIFY(_pool->getObjectsCount() == 1);
    QCOMPARE(ptrObject->getInUse(),true);
    QCOMPARE(ptrObject->getObjectState(),OBJECT_STATE::NEW_OBJECT);

    _pool->deleteObject(id);

    QCOMPARE(_pool->isExistsObject(id),false);
    QVERIFY(_pool->allValues().count() != 0);

    QCOMPARE(ptrObject->getInUse(),false);
    QCOMPARE(ptrObject->getObjectState(),OBJECT_STATE::DELETE_OBJECT);
    QVERIFY(ptrObject->getId() != id);

    ptrObject = _pool->createNewObject(id,
                                       QDateTime::currentDateTime(),
                                       Position(),
                                       false);

    QVERIFY(ptrObject.isNull() != true);
    QVERIFY(_pool->getObjectsCount() == 1);
    QCOMPARE(ptrObject->getInUse(),true);
    QCOMPARE(ptrObject->getObjectState(),OBJECT_STATE::NEW_OBJECT);

}

void PoolObjectsTestTest::updateObjectTest()
{
    int size = vectorId.size();
    for(uint32_t i = 1; i < size;++i )
    {
        if(_pool->isExistsObject(i))
        {
            QSharedPointer<IObject> ptrObject = _pool->getObjectByID(i);
            QVERIFY(ptrObject.isNull() != true);
            QCOMPARE(ptrObject->getId(),uint64_t(i));
        }
        else
        {
            QVERIFY(_pool->createNewObject(i,
                                           QDateTime::currentDateTime(),
                                           Position(),
                                           false).isNull() != true);
            QCOMPARE(_pool->isExistsObject(i),true);
        }
    }

    for(uint32_t i = 1; i < size;++i )
    {
        QSharedPointer<IObject> ptrObject = _pool->getObjectByID(i);
        QVERIFY(ptrObject.isNull() != true);

        ptrObject->setAzimuth(i);
        ptrObject->setObjectState(OBJECT_STATE::UPDATE_OBJECT);

        QCOMPARE(ptrObject->getAzimuth(),double(i));
        QCOMPARE(ptrObject->getObjectState(),OBJECT_STATE::UPDATE_OBJECT);
    }

}


QTEST_APPLESS_MAIN(PoolObjectsTestTest)

