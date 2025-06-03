#include "PathFinder.h"
#include <Container/Set.h>

TArray<FGridNode*> FPathFinder::FindNodePathByNode(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode)
{
    for (auto& Elem : GridMap.GridNodes)
    {
        Elem.Value.Parent = nullptr;
        Elem.Value.GCost = FLT_MAX;
        Elem.Value.HCost = 0.0f;
    }
    StartNode.GCost = 0.0f;

    TArray<FGridNode*> OpenSet;
    TSet<FGridNode*> ClosedSet;

    OpenSet.Add(&StartNode);

    while (!OpenSet.IsEmpty()) {
        // FCost 가장 낮은 노드 선택.
        auto CurrentIt = std::min_element(OpenSet.begin(), OpenSet.end(),
            [](FGridNode* A, FGridNode* B) { return A->CalcCost() < B->CalcCost(); });

        FGridNode* CurrentNode = *CurrentIt;

        // 목표 지점 도착 시
        if (CurrentNode->X == TargetNode.X && CurrentNode->Y == TargetNode.Y)
        {
            // 무한루프 방지용 방문 노드 세트
            TSet<FGridNode*> VisitedNodes;


            // 경로 재구성
            TArray<FGridNode*> Path;
            while (CurrentNode)
            {
                // 무한루프 방지: 자기 자신을 Parent로 가진 경우
                if (VisitedNodes.Contains(CurrentNode))
                {
                    UE_LOG(ELogLevel::Error, TEXT("FindNodePathByNode: 무한루프 감지! Node=(%d,%d)"), CurrentNode->X, CurrentNode->Y);
                    break;
                }
                VisitedNodes.Add(CurrentNode);

                // Parent가 자기 자신 가리키면 강제 종료
                if (CurrentNode->Parent == CurrentNode)
                {
                    UE_LOG(ELogLevel::Error, TEXT("FindNodePathByNode: Parent가 자기 자신을 가리킴! Node=(%d,%d)"), CurrentNode->X, CurrentNode->Y);
                    break;
                }

                Path.Add(CurrentNode);
                CurrentNode = CurrentNode->Parent;
            }

            std::reverse(Path.begin(), Path.end());
            return Path;
        }
        OpenSet.Remove(*CurrentIt);
        ClosedSet.Add(CurrentNode);

        // 인접 노드 순회(상하좌우,대각선)
        for (auto Neighbor : GetNeighbors(GridMap, *CurrentNode))
        {
            if (!Neighbor->bWalkable || ClosedSet.Contains(Neighbor)) {
                continue;
            }

            float NewGCost = CurrentNode->GCost + 1.0f;
            if (NewGCost < Neighbor->GCost || !std::count(OpenSet.begin(), OpenSet.end(), Neighbor))
            {
                Neighbor->GCost = NewGCost;
                Neighbor->HCost = CalcHeuristic(*Neighbor, TargetNode);
                Neighbor->Parent = CurrentNode;

                if (!std::count(OpenSet.begin(), OpenSet.end(), Neighbor))
                {
                    OpenSet.Add(Neighbor);
                }
            }
        }
    }

    return {};
}

float FPathFinder::CalcHeuristic( FGridNode& A, FGridNode& B)
{
    // 맨해튼 거리 사용
    return static_cast<float>(abs(A.X - B.X) + abs(A.Y - B.Y));
}

TArray<FGridNode*> FPathFinder::GetNeighbors(FGridMap& GridMap, const FGridNode& Node)
{
    TArray<FGridNode*> Neighbors;

    const int DX[4] = { -1, 1, 0, 0 };
    const int DY[4] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i)
    {
        int NX = Node.X + DX[i];
        int NY = Node.Y + DY[i];

        if (NX >= 0 && NX < GridMap.Width && NY >= 0 && NY < GridMap.Height)
        {
            Neighbors.Add(&GridMap.GetNode(NX, NY));
        }
    }
    return Neighbors;
}

TArray<FVector> FPathFinder::FindWorldPosPathByNode(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode)
{
    TArray<FGridNode*> PathNodes = FindNodePathByNode(GridMap, StartNode, TargetNode);
    TArray<FVector> WorldPaths;
    for (FGridNode* Node : PathNodes)
    {
        FVector WorldPos = Node->GetPosition(); // GridSize를 100.0f로 가정
        WorldPaths.Add(WorldPos);
    }

    DebugWorldPosPath(WorldPaths);

    return WorldPaths;
}

TArray<FVector> FPathFinder::FindWorlPosPathByWorldPos(FGridMap& GridMap, const FVector& StartWorldPos, const FVector& TargetWorldPos)
{

    int StartX = FMath::FloorToInt((StartWorldPos.X - GridMap.MinPoint.X) / GridMap.GridSpacing);
    int StartY = FMath::FloorToInt((StartWorldPos.Y - GridMap.MinPoint.Y) / GridMap.GridSpacing);

    int TargetX = FMath::FloorToInt((TargetWorldPos.X - GridMap.MinPoint.X) / GridMap.GridSpacing);
    int TargetY = FMath::FloorToInt((TargetWorldPos.Y - GridMap.MinPoint.Y) / GridMap.GridSpacing);

    // 유효성 검사
    StartX = FMath::Clamp(StartX, 0, GridMap.Width - 1);
    StartY = FMath::Clamp(StartY, 0, GridMap.Height - 1);
    TargetX = FMath::Clamp(TargetX, 0, GridMap.Width - 1);
    TargetY = FMath::Clamp(TargetY, 0, GridMap.Height - 1);

    // 노드 가져오기
    FGridNode& StartNode = GridMap.GetNode(StartX, StartY);
    FGridNode& TargetNode = GridMap.GetNode(TargetX, TargetY);

    // 노드로 경로 찾기 로직 호출
    return FindWorldPosPathByNode(GridMap, StartNode, TargetNode);
}

const void FPathFinder::DebugPrint(TArray<FGridNode*>& Path) const
{
    if (!Path.IsEmpty())
    {
        UE_LOG(ELogLevel::Display, "=== 경로 찾기 결과 ===");
        for (auto* Node : Path)
        {
            UE_LOG(ELogLevel::Display, "(%d, %d)", Node->X, Node->Y);
        }
    }
    else {
        UE_LOG(ELogLevel::Display, "경로를 찾지 못했습니다.");
    }
}

const void FPathFinder::DebugWorldPosPath(TArray<FVector>& Path) const
{
    UE_LOG(ELogLevel::Warning, "=== WorldPositionPath ===");

    for (auto pos : Path) {
        UE_LOG(ELogLevel::Display, "x : %f, / y : %f / z : %f", pos.X, pos.Y, pos.Z);
    }
}
