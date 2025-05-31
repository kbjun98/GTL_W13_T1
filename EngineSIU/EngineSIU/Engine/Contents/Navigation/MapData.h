#pragma once
#include <Container/Array.h>
#include <Math/Vector.h>

struct FGridNode
{
    int X;
    int Y;
    bool bWalkable = true;

    float GCost = 0.0f;         // 시작 노드부터의 거리
    float HCost = 0.0f;         // 휴리스틱(목표까지의 추정 거리)
    FGridNode* Parent = nullptr;

    float CalcCost() const { return GCost + HCost; }
};

class FGridMap
{
public:
    int Width = 0;
    int Height = 0;
    TArray<TArray<FGridNode>> GridNodes;

    // 데이터 로드
    void LoadMapFromFile(const FString FilePath);

    // 노드 가져오기 ( 인덱스->좌표 변환 처리 기능)
    FGridNode& GetNode(int X, int Y);

    // 월드 좌표 변환
    FVector GetWorldPosition(int X, int Y, float GridSize = 1.0f);

    void DebugPrint() const;
};


