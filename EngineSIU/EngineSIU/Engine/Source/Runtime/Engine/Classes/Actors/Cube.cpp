#include "Cube.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/FObjLoader.h"

#include "GameFramework/Actor.h"

ACube::ACube()
{
    StaticMeshComponent->SetStaticMesh(FObjManager::GetStaticMesh(L"Contents/Primitives/CubePrimitive.obj"));

    
}

void ACube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
