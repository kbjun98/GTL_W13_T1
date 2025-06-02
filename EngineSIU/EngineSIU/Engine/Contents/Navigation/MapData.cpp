#include "MapData.h"
#include <fstream>
#include <UserInterface/Console.h>
#include <sstream>
#include <Components/StaticMeshComponent.h>
#include <UObject/UObjectIterator.h>

void FGridMap::LoadMapFromFile(const FString FilePath)
{
   /* std::ifstream File(FilePath.ToAnsiString());

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

    File.close();*/
}

FGridNode& FGridMap::GetNode(int X, int Y)
{
    return GridNodes[Get1DIndex(X, Y)];
}

void FGridMap::DebugPrint() const
{
    UE_LOG(ELogLevel::Display, "맵 디버그 출력 (Width = %d, Height = %d): ", Width, Height);
    for (int y = 0; y < Height; ++y)
    {
        FString log;
        for (int x = 0; x < Width; ++x)
        {
            FIntPoint Key(x, y);

            uint32 KeyIndex = Get1DIndex(Key.X, Key.Y);
            const FGridNode* Node = GridNodes.Find(KeyIndex);
            if (Node) {
                if (Node->bWalkable) {
                    log = log + "O ";
                }
                else {
                    log = log + "X ";

                }
            }
        }
        UE_LOG(ELogLevel::Display, "%s", *log);
    }
}

void FGridMap::InitializeGridMap()
{
    InitializeGridNodeFromMeshes();
    AnalyzeWalkableFromMeshes();

    DebugPrint();
}


void FGridMap::InitializeGridNodeFromMeshes()
{
    const float GridSpacing = 10.0f;

    MinPoint= FVector(FLT_MAX);
    MaxPoint = FVector(-FLT_MAX);
    
    FVector Origin = FVector::ZeroVector;

    GridNodes.Empty();


    // 모든 StaticMeshComponent의 AABB로 전체 영역 계산
    for (auto It : TObjectRange<UStaticMeshComponent>())
    {
        FBoundingBox LocalAABB = It->GetBoundingBox();

        // 월드 변환 정보
        FTransform WorldTransform = It->GetComponentTransform();

        
        // AABB의 8개의 코너를 월드 공간으로 변환 후, 전체 Min/Max 계산
        FVector LocalMin = LocalAABB.MinLocation;
        FVector LocalMax = LocalAABB.MaxLocation;

        FVector LocalCorners[8] =
        {
            FVector(LocalMin.X, LocalMin.Y, LocalMin.Z),
            FVector(LocalMax.X, LocalMin.Y, LocalMin.Z),
            FVector(LocalMin.X, LocalMax.Y, LocalMin.Z),
            FVector(LocalMax.X, LocalMax.Y, LocalMin.Z),

            FVector(LocalMin.X, LocalMin.Y, LocalMax.Z),
            FVector(LocalMax.X, LocalMin.Y, LocalMax.Z),
            FVector(LocalMin.X, LocalMax.Y, LocalMax.Z),
            FVector(LocalMax.X, LocalMax.Y, LocalMax.Z)
        };

        // TODO : 중복된 월드 포지션 계산 함수로 빼기
        for (int i = 0; i < 8; ++i)
        {
            FVector WorldCorner = WorldTransform.TransformPosition(LocalCorners[i]);

            MinPoint.X = FMath::Min(MinPoint.X, WorldCorner.X);
            MinPoint.Y = FMath::Min(MinPoint.Y, WorldCorner.Y);
            MinPoint.Z = FMath::Min(MinPoint.Z, WorldCorner.Z);

            MaxPoint.X = FMath::Max(MaxPoint.X, WorldCorner.X);
            MaxPoint.Y = FMath::Max(MaxPoint.Y, WorldCorner.Y);
            MaxPoint.Z = FMath::Max(MaxPoint.Z, WorldCorner.Z);
        }
    }

    // 그리드 크기 계산
    Width = FMath::CeilToInt((MaxPoint.X - MinPoint.X) / GridSpacing);
    Height = FMath::CeilToInt((MaxPoint.Y - MinPoint.Y) / GridSpacing);

    UE_LOG(ELogLevel::Display, TEXT("GridMap 영역: Width=%d, Height=%d"), Width, Height);
    UE_LOG(ELogLevel::Display, TEXT("MinPoint: %s"), *MinPoint.ToString());
    UE_LOG(ELogLevel::Display, TEXT("MaxPoint: %s"), *MaxPoint.ToString());

    // GridNode 초기화
    for (int Y = 0; Y < Height; ++Y)
    {
        for (int X = 0; X < Width; ++X)
        {
            FIntPoint Key(X, Y);
            FGridNode NewNode;

            // 월드 위치 계산 (MinPoint 기준으로 오프셋)
            float WorldX = MinPoint.X + X * GridSpacing + GridSpacing * 0.5f; // 센터 좌표
            float WorldY = MinPoint.Y + Y * GridSpacing + GridSpacing * 0.5f;

            NewNode.bWalkable = true; // 초기값: 이동 가능
            NewNode.X = X;
            NewNode.Y = Y;

            NewNode.WorldPosition = FVector(WorldX, WorldY, 0.0f);

            GridNodes.Add(Get1DIndex(Key.X, Key.Y), NewNode);
        }
    }
    
}

