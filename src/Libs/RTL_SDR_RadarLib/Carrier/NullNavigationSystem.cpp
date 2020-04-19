#include "NullNavigationSystem.h"
#include "../shared_classes/coord/Position.h"

NullNavigationSystem::NullNavigationSystem()
{

}

Position NullNavigationSystem::getGeoCoord()
{
    return Position(39.736583, 47.293666);
}

