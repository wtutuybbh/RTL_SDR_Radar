#ifndef ICONTAINEROBJECT
#define ICONTAINEROBJECT

#include <QVector>
#include <QSharedPointer>

#include "IObject.h"

typedef  QSharedPointer< QHash<uint64_t, QSharedPointer<IObject> > > pHash;
class Position;

class IPoolObject
{

public:
    virtual ~IPoolObject(){}
    /*!
     * \brief createNewObject функция создания нового объекта в пуле
     * \param id - уникальный id
     * \param reg_time - время регистрации
     * \param geoPosition - геокоординаты если есть
     * \param isImit - создание ими
     * \return указатель на новый объект
     */
    virtual QSharedPointer<IObject> createNewObject(uint64_t id,
                                                    QDateTime reg_time,
                                                    Position geoPosition,
                                                    bool isImit) = 0;
    /*!
     * \brief values получение списка указателей на существующие объекты.
     * Объекты которые помечены, как неактулаьные выведены не будут.
     * \return список умных указателей на объекты
     */
    virtual QList<QSharedPointer<IObject>> values() = 0;

    /*!
     * \brief values получение списка указателей на все объекты
     * в пуле. Будут выведены все актуальные + объекты,помеченные удалёнными
     * \return список умных указателей на объекты
     */
    virtual QList<QSharedPointer<IObject>> allValues() = 0;

    /*!
     * \brief getObjectsCount -возвращает количество объектов,
     * которые находятся в актуальном состоянии (используются)
     * \return
     */
    virtual int getObjectsCount() = 0;

    /*!
     * \brief isExistsObject проверка существования объякта в пуле
     * \param id - уникальный id для поиска
     * \return true - если в пуле объект есть и он в актуально состоянии
     * false -объекта не существует или он был удален
     */
    virtual bool isExistsObject(uint64_t id) = 0;

    /*!
     * \brief getObjectByID получение указателя на объект по id
     * \param id - уникальный идентификатор для поиска
     * \return  указатель на объект или nullptr если его нет
     */
    virtual QSharedPointer<IObject> getObjectByID(uint64_t id) = 0;
    /*!
     * \brief prepareAllObjectToDelete пометить все объекты в пуле
     * флагом - готов для удаления.
     * При обновлении параметров данный флаг сбрасывается
     */
    virtual void prepareAllObjectToDelete() = 0;
    /*!
     * \brief deleteMarkedObjects - удалит объекты, у которых
     * присутствует флаг DELETE_OBJECT
     */
    virtual void deleteMarkedObjects() = 0;
    /*!
     * \brief deleteObject -  удалить объект
     * по уникальному идентификатору
     * \param id - идентификатор объекта
     */
    virtual void deleteObject(uint64_t id) = 0;
    /*!
     * \brief lockPool блокировка пула объектов
     * для внесения изменния параметров объектов
     */
    virtual void lockPool() = 0;
    /*!
     * \brief tryLockPool - попробовать заблокировать пул потоков.
     * Данный вызов применяется при работе с пулом из основного потока.
     * \return true - блокировка установлена, false - блокировка не возможна
     */
    virtual bool tryLockPool() = 0;
    /*!
     * \brief unlockPool - снять блокировку с пула объектов
     */
    virtual void unlockPool() = 0;

};
#endif // ICONTAINEROBJECT

