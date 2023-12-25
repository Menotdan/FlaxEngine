#pragma once

#include "PLCTProperties.h"
#include "PLCTPoint.h"
#include "Engine/Core/Config.h"
#include "Engine/Core/Delegate.h"
#include "Engine/Level/Actor.h"
#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"

class PLCTVolume;

/// <summary>
/// PLCT Surface class used for sampling points.
/// </summary>
API_CLASS(Abstract) class FLAXENGINE_API PLCTSurface : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTSurface, ScriptingObject);

public:
    /// <summary>
    /// Gets the properties of the surface.
    /// </summary>
    /// <returns>The surface properties.</returns>
    API_PROPERTY() PLCTPropertyStorage* GetProperties()
    {
        return &_properties;
    }

    /// <summary>
    /// Samples the surface along the X and Z axes.
    /// </summary>
    /// <param name="coordinates">(X,Z) coordinates.</param>
    /// <returns>Sampled point, or null if there is no valid point.</returns>
    API_FUNCTION() virtual PLCTPoint* SampleXZ(Vector2 coordinates)
    {
        return nullptr;
    }

    /// <summary>
    /// Samples the surface along the X and Z axes.
    /// </summary>
    /// <param name="coordinates">(X,Z) coordinates.</param>
    /// <param name="targetContainer">The container to add the point to.</param>
    /// <returns>True if there is a valid point, otherwise false.</returns>
    API_FUNCTION() virtual bool SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer)
    {
        return false;
    }

    API_FUNCTION() virtual bool CheckActorMatchesAndSet(Actor* actor)
    {
        return false;
    }

    API_PROPERTY() PLCTVolume* GetVolume()
    {
        return _volume;
    }

    API_PROPERTY() void SetVolume(PLCTVolume* volume)
    {
        _volume = volume;
    }

private:
    PLCTPropertyStorage _properties;
    PLCTVolume* _volume = nullptr;
};
