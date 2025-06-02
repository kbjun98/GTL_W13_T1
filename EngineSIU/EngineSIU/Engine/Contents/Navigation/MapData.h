#pragma once
#include <Container/Array.h>
#include <Math/Vector.h>
#include "Core/Container/Map.h"


struct FGridNode
{
    int X= 0 ;
    int Y = 0;
    bool bWalkable = true;

    float GCost = 0.0f;         // 시작 노드부터의 거리
    float HCost = 0.0f;         // 휴리스틱(목표까지의 추정 거리)
    FGridNode* Parent = nullptr;

    FVector WorldPosition = FVector::ZeroVector;

    float CalcCost() const { return GCost + HCost; }
    FVector GetPosition(){ return WorldPosition; }
};

struct FIntPoint
{
    /** X 좌표 */
    int32 X = 0;

    /** Y 좌표 */
    int32 Y = 0;

    /** 생성자 */
    FIntPoint() = default;

    FIntPoint(int32 InX, int32 InY)
        : X(InX), Y(InY)
    {
    }

    /** 비교 연산자 (정렬, TMap 키로 사용) */
    bool operator==(const FIntPoint& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    /** 디버그용 출력 문자열 */
    FString ToString() const
    {
        return FString::Printf(TEXT("(%d, %d)"), X, Y);
    }
};

class FGridMap
{
public:
    int Width = 0;
    int Height = 0;
    FVector MinPoint = FVector::ZeroVector;
    FVector MaxPoint = FVector::ZeroVector;

    TMap<int32, FGridNode> GridNodes;

    // FIXME : 리팩터링 필요.
    int32 Get1DIndex(int X, int Y) const
    {
        return Y * Width + X;
    }

    // GridMap 초기화 시 호출
    void InitializeGridMap();

    // 노드 가져오기 ( 인덱스->좌표 변환 처리 기능)
    FGridNode& GetNode(int X, int Y);

    void DebugPrint() const;
    
    void InitializeGridNodeFromMeshes();

    void AnalyzeWalkableFromMeshes();

    // === 레거시 코드들 ===
    void LoadMapFromFile(const FString FilePath);
};






