#ifndef SUBJECT_H
#define SUBJECT_H

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>

#include "interface/IObserver.h"
#include "interface/ISubject.h"

class Subject: public ISubject
{
    QMutex _mutexSmartPtr;
    QMutex _mutexRawPtr;
public:
    Subject();
    virtual ~Subject() override;

    void Attach(QSharedPointer<IObserver> o) override;
    void Deatach(QSharedPointer<IObserver> o) override;

    void Attach(IObserver* o) override;
    void Deatach(IObserver* o) override;

    void Notify(QSharedPointer<IPoolObject> pool) override;


private:
    QVector<QSharedPointer<IObserver>> _observers;
    QVector<IObserver*> _rawObservers;
};

#endif // SUBJECT_H
