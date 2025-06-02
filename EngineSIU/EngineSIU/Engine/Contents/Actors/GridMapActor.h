#pragma once
#include <Engine/Contents/Components/GridMapComponent.h>
#include "Classes/GameFrameWork/Actor.h"
#include "UObject/ObjectFactory.h"
#include "UObject/ObjectMacros.h"

class AGridMapActor : public AActor
{
    DECLARE_CLASS(AGridMapActor, AActor)
public:
    AGridMapActor();
    UPROPERTY(UGridMapComponent*, GridComponent, = nullptr);
};
