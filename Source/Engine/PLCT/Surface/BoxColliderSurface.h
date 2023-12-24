#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"
#include "Engine/Level/Actor.h"
#include "Engine/Physics/Colliders/BoxCollider.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Vector3.h"
#include "../PLCTSurface.h"

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
    BoxCollider* _actor;
};
