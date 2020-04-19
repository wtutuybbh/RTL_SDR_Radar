#ifndef IOBSERVER
#define IOBSERVER
#include <QSharedPointer>
#include <stdint.h>

class Subject;
class IPoolObject;
class ISubject;

class  IObserver
{
public:
    virtual ~IObserver(){}
    virtual void subscribe(QSharedPointer<ISubject> subject ) = 0;
    virtual void update(QSharedPointer<IPoolObject> pool ) = 0;
    virtual void unsubscribe(QSharedPointer<ISubject> subject) = 0;
    u_int64_t getObserverId() { return u_int64_t(this); }
protected:
    IObserver(){}
};

#endif // IOBSERVER
