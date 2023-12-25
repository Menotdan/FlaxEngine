#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"
#include "Engine/Level/Actor.h"
#include "Engine/Physics/Colliders/BoxCollider.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Vector3.h"
#include "../PLCTSurface.h"

struct Face
{
    Vector3 edge1;
    Vector3 edge2;
    Vector3 edge3;
    Vector3 edge4;

    bool IsWithinBounds(Vector2 xz)
    {
        Vector2 AB = Vector2(edge2.X, edge2.Z) - Vector2(edge1.X, edge1.Z);
        Vector2 AM = xz - Vector2(edge1.X, edge1.Z);
        Vector2 BC = Vector2(edge3.X, edge3.Z) - Vector2(edge2.X, edge2.Z);
        Vector2 BM = xz - Vector2(edge2.X, edge2.Z);

        float dot1 = Vector2::Dot(AB, AM);
        float dot2 = Vector2::Dot(AB, AB);
        float dot3 = Vector2::Dot(BC, BM);
        float dot4 = Vector2::Dot(BC, BC);
        return 0 <= dot1 && dot1 <= dot2 &&
            0 <= dot3 && dot3 <= dot4;
    }

    float GetYAt(Vector2 xz)
    {
        Vector3 v1 = edge1 - edge2;
        Vector3 v2 = edge1 - edge3;
        Vector3 n = Vector3::Cross(v1, v2);

        return (1 / Math::Max(n.Y, 0.0001f)) * ((n.X * edge1.X) + (n.Y * edge1.Y) + (n.Z * edge1.Z) - (n.X * xz.X) - (n.Z * xz.Y));
    }
};

/// <summary>
/// Box collider PLCT surface.
/// </summary>
API_CLASS(Abstract, NoSpawn) class FLAXENGINE_API BoxColliderSurface : public PLCTSurface
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(BoxColliderSurface);

public:
    // [PLCTSurface]
    PLCTPoint* SampleXZ(Vector2 coordinates) override;
    bool SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer) override;
    bool CheckActorMatchesAndSet(Actor* actor) override;
private:
    BoxCollider* _actor = nullptr;
};
