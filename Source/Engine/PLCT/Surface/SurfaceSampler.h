#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"
#include "Engine/Level/Actors/PLCTVolume.h"
#include "../PLCTSurface.h"
#include "../PLCTPoint.h"

API_STRUCT() struct SurfaceSamplerSettings
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(SurfaceSamplerSettings)

    API_FIELD() float Spacing = 10;
};

/// <summary>
/// PLCT Point Filter.
/// </summary>
API_CLASS() class FLAXENGINE_API SurfaceSampler : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(SurfaceSampler, ScriptingObject);

public:
    API_FUNCTION() virtual PLCTPointsContainer* SampleXZ(PLCTSurface* surface)
    {
        PLCTVolume* volume = surface->GetVolume();
        PLCTPointsContainer* container = new PLCTPointsContainer();
        OrientedBoundingBox box = volume->GetOrientedBox();
        Vector2 start = Vector2(box.GetCenter().X, box.GetCenter().Z) + Vector2(box.Extents.X, box.Extents.Z);
        Vector2 current = start;

        Vector2 left = Vector2(box.Transformation.GetLeft().X, box.Transformation.GetLeft().Z);
        Vector2 back = Vector2(box.Transformation.GetBackward().X, box.Transformation.GetBackward().Z);

        int iterationCount = 0;
        while (true)
        {
            surface->SampleXZ(current, container);
            current += left * _settings.Spacing;
            if (!Check(volume, current))
            {
                iterationCount++;

                // Reset back to the right
                current = start + back * _settings.Spacing;
                if (!Check(volume, current))
                {
                    break;
                }
            }
        }

        return container;
    }

    API_FUNCTION() void Configure(SurfaceSamplerSettings& settings)
    {
        _settings = settings;
    }

private:
    bool Check(PLCTVolume* volume, Vector2& point)
    {
        return volume->GetOrientedBox().Contains(Vector3(point.X, volume->GetPosition().Y, point.Y)) == ContainmentType::Contains;
    }

    SurfaceSamplerSettings _settings;
};
