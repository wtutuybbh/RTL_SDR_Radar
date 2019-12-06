#ifndef ISUBJECT_H
#define ISUBJECT_H

#include <QSharedPointer>

class IObserver;
class IPoolObject;

class ISubject
{
public:
    virtual ~ISubject() {}

    virtual void Attach(QSharedPointer<IObserver> o) = 0;
    virtual void Deatach(QSharedPointer<IObserver> o) = 0;
    virtual void Attach(IObserver* o) = 0;
    virtual void Deatach(IObserver* o) = 0;
    virtual void Notify(QSharedPointer<IPoolObject> pool) = 0;
};

#endif // ISUBJECT_H
