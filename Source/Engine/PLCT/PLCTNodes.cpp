#include "PLCTNodes.h"
#include "Surface/BoxColliderSurface.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Core/Random.h"
#include "Engine/Level/Prefabs/PrefabManager.h"
#include "Engine/Level/Prefabs/Prefab.h"

bool PLCTSampleSurface::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    if (id == 1) // Points collection output
    {
        PLCTSurfaceList* surfaces;

        VisjectGraphBox box = node.Boxes[0];
        PLCTGraphNode* connectedNode;
        ScriptingObject* object;

        if (!GetObjectFromInputBox(box, connectedNode, volume, object))
            return false;

        if (!object->Is<PLCTSurfaceList>())
            return false;

        surfaces = (PLCTSurfaceList*) object;
        SurfaceSampler* sampler = New<SurfaceSampler>();
        sampler->Configure(Settings);
        sampler->SetSurfaceList(surfaces);

        PLCTPointsContainer* points = sampler->SampleXZ();
        output = Variant(points);
        Delete(sampler);
        return true;
    }

    return false;
}

bool PLCTGetBoxColliderSurfaces::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    BoxColliderSurface* baseInstance = New<BoxColliderSurface>();
    PLCTSurfaceList* surfaces = volume->FindAllSurfaces(baseInstance);
    Delete(baseInstance);

    output = Variant(surfaces);
    return true;
}

bool GetPoints(VisjectGraphBox box, PLCTNode* node, PLCTVolume* volume, PLCTPointsContainer*& outPoints)
{
    PLCTGraphNode* connectedNode;
    ScriptingObject* object;

    if (!node->GetObjectFromInputBox(box, connectedNode, volume, object))
        return false;

    if (!object->Is<PLCTPointsContainer>())
        return false;

    outPoints = (PLCTPointsContainer*)object;
    return true;
}

bool PLCTDebugDrawPoints::Execute(PLCTGraphNode& node, PLCTVolume* volume)
{
    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    if (!GetPoints(box, this, volume, points))
        return false;

    for (int i = 0; i < points->GetPoints().Count(); i++)
    {
#ifdef USE_EDITOR
        DebugDraw::DrawSphere(BoundingSphere(points->GetPoints()[i]->GetTransform().Translation, 2), PointColor, 20.0f);
#endif
    }

    return true;
}

bool PLCTSpawnPrefabAtPoints::Execute(PLCTGraphNode& node, PLCTVolume* volume)
{
    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    if (!GetPoints(box, this, volume, points))
        return false;

    for (int pointIdx = 0; pointIdx < points->GetPoints().Count(); pointIdx++)
    {
        float pickedPrefabNum = Random::Rand();
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
        PrefabManager::SpawnPrefab(entry->Prefab.Get(), volume, points->GetPoints()[pointIdx]->GetTransform());
    }

    return true;
}
