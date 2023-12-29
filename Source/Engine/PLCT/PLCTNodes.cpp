#include "PLCTNodes.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Core/RandomStream.h"
#include "Engine/Level/Actors/PLCTVolume.h"
#include "Engine/Level/Prefabs/PrefabManager.h"
#include "Engine/Level/Prefabs/Prefab.h"

#include "PLCTProperties.h"
#include "Surface/BoxColliderSurface.h"
#include "Surface/TerrainSurface.h"

bool PLCTSampleSurface::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    LOG(Warning, "sample surface");
    Variant Cached = volume->RuntimeCache->GetPropertyValue(GetID().ToString());
    if (!(Cached.Type == VariantType::Null))
    {
        Arch2RuntimeCache* cache = (Arch2RuntimeCache*)Cached.AsPointer;
        output = Variant(cache->Points);
        return true;
    }

    PLCTSurfaceList* surfaces;

    VisjectGraphBox box = node.Boxes[0];
    PLCTGraphNode* connectedNode;
    ScriptingObject* object;

    if (!GetObjectFromInputBox(box, connectedNode, volume, object))
        return false;

    CHECK_RETURN(object, false);
    if (!object->Is<PLCTSurfaceList>())
        return false;

    surfaces = (PLCTSurfaceList*) object;
    SurfaceSampler* sampler = New<SurfaceSampler>();
    sampler->Configure(Settings);
    sampler->SetSurfaceList(surfaces);

    PLCTPointsContainer* points = sampler->SampleXZ();
    Delete(sampler);

    Arch2RuntimeCache* cache = New<Arch2RuntimeCache>();
    cache->Points = points;
    volume->RuntimeCache->SetPropertyValue(GetID().ToString(), Variant(cache));

    output = Variant(points);
    return true;
}

bool PLCTGetBoxColliderSurfaces::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    LOG(Warning, "get collider surfaces");
    Variant Cached = volume->RuntimeCache->GetPropertyValue(GetID().ToString());
    if (!(Cached.Type == VariantType::Null))
    {
        Arch0RuntimeCache* cache = (Arch0RuntimeCache*) Cached.AsPointer;
        output = Variant(cache->SurfaceList);
        return true;
    }

    BoxColliderSurface* baseInstance = New<BoxColliderSurface>();
    PLCTSurfaceList* surfaces = volume->FindAllSurfaces(baseInstance);
    if (surfaces == nullptr)
    {
        Delete(baseInstance);
        output = Variant(nullptr);
        return false;
    }

    if (surfaces->GetSurfaces().Count() == 0)
    {
        Delete(surfaces);
        Delete(baseInstance);
        output = Variant(nullptr);
        return false;
    }
    Delete(baseInstance);

    Arch0RuntimeCache* cache = New<Arch0RuntimeCache>();
    cache->SurfaceList = surfaces;
    volume->RuntimeCache->SetPropertyValue(GetID().ToString(), Variant(cache));

    output = Variant(surfaces);
    return true;
}

bool PLCTGetTerrainSurfaces::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    Variant Cached = volume->RuntimeCache->GetPropertyValue(GetID().ToString());
    if (!(Cached.Type == VariantType::Null))
    {
        Arch0RuntimeCache* cache = (Arch0RuntimeCache*)Cached.AsPointer;
        output = Variant(cache->SurfaceList);
        return true;
    }

    TerrainSurface* baseInstance = New<TerrainSurface>();
    PLCTSurfaceList* surfaces = volume->FindAllSurfaces(baseInstance);
    CHECK_RETURN(surfaces, false);
    Delete(baseInstance);

    Arch0RuntimeCache* cache = New<Arch0RuntimeCache>();
    cache->SurfaceList = surfaces;
    volume->RuntimeCache->SetPropertyValue(GetID().ToString(), Variant(cache));

    output = Variant(surfaces);
    return true;
}


