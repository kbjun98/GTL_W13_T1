#include "TestVolume.h"

#include "Components/BoxComponent.h"

void ATestVolume::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    SetActorTickInEditor(false);

    BoxComponent = AddComponent<UBoxComponent>(TEXT("BoxComponent_0"));
    RootComponent = BoxComponent;
}
