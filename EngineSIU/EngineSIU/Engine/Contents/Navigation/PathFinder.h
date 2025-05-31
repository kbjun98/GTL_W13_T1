#pragma once
#include "MapData.h"
#include "UObject/Object.h"
#include "CoreUObject/UObject/ObjectMacros.h"
/*
*   A* 경로 탐색 클래스
*/
class FPathFinder : public UObject
{
    DECLARE_CLASS(FPathFinder, UObject)

public:
    FPathFinder() = default;
    ~FPathFinder() = default;
    // 실제 경로 탐색
    TArray<FGridNode*> FindPath(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode);

    const void DebugPrint(TArray<FGridNode*>& Path) const;
private:
    // 휴리스틱 계산
    float CalcHeuristic(FGridNode& A, FGridNode& B);

    // 인접 노드 반환(상하좌우, 대각선 포함)
    TArray<FGridNode*> GetNeighbors(FGridMap& GridMap, const FGridNode& Node);

    
};
