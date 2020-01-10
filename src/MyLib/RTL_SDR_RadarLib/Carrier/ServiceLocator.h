#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <QSharedPointer>

#include "interface/ICarrierClass.h"
#include "NullCarrier.h"
/*!
 * \brief The ServiceLocator class
 * Реализация паттерна поиска службы
 * Возвращает реальный объект или обхект заголушка
 */
class ServiceLocator
{
    static QSharedPointer<ICarrierClass> _carrier;
public:
    /*!
     * \brief getCarrier получение указателя на класс носителя
     * Гарантируется, что  указатель всегда не nullptr
     * \return
     */
    static QSharedPointer<ICarrierClass> getCarrier();
    /*!
     * \brief provide установка указателя на службу
     * \param carrier указатель на службу.
     * При передаче пустого указателя будет создан экземпляр
     * класса-заглушки
     */
    static void provide(QSharedPointer<ICarrierClass> carrier);
};


#endif // SERVICELOCATOR_H
