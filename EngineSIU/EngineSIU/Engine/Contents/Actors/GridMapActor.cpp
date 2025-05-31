#include "GridMapActor.h"

AGridMapActor::AGridMapActor()
{
    GridComponent = AddComponent<UGridMapComponent>("GrimdMapComponent_0");
    RootComponent = GridComponent;

}
