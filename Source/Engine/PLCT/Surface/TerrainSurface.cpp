#include "BoxColliderSurface.h"
#include "Engine/Level/Actors/PLCTVolume.h"
#include "Engine/Terrain/TerrainPatch.h"
#include "TerrainSurface.h"

bool PointInsdeTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 p)
{
    bool b0 = (Vector2::Dot(Vector2(p.X - v1.X, p.Y - v1.Y), Vector2(v1.Y - v2.Y, v2.X - v1.X)) > 0);
    bool b1 = (Vector2::Dot(Vector2(p.X - v2.X, p.Y - v2.Y), Vector2(v2.Y - v3.Y, v3.X - v2.X)) > 0);
    bool b2 = (Vector2::Dot(Vector2(p.X - v3.X, p.Y - v3.Y), Vector2(v3.Y - v1.Y, v1.X - v3.X)) > 0);
    return (b0 == b1 && b1 == b2);
}

PLCTPoint* TerrainSurface::SampleXZ(Vector2 coordinates)
{
    CHECK_RETURN(_actor, nullptr);
    float volumeY = GetVolume()->GetOrientedBox().Extents.Y + GetVolume()->GetOrientedBox().GetCenter().Y;
    Vector3 topPosition = Vector3(coordinates.X, volumeY, coordinates.Y);

    RayCastHit hit;
    if (!_actor->RayCast(topPosition, Vector3::Down, hit, GetVolume()->GetOrientedBox().Extents.Y * 2))
    {
        return nullptr;
    }

    Transform pointTransform = Transform::Identity;
    pointTransform.Translation = hit.Point;

    PLCTPoint* point = New<PLCTPoint>();
    point->SetTransform(pointTransform);

    return point;

    /*int32 patchesCount = _actor->GetPatchesCount();
    TerrainPatch* patch = nullptr;
    for (int i = 0; i < patchesCount; i++)
    {
        BoundingBox box;
        _actor->GetPatchBounds(i, box);
        if (coordinates.X >= box.Minimum.X && coordinates.X < box.Maximum.X
            && coordinates.Y >= box.Minimum.Z && coordinates.Y < box.Maximum.Z)
        {
            patch = _actor->GetPatch(i);
            break;
        }
    }

    if (!patch)
        return nullptr;

    const Array<Vector3>* triangles = &patch->GetCollisionTriangles();
    CHECK_RETURN(triangles, nullptr);

    int triangleCount = triangles->Count() / 3;
    for (int i = 0; i < triangleCount; i++)
    {
        Vector3 v1Vector3 = triangles->At((i * 3) + 0);
        Vector2 v1 = Vector2(v1Vector3.X, v1Vector3.Z);
        if (Vector2::DistanceSquared(v1, coordinates) > 5000)
            continue;

        Vector3 v2Vector3 = triangles->At((i * 3) + 1);
        Vector3 v3Vector3 = triangles->At((i * 3) + 2);
        Vector2 v2 = Vector2(v2Vector3.X, v2Vector3.Z);
        Vector2 v3 = Vector2(v3Vector3.X, v3Vector3.Z);

        if (PointInsdeTriangle(v1, v2, v3, coordinates))
        {
            Vector3 midpoint = (v1Vector3 + v2Vector3 + v3Vector3) / 3.0f;
            Transform pointTransform = Transform::Identity;
            pointTransform.Translation = midpoint;

            PLCTPoint* point = New<PLCTPoint>();
            point->SetTransform(pointTransform);

            return point;
        }
    }*/

    return nullptr;
}

bool TerrainSurface::SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer)
{
    CHECK_RETURN(targetContainer, false);
    PLCTPoint* point = SampleXZ(coordinates);
    if (point == nullptr)
    {
        return false;
    }

    targetContainer->GetPoints().Add(point);
    return true;
}

bool TerrainSurface::CheckActorMatchesAndSet(Actor* actor)
{
    CHECK_RETURN(actor, false);
    if (actor->Is<Terrain>())
    {
        _actor = (Terrain*)actor;
        return true;
    }

    return false;
}
