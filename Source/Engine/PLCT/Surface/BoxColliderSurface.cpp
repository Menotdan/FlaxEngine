#include "BoxColliderSurface.h"

PLCTPoint* BoxColliderSurface::SampleXZ(Vector2 coordinates)
{
    Vector3 pointFound;

    BoundingSphere actorSphere = _actor->GetSphere();
    Vector3 pointToSearchFrom = {coordinates.X, actorSphere.Center.Y + actorSphere.Radius, coordinates.Y};
    _actor->ClosestPoint(pointToSearchFrom, pointFound);

    Transform pointTransform = Transform::Identity;
    pointTransform.Translation = pointFound;
    pointTransform.Orientation = _actor->GetOrientation();

    PLCTPoint* point = new PLCTPoint();
    point->SetTransform(pointTransform);

    return point;
}

bool BoxColliderSurface::SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer)
{
    return false;
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
