#include "BoxColliderSurface.h"

PLCTPoint* BoxColliderSurface::SampleXZ(Vector2 xz)
{
    if (!_actor)
    {
        return nullptr;
    }

    Vector3 pointFound;

    OrientedBoundingBox actorBox = _actor->GetOrientedBox();
    Vector3 corners[8];
    actorBox.GetCorners(corners);

    Face faces[6];
    faces[0] = { corners[0], corners[1], corners[2], corners[3]}; // top
    faces[1] = { corners[4], corners[5], corners[6], corners[7] }; // bottom
    faces[2] = { corners[4], corners[0], corners[3], corners[7] }; // front
    faces[3] = { corners[6], corners[2], corners[1], corners[5] }; // back
    faces[4] = { corners[5], corners[1], corners[0], corners[4] }; // left
    faces[5] = { corners[7], corners[3], corners[2], corners[6] }; // right

    float highestY = 0;
    bool foundAny = false;
    for (int face = 0; face < 6; face++)
    {
        if (!faces[face].IsWithinBounds(xz))
        {
            continue;
        }

        float y = faces[face].GetYAt(xz);
        if (y > highestY || !foundAny)
        {
            highestY = y;
            pointFound = Vector3(xz.X, y, xz.Y);
        }

        foundAny = true;
    }

    if (!foundAny)
    {
        return nullptr;
    }
    
    Transform pointTransform = Transform::Identity;
    pointTransform.Translation = pointFound;
    pointTransform.Orientation = _actor->GetOrientation();

    PLCTPoint* point = new PLCTPoint();
    point->SetTransform(pointTransform);

    return point;
}

bool BoxColliderSurface::SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer)
{
    PLCTPoint* point = SampleXZ(coordinates);
    if (point == nullptr)
    {
        return false;
    }

    targetContainer->GetPoints().Add(*point);
    return true;
}

bool BoxColliderSurface::CheckActorMatchesAndSet(Actor* actor)
{
    if (actor->Is<BoxCollider>())
    {
        _actor = (BoxCollider*) actor;
        return true;
    }

    return false;
}
