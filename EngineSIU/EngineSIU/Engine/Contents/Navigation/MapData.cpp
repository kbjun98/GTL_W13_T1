#include "MapData.h"
#include <fstream>
#include <UserInterface/Console.h>
#include <sstream>
#include <Components/StaticMeshComponent.h>
#include <UObject/UObjectIterator.h>
#include <Engine/Engine.h>
#include "Runtime/Engine/World/World.h"
void FGridMap::LoadMapFromFile(const FString FilePath)
{
   /* std::ifstream File(FilePath.ToAnsiString());

    if (!File.is_open())
    {
        //UE_LOG(ELogLevel::Warning, "Cannot open MapFile");
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
    /* 엔진 내 콘솔 출력*/
   /* //UE_LOG(ELogLevel::Display, "맵 디버그 출력 (Width = %d, Height = %d): ", Width, Height);
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
        //UE_LOG(ELogLevel::Display, "%s", *log);
    }*/
    std::cout << "맵 디버그 출력 (Width = " << Width << ", Height = " << Height << "):" << std::endl;
    for (int y = 0; y < Height; ++y)
    {
        std::string log;
        for (int x = 0; x < Width; ++x)
        {
            FIntPoint Key(x, y);
            uint32 KeyIndex = Get1DIndex(Key.X, Key.Y);
            const FGridNode* Node = GridNodes.Find(KeyIndex);

            if (Node) {
                log += Node->bWalkable ? "O " : "X ";
            }
        }
        std::cout << log << std::endl;
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

    MinPoint= FVector(FLT_MAX);
    MaxPoint = FVector(-FLT_MAX);
    
    FVector Origin = FVector::ZeroVector;

    GridNodes.Empty();


    // 모든 StaticMeshComponent의 AABB로 전체 영역 계산
    for (auto It : TObjectRange<UStaticMeshComponent>())
    {
        if (It->GetMeshType() != EStaticMeshType::Map)
        {
            continue;
        }
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
            //FVector WorldCorner = WorldTransform.TransformPositionWithoutScale(LocalCorners[i]);
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
    FGridMap* GridMap = GEngine->ActiveWorld->GetGridMap();

    Width = FMath::CeilToInt((MaxPoint.X - MinPoint.X) / GridMap->GridSpacing);
    Height = FMath::CeilToInt((MaxPoint.Y - MinPoint.Y) / GridMap->GridSpacing);

    //UE_LOG(ELogLevel::Display, TEXT("GridMap 영역: Width=%d, Height=%d"), Width, Height);
    //UE_LOG(ELogLevel::Display, TEXT("MinPoint: %s"), *MinPoint.ToString());
    //UE_LOG(ELogLevel::Display, TEXT("MaxPoint: %s"), *MaxPoint.ToString());

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
        
        // Obstacle로 인해 false면 패스
        if (Node.bWalkable == false)
        {
            continue;
        }
        Node.bWalkable = false;

        // Ray 시작점: 노드 위치 위에서 시작
        FVector RayStart = Node.GetPosition() + FVector(0.0f, 0.0f, StartZOffset);

        bool bHit = false;
        bool bObstacleHit = false;
        bool bMapHit = false;
        FVector HitNormal = FVector::UpVector;
        EStaticMeshType StMeshType = EStaticMeshType::NONE;

        // 모든 StaticMeshComponent 순회
        for (auto It : TObjectRange<UStaticMeshComponent>())
        {
            StMeshType = It->GetMeshType();
            if (StMeshType == EStaticMeshType::Map || StMeshType == EStaticMeshType::Obstacle) {
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
                        if (StMeshType == EStaticMeshType::Obstacle) {
                            bObstacleHit = true;
                            break; // 장애물 충돌하면 바로 멈춰도 됨
                        }
                        else if (StMeshType == EStaticMeshType::Map) {
                            bMapHit = true;
                            HitNormal = TempNormal; // 가장 마지막 Map 메시의 법선 저장
                        }
                    }
                }

                if (bObstacleHit) break;
            }
        }

        // 최종 Walkable 결정
        if (bObstacleHit)
        {
            Node.bWalkable = false;
        }
        else if (bMapHit)
        {
            Node.bWalkable = FMath::Abs(HitNormal.Z) > 0.5f;
        }
        else
        {
            Node.bWalkable = false;
        }
    }

    //UE_LOG(ELogLevel::Display, TEXT("Raycast + Normal 기반 장애물 분석 완료!"));
}

