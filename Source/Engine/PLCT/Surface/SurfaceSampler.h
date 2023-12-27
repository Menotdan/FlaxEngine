#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"
#include "Engine/Core/Config.h"
#include "Engine/Level/Actors/PLCTVolume.h"
#include "../PLCTSurface.h"
#include "../PLCTPoint.h"

API_STRUCT() struct SurfaceSamplerSettings
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(SurfaceSamplerSettings)

    API_FIELD() float Spacing = 10;
};

/// <summary>
/// PLCT Surface Sampler. Samples surfaces at points on a grid.
/// </summary>
API_CLASS() class FLAXENGINE_API SurfaceSampler : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(SurfaceSampler, ScriptingObject);

public:
    /// <summary>
    /// Sample all XZ coordinates.
    /// </summary>
    /// <param name="surface">The target surface.</param>
    /// <param name="container">The output container.</param>
    /// <returns>True if any points were sampled, otherwise false.</returns>
    API_FUNCTION() virtual bool SampleXZ(PLCTSurface* surface, PLCTPointsContainer* container)
    {
        PLCTVolume* volume = surface->GetVolume();
        OrientedBoundingBox box = volume->GetOrientedBox();
        Vector2 start = Vector2(box.GetCenter().X, box.GetCenter().Z) + Vector2(box.Extents.X, box.Extents.Z);
        bool foundAnyPoint = false;

        Vector2 left = Vector2(box.Transformation.GetLeft().X, box.Transformation.GetLeft().Z);
        Vector2 back = Vector2(box.Transformation.GetBackward().X, box.Transformation.GetBackward().Z);
        start += left * 0.01f;
        start += back * 0.01f;

        Vector2 current = start;

        int iterationCount = 0;
        while (true)
        {
            foundAnyPoint = surface->SampleXZ(current, container) || foundAnyPoint;
            current += left * _settings.Spacing;
            if (!Check(volume, current))
            {
                iterationCount++;

                // Reset back to the right
                current = start + (back * (_settings.Spacing * (float) iterationCount));
                if (!Check(volume, current))
                {
                    break;
                }
            }
        }

        return foundAnyPoint;
    }

    /// <summary>
    /// Sample all XZ coordinates.
    /// </summary>
    /// <param name="surface">The target surface.</param>
    /// <returns>The points container, or null if no points were sampled.</returns>
    API_FUNCTION() virtual PLCTPointsContainer* SampleXZ(PLCTSurface* surface)
    {
        PLCTPointsContainer* container = new PLCTPointsContainer();
        bool foundPoint = SampleXZ(surface, container);

        if (!foundPoint)
        {
            container->DeleteObjectNow();
            return nullptr;
        }

        return container;
    }

    /// <summary>
    /// Sample all XZ coordinates from every surface in this sampler.
    /// </summary>
    /// <returns>The points container, or null if no points were sampled.</returns>
    API_FUNCTION() virtual PLCTPointsContainer* SampleXZ()
    {
        PLCTPointsContainer* container = new PLCTPointsContainer();
        bool foundAnyPoints = false;

        for (int surfIdx = 0; surfIdx < _surfaces.GetSurfaces().Count(); surfIdx++)
        {
            foundAnyPoints = SampleXZ(_surfaces.GetSurfaces()[surfIdx], container) || foundAnyPoints;
        }

        return foundAnyPoints ? container : nullptr;
    }

    API_FUNCTION() void Configure(SurfaceSamplerSettings& settings)
    {
        _settings = settings;
    }

    /// <summary>
    /// Gets/Sets the surface list.
    /// </summary>
    API_PROPERTY() void SetSurfaceList(PLCTSurfaceList* surfaces)
    {
        _surfaces = *surfaces;
    }

    /// <summary>
    /// Gets/Sets the surface list.
    /// </summary>
    API_PROPERTY() PLCTSurfaceList* GetSurfaceList()
    {
        return &_surfaces;
    }

private:
    bool Check(PLCTVolume* volume, Vector2& point)
    {
        return volume->GetOrientedBox().Contains(Vector3(point.X, volume->GetPosition().Y, point.Y)) == ContainmentType::Contains;
    }

    PLCTSurfaceList _surfaces;
    SurfaceSamplerSettings _settings;
};
