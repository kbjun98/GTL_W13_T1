#include "TestVolume.h"

#include "Components/BoxComponent.h"

void ATestVolume::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    BoxComponent = AddComponent<UBoxComponent>(TEXT("BoxComponent_0"));
    RootComponent = BoxComponent;
}
