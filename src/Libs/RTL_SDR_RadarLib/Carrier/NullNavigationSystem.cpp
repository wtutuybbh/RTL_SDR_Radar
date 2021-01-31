#include "NullNavigationSystem.h"
#include "osm/coordUtils/Position.h"

NullNavigationSystem::NullNavigationSystem()
{
    _position = Position(39.736583, 47.293666);
}

void NullNavigationSystem::setGeoCoord(const Position &pos)
{
    _position = pos;
}

Position NullNavigationSystem::getGeoCoord()
{
    return _position;
}

