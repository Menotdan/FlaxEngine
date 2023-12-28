#pragma once

#include "BoxVolume.h"
#include "Engine/Content/AssetReference.h"

class PLCTSurface;
class PLCTSurfaceList;
class PLCTGraph;

/// <summary>
/// PLCT Volume.
/// </summary>
API_CLASS(Attributes = "ActorContextMenu(\"New/Other/PLCT Volume\"), ActorToolbox(\"Other\")")
class FLAXENGINE_API PLCTVolume : public BoxVolume
{
    DECLARE_SCENE_OBJECT(PLCTVolume);
    API_AUTO_SERIALIZATION();

    /// <summary>
    /// The graph that gets executed when this volume is set to generate.
    /// </summary>
    API_FIELD(Attributes = "DefaultValue(null), EditorDisplay(\"PLCT Graph\")")
    AssetReference<PLCTGraph> Graph;

    /// <summary>
    /// Generate using this volume.
    /// </summary>
    /// <returns>True if generation succeeded, otherwise false.</returns>
    API_FUNCTION() bool Generate();

    /// <summary>
    /// Finds the first surface of the surface instance type, and then reads that data into the given surface object.
    /// </summary>
    /// <param name="surface">The target surface object.</param>
    /// <returns>True if found the surface, otherwise false.</returns>
    API_FUNCTION() bool FindFirstSurface(PLCTSurface* surface);

    /// <summary>
    /// Find all surfaces of the provided type.
    /// </summary>
    /// <param name="baseInstance">The base instance of the intended surface type.</param>
    /// <returns>The surface list, or null if no surfaces were found.</returns>
    API_FUNCTION() PLCTSurfaceList* FindAllSurfaces(PLCTSurface* baseInstance);

    /// <summary>
    /// Find the surface of the provided type at the given index.
    /// </summary>
    /// <param name="surface">The target surface object.</param>
    /// <param name="index">The index of surface to search for.</param>
    /// <returns>True if found matching surface, otherwise false.</returns>
    API_FUNCTION() bool FindSurfaceAtIndex(PLCTSurface* surface, int index);
};
