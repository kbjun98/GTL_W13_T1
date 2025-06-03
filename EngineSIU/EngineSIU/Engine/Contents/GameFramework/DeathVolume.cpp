#include "DeathVolume.h"

#include "Components/BoxComponent.h"

void ADeathVolume::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    SetActorTickInEditor(false);

    BoxComponent = AddComponent<UBoxComponent>(TEXT("BoxComponent_0"));
    RootComponent = BoxComponent;
}
