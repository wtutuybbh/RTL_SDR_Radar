#include <QDebug>

#include "Carrier/NullCarrier.h"
#include "NullNavigationSystem.h"
#include "MapGraphics/coordUtils/Position.h"

NullCarrier::NullCarrier()
{

    _ptrNavigation = QSharedPointer<INavigationSystem>(new NullNavigationSystem());
    qDebug()<<"NullCarrier create";
}

NullCarrier::~NullCarrier()
{
    qDebug()<<"NullCarrier delete";
    _ptrNavigation.clear();
}

Position NullCarrier::getGeoCoord()
{
    return _ptrNavigation->getGeoCoord();
}