void FGridMap::AnalyzeWalkableFromMeshes()
{
    // Raycast는 위에서 아래로 (바닥 찾기)
    const FVector RayDirection = FVector(0.0f, 0.0f, -1.0f);
    const float RayLength = 1000.0f; // 충분히 긴 거리
    const float StartZOffset = 500.0f; // 시작점 Z 보정

    // 모든 노드 순회
    for (auto& Elem : GridNodes)
    {
        FGridNode& Node = Elem.Value;

        // Ray 시작점: 노드 위치 위에서 시작
        FVector RayStart = Node.GetPosition() + FVector(0.0f, 0.0f, StartZOffset);

        bool bHit = false;
        FVector HitNormal = FVector::UpVector;

        // 모든 StaticMeshComponent 순회
        for (auto It : TObjectRange<UStaticMeshComponent>())
        {
            UStaticMesh* Mesh = It->GetStaticMesh();
            if (!Mesh) continue;

            FStaticMeshRenderData* RenderData = Mesh->GetRenderData();
            if (!RenderData) continue;

            FTransform WorldTransform = It->GetComponentTransform();
            TArray<FStaticMeshVertex>& Vertices = RenderData->Vertices;
            const TArray<uint32>& Indices = RenderData->Indices;
            const int TriangleNum = Indices.Num() / 3;

            // 메시의 모든 삼각형 순회
            for (int TriIdx = 0; TriIdx < TriangleNum; ++TriIdx)
            {
                // 삼각형 월드 좌표로 변환
                FVector V0 = WorldTransform.TransformPosition(Vertices[Indices[TriIdx * 3 + 0]].GetPosition());
                FVector V1 = WorldTransform.TransformPosition(Vertices[Indices[TriIdx * 3 + 1]].GetPosition());
                FVector V2 = WorldTransform.TransformPosition(Vertices[Indices[TriIdx * 3 + 2]].GetPosition());

                float HitDist;
                FVector TempNormal;

                if (It->IntersectRayTriangle(RayStart, RayDirection, V0, V1, V2, HitDist, TempNormal))
                {
                    bHit = true;
                    HitNormal = TempNormal;
                    break; // 하나만 맞으면 장애물로 간주
                }
            }

            if (bHit) break;
        }

        // 충돌 결과로 Walkable 여부 판정
        if (bHit)
        {
            // 바닥일 가능성: 법선의 Z성분이 크면 바닥
            if (FMath::Abs(HitNormal.Z) > 0.5f)
            {
                Node.bWalkable = true; // 바닥 → 이동 가능
            }
            else
            {
                Node.bWalkable = false; // 벽 → 이동 불가
            }
        }
        else
        {
            Node.bWalkable = false; // 아무 메시도 없으면 공중 → 이동 불가
        }
    }

    UE_LOG(ELogLevel::Display, TEXT("Raycast + Normal 기반 장애물 분석 완료!"));
}