bool GetPoints(VisjectGraphBox box, PLCTNode* node, PLCTVolume* volume, PLCTPointsContainer*& outPoints)
{
    PLCTGraphNode* connectedNode;
    ScriptingObject* object;

    CHECK_RETURN(node, false);
    if (!node->GetObjectFromInputBox(box, connectedNode, volume, object))
        return false;

    CHECK_RETURN(object, false);
    if (!object->Is<PLCTPointsContainer>())
        return false;

    outPoints = (PLCTPointsContainer*)object;
    return true;
}

bool PLCTDebugDrawPoints::Execute(PLCTGraphNode& node, PLCTVolume* volume)
{
    LOG(Warning, "debug draw");
    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    if (!GetPoints(box, this, volume, points))
        return false;

    CHECK_RETURN(points, false);
    for (int i = 0; i < points->GetPoints().Count(); i++)
    {
#if USE_EDITOR
        DebugDraw::DrawSphere(BoundingSphere(points->GetPoints()[i]->GetTransform().Translation, 2), PointColor, 20.0f);
#endif
    }

    return true;
}

bool PLCTSpawnPrefabAtPoints::Execute(PLCTGraphNode& node, PLCTVolume* volume)
{
    LOG(Warning, "spawn prefabs");
    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    if (!GetPoints(box, this, volume, points))
        return false;

    RandomStream stream = RandomStream();
    stream.GenerateNewSeed();

    CHECK_RETURN(points, false);
    for (int pointIdx = 0; pointIdx < points->GetPoints().Count(); pointIdx++)
    {
        float pickedPrefabNum = stream.GetFraction();
        float totalRead = 0;
        PrefabSpawnEntry* entry = nullptr;

        for (int entryIdx = 0; entryIdx < Prefabs.Count(); entryIdx++)
        {
            totalRead += Prefabs[entryIdx].Weight;
            if (totalRead >= pickedPrefabNum)
            {
                entry = &Prefabs[entryIdx];
                break;
            }
        }

        if (entry == nullptr)
            continue;
        if (!entry->Prefab || entry->Prefab->WaitForLoaded())
            continue;

        PrefabManager::SpawnPrefab(entry->Prefab.Get(), (Actor*) volume->GenerationContainer.Get(), points->GetPoints()[pointIdx]->GetTransform());
    }

    return true;
}

bool PLCTFilterByRandom::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    LOG(Warning, "filter by random");
    Variant Cached = volume->RuntimeCache->GetPropertyValue(GetID().ToString());
    if (!(Cached.Type == VariantType::Null))
    {
        Arch2RuntimeCache* cache = (Arch2RuntimeCache*)Cached.AsPointer;
        output = Variant(cache->Points);
        return true;
    }

    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    if (!GetPoints(box, this, volume, points))
        return false;

    PLCTPointsContainer* filteredPoints = New<PLCTPointsContainer>();

    CHECK_RETURN(points, false);
    for (int pointIdx = 0; pointIdx < points->GetPoints().Count(); pointIdx++)
    {
        PLCTPoint* point = points->GetPoints()[pointIdx];
        CHECK_RETURN(point, false);
        Variant randomValue = point->GetProperties()->GetPropertyValue(TEXT("Random"));
        if (randomValue.Type == VariantType::Null)
            continue;

        if (randomValue.AsFloat < MinValue ||
            randomValue.AsFloat > MaxValue)
        {
            continue;
        }

        PLCTPoint* filteredPoint = New<PLCTPoint>();
        Memory::CopyItems<PLCTPoint>(filteredPoint, point, 1);
        filteredPoints->GetPoints().Add(filteredPoint);
    }

    Arch2RuntimeCache* cache = New<Arch2RuntimeCache>();
    cache->Points = filteredPoints;
    volume->RuntimeCache->SetPropertyValue(GetID().ToString(), Variant(cache));

    output = Variant(filteredPoints);
    return true;
}
