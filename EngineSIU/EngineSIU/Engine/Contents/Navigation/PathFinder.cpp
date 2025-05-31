#include "PathFinder.h"
#include <Container/Set.h>

TArray<FGridNode*> FPathFinder::FindPath(FGridMap& GridMap, FGridNode& StartNode, FGridNode& TargetNode)
{
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
            // 경로 재구성
            TArray<FGridNode*> Path;
            while (CurrentNode)
            {
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
