#include "GridMapComponent.h"
#include "CoreUObject/UObject/ObjectFactory.h"
#include <Engine/Contents/Navigation/PathFinder.h>
#include "Navigation/MapData.h"
UGridMapComponent::UGridMapComponent()
{
    SetType(StaticClass()->GetName());

}

void UGridMapComponent::InitializeComponent()
{
    Super::InitializeComponent();
    
    GridMap.InitializeGridMap();
    DebugPrint();
    
    FGridNode& StartNode = GridMap.GetNode(0, 2);
    FGridNode& TargetNode = GridMap.GetNode(5, 2);

    PathFinder = new FPathFinder();

    TArray<FGridNode*> Path = PathFinder->FindNodePathByNode(GridMap, StartNode, TargetNode);

    PathFinder->DebugPrint(Path);

    TArray<FVector> WorldPositionPath = PathFinder->FindWorlPosPathByWorldPos(GridMap, FVector(0, 0, 0), FVector(500, 0, 0));

    
}

void UGridMapComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
}

void UGridMapComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
}

void UGridMapComponent::SetProperties(const TMap<FString, FString>& InProperties)
{
}

void UGridMapComponent::LoadMap(const std::string& FilePath)
{
    GridMap.LoadMapFromFile(FilePath);
}

void UGridMapComponent::DebugPrint() const
{
    GridMap.DebugPrint();
}

void UGridMapComponent::RenderDebugGrid()
{
}

int UGridMapComponent::GetWidth() const
{
    return GridMap.Width;
}

int UGridMapComponent::GetHeight() const
{
    return GridMap.Height;
}
