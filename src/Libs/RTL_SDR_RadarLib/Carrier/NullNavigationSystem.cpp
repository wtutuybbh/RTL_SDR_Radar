#include "NullNavigationSystem.h"
#include "osm/coordUtils/Position.h"

NullNavigationSystem::NullNavigationSystem()
{

}

Position NullNavigationSystem::getGeoCoord()
{
    return Position(39.736583, 47.293666);
}

