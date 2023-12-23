#pragma once

#include "PLCTProperties.h"
#include "PLCTPoint.h"
#include "Engine/Core/Config.h"
#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"

/// <summary>
/// PLCT Surface class used for sampling points.
/// </summary>
API_CLASS(Abstract, NoSpawn) class FLAXENGINE_API PLCTSurface : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTSurface);

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
        return new PLCTPoint();
    }

    /// <summary>
    /// Samples the surface along the X and Z axes.
    /// </summary>
    /// <param name="coordinates">(X,Z) coordinates.</param>
    /// <param name="targetContainer">The container to add the point to.</param>
    /// <returns>True if there is a valid point, otherwise false.</returns>
    API_FUNCTION() virtual bool SampleXZ(Vector2 coordinates, PLCTPointsContainer* targetContainer)
    {
        targetContainer->GetPoints().Add(PLCTPoint());
        return true;
    }

private:
    PLCTPropertyStorage _properties;
};
