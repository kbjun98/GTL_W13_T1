#include "SuccessVolume.h"

#include "Components/BoxComponent.h"

void ASuccessVolume::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    SetActorTickInEditor(false);

    BoxComponent = AddComponent<UBoxComponent>(TEXT("BoxComponent_0"));
    RootComponent = BoxComponent;
}
