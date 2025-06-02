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
    FPathFinder();
    ~FPathFinder() = default;

    // Node 로 Node 경로 찾기
    TArray<FGridNode*> FindNodePathByNode(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode);
    
    // Node 로 WorldPosition 경로 찾기
    TArray<FVector> FindWorldPosPathByNodePath(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode);

    // WorldPosition으로 WorldPosition 경로 찾기
    TArray<FVector> FindWorlPosPathByWorldPos(FGridMap& GridMap, const FVector& StartWorldPos, const FVector& TargetWorldPos);
    const void DebugPrint(TArray<FGridNode*>& Path) const;
private:
    // 휴리스틱 계산
    float CalcHeuristic(FGridNode& A, FGridNode& B);

    // 인접 노드 반환(상하좌우, 대각선 포함)
    TArray<FGridNode*> GetNeighbors(FGridMap& GridMap, const FGridNode& Node);

    
};
