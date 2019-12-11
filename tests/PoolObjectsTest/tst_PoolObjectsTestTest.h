#ifndef TST_POOLOBJECTSTESTTEST_H
#define TST_POOLOBJECTSTESTTEST_H

#include <QString>
#include <QtTest>
#include <QDebug>
#include <QObject>

#include "interface/IPoolObject.h"
class PoolObjectsTestTest : public QObject
{
    Q_OBJECT
    IPoolObject* _pool;
    QVector<uint32_t> vectorId;
public:
    PoolObjectsTestTest();
    ~PoolObjectsTestTest();
private Q_SLOTS:
    void initTestCase();
    void addZeroIdTest();
    void addEqualIdTest();
    void addObjectsToPoolTest();
    void getSizePoolTest();
    void deleteObjectFromPoolTest();
    void deleteAllObjectsFromPoolTest();
    void deleteAddDeletObjectTest();
    void updateObjectTest();
};


#endif // TST_POOLOBJECTSTESTTEST_H
