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
    //GridMap.LoadMapFromFile("Engine/Contents/Resources/Map.txt");
    
    GridMap.InitializeGridNodeFromMeshes();
    GridMap.AnalyzeWalkableFromMeshes();
    DebugPrint();
    
    FGridNode& StartNode = GridMap.GetNode(0, 2);
    FGridNode& TargetNode = GridMap.GetNode(5, 2);

    PathFinder = FObjectFactory::ConstructObject<FPathFinder>(this);

    TArray<FGridNode*> Path = PathFinder->FindNodePathByNode(GridMap, StartNode, TargetNode);

    PathFinder->DebugPrint(Path);

    TArray<FVector> WorldPositionPath = PathFinder->FindWorlPosPathByWorldPos(GridMap, FVector(0, 0, 0), FVector(500, 0, 0));

    UE_LOG(ELogLevel::Warning, "=== WorldPositionPath ===");

    for (auto pos : WorldPositionPath) {
        UE_LOG(ELogLevel::Display, "x : %f, / y : %f / z : %f", pos.X, pos.Y, pos.Z);
    }
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