void FGridMap::SaveToBinaryFile(const FString& FilePath)
{
    std::ofstream OutFile(FilePath.ToAnsiString(), std::ios::binary);
    if (!OutFile.is_open())
    {
        //UE_LOG(ELogLevel::Error, "Failed to open file for saving: %s", *FilePath);
        return;
    }

    OutFile.write(reinterpret_cast<const char*>(&Width), sizeof(int));
    OutFile.write(reinterpret_cast<const char*>(&Height), sizeof(int));
    OutFile.write(reinterpret_cast<const char*>(&MinPoint), sizeof(FVector));
    OutFile.write(reinterpret_cast<const char*>(&MaxPoint), sizeof(FVector));

    int32 NodeCount = GridNodes.Num();
    OutFile.write(reinterpret_cast<const char*>(&NodeCount), sizeof(int32));

    for (const auto& Pair : GridNodes)
    {
        int32 Index = Pair.Key;
        const FGridNode& Node = Pair.Value;

        OutFile.write(reinterpret_cast<const char*>(&Index), sizeof(int32));
        OutFile.write(reinterpret_cast<const char*>(&Node.X), sizeof(int));
        OutFile.write(reinterpret_cast<const char*>(&Node.Y), sizeof(int));
        OutFile.write(reinterpret_cast<const char*>(&Node.bWalkable), sizeof(bool));
        OutFile.write(reinterpret_cast<const char*>(&Node.WorldPosition), sizeof(FVector));
    }

    OutFile.close();
    //UE_LOG(ELogLevel::Display, "GridMap 저장 완료: %s", *FilePath);
}

void FGridMap::LoadFromBinaryFile(const FString& FilePath)
{
    FGridMap* GridMap = GEngine->ActiveWorld->GetGridMap();

    std::ifstream InFile(FilePath.ToAnsiString(), std::ios::binary);
    if (!InFile.is_open())
    {
        //UE_LOG(ELogLevel::Error, "Failed to open file for loading: %s", *FilePath);
        return;
    }

    InFile.read(reinterpret_cast<char*>(&Width), sizeof(int));
    InFile.read(reinterpret_cast<char*>(&Height), sizeof(int));
    InFile.read(reinterpret_cast<char*>(&MinPoint), sizeof(FVector));
    InFile.read(reinterpret_cast<char*>(&MaxPoint), sizeof(FVector));

    int32 NodeCount = 0;
    InFile.read(reinterpret_cast<char*>(&NodeCount), sizeof(int32));
    GridMap->GridNodes.Empty();

    for (int i = 0; i < NodeCount; ++i)
    {
        int32 Index;
        FGridNode Node;
        InFile.read(reinterpret_cast<char*>(&Index), sizeof(int32));
        InFile.read(reinterpret_cast<char*>(&Node.X), sizeof(int));
        InFile.read(reinterpret_cast<char*>(&Node.Y), sizeof(int));
        InFile.read(reinterpret_cast<char*>(&Node.bWalkable), sizeof(bool));
        InFile.read(reinterpret_cast<char*>(&Node.WorldPosition), sizeof(FVector));
        GridMap->GridNodes.Add(Index, Node);
    }

    InFile.close();
    //UE_LOG(ELogLevel::Display, "GridMap 로드 완료: %s", *FilePath);
    GridMap->DebugPrint();
}




