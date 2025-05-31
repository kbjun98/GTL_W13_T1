#include "GridMapComponent.h"
#include "CoreUObject/UObject/ObjectFactory.h"
#include <Engine/Contents/Navigation/PathFinder.h>

UGridMapComponent::UGridMapComponent()
{
    SetType(StaticClass()->GetName());
}

void UGridMapComponent::InitializeComponent()
{
    Super::InitializeComponent();
    GridMap.LoadMapFromFile("Engine/Contents/Resources/Map.txt");
    DebugPrint();

    FGridNode& StartNode = GetNode(5, 1);
    FGridNode& TargetNode = GetNode(0, 0);

    PathFinder = FObjectFactory::ConstructObject<FPathFinder>(this);

    TArray<FGridNode*> Path = PathFinder->FindPath(GridMap, StartNode, TargetNode);

    PathFinder->DebugPrint(Path);
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

FGridNode& UGridMapComponent::GetNode(int X, int Y)
{
    return GridMap.GridNodes[Y][X];
}
