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
    QMutex _mutex;
public:
    virtual ~Subject();

    void Attach(QSharedPointer<IObserver> o) override;
    void Deatach(QSharedPointer<IObserver> o) override;
    void Notify(QSharedPointer<IPoolObject> pool) override;
protected:
    Subject();

private:
    QVector<QSharedPointer<IObserver>> _observers;
};

#endif // SUBJECT_H
