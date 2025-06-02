#include "PlayerStart.h"

#include "Components/BillboardComponent.h"

void APlayerStart::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();

    UBillboardComponent* BillboardComponent = AddComponent<UBillboardComponent>("BillboardComponent_0");
    BillboardComponent->SetTexture(L"Assets/Editor/Icon/S_Player.PNG");
    BillboardComponent->bIsEditorBillboard = true;
}

UObject* APlayerStart::Duplicate(UObject* InOuter)
{
    return Super::Duplicate(InOuter);
}
