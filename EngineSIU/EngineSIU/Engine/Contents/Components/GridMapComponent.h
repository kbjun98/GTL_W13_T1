#pragma once
#include "Components/PrimitiveComponent.h"
#include <Engine/Contents/Navigation/MapData.h>


class FPathFinder;
class UGridMapComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UGridMapComponent, UPrimitiveComponent)
public:
    UGridMapComponent();
    ~UGridMapComponent() = default;
    
    FGridMap GridMap;

    FPathFinder* PathFinder;
public:
    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

    void LoadMap(const std::string& FilePath);
    
    void DebugPrint() const;
    void RenderDebugGrid();

    int GetWidth() const;
    int GetHeight() const;

    FGridNode& GetNode(int X, int Y);
};
