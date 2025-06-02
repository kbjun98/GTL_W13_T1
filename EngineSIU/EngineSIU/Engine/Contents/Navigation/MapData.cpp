#include "MapData.h"
#include <fstream>
#include <UserInterface/Console.h>
#include <sstream>

void FGridMap::LoadMapFromFile(const FString FilePath)
{
    std::ifstream File(FilePath.ToAnsiString());

    if (!File.is_open())
    {
        UE_LOG(ELogLevel::Warning, "Cannot open MapFile");
        return;
    }

    std::string Line;
    while (std::getline(File, Line)) {
        std::istringstream Stream(Line);
        TArray<FGridNode> Row;
        int Value;
        int X = 0;


        while (Stream >> Value)
        {
            FGridNode  NewNode;
            NewNode.X = X;
            NewNode.Y = GridNodes.Num();
            NewNode.bWalkable = Value == 1;
            Row.Add(NewNode);
            ++X;
        }
        GridNodes.Add(Row);
    }
    Height = GridNodes.Num();
    if (GridNodes.IsEmpty()) {
        Width = 0;
    }
    else {
        Width = GridNodes[0].Num();
    }

    File.close();
}

FGridNode& FGridMap::GetNode(int X, int Y)
{
    return GridNodes[Y][X];
}

FVector FGridMap::GetWorldPosition(int X, int Y, float GridSize)
{
    return FVector(X * GridSize, Y*GridSize, 0.0f);
}

void FGridMap::DebugPrint() const
{
    UE_LOG(ELogLevel::Display, "맵 디버그 출력 (Width = %d, Height = %d): ", Width, Height);
    for (int y = 0; y < Height; ++y)
    {
        FString log;
        for (int x = 0; x < Width; ++x)
        {
            const FGridNode& Node = GridNodes[y][x];
            if (Node.bWalkable) {
                log = log + "O ";
            }
            else {
                log = log + "X ";
                
            }
        }
        UE_LOG(ELogLevel::Display, "%s", *log);
    }
}




