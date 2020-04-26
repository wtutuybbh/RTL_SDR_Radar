#include "Subject.h"

Subject::Subject()
{
}


Subject::~Subject()
{
    qDebug()<<"~Subject" << _observers.size();
    _observers.clear();
}

void Subject::Attach(QSharedPointer<IObserver> o)
{
    QMutexLocker _lock(&_mutexSmartPtr);
    if(o.isNull())
    {
        qDebug()<<"Subject::Attach() -> IObserver nullptr ";
        return;
    }

    for(auto & iter: _observers)
    {
        if(iter->getObserverId() == o->getObserverId())
        {
            qDebug()<<"this subscribe in vector";
            return;
        }
    }
    //Add new subscriber to new subscriber list
    _observers.push_back(o);
    qDebug()<<"[Attach] : number of observers ="<< _observers.size();
}

void Subject::Deatach(QSharedPointer<IObserver> o)
{
    //Enter to locked section
    QMutexLocker _lock(&_mutexSmartPtr);
    if(_observers.isEmpty())
    {
        qDebug()<<"no subscribers";
        return;
    }

    _observers.remove(_observers.indexOf(o));
    qDebug()<<"[Deatach] : number of observers =" << _observers.size();
}

void Subject::Attach(IObserver *o)
{
    QMutexLocker _lock(&_mutexRawPtr);
    if( o == nullptr )
    {
        qDebug()<<"Subject::Attach() -> IObserver nullptr ";
        return;
    }

    for(auto & iter: _rawObservers)
    {
        if(iter->getObserverId() == o->getObserverId())
        {
            qDebug()<<"this subscribe in vector";
            return;
        }
    }
    //Add new subscriber to new subscriber list
    _rawObservers.push_back(o);
    qDebug()<<"[Attach] : number of observers ="<< _rawObservers.size();
}

void Subject::Deatach(IObserver *o)
{
    //Enter to locked section
    QMutexLocker _lock(&_mutexRawPtr);
    if(_rawObservers.isEmpty() || o == nullptr)
    {
        qDebug()<<"no subscribers";
        return;
    }

    _rawObservers.remove(_rawObservers.indexOf(o));
    qDebug()<<"[Deatach] : number of observers =" << _rawObservers.size();
}

void Subject::Notify(QSharedPointer<IPoolObject> pool)
{
    if(pool.isNull())
    {
        qDebug()<<"[Notify()] :  pool == nullptr";
        return;
    }

    QMutexLocker lock(&_mutexSmartPtr);

    for(auto & iter: _observers)
        iter->update(pool);
    lock.unlock();

    QMutexLocker lockRaw(&_mutexRawPtr);

    for(auto & iter: _rawObservers)
        iter->update(pool);
}
