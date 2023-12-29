#pragma once

#include "PLCTNode.h"
#include "Surface/SurfaceSampler.h"
#include "Engine/Core/Collections/Array.h"

/// <summary>
/// Gets all box collider surfaces found in the PLCT volume.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTGetBoxColliderSurfaces : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTGetBoxColliderSurfaces, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 0;
    }

public:
    // [PLCTNode]
    bool GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output) override;
};

/// <summary>
/// Gets all terrain surfaces found in the PLCT volume.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTGetTerrainSurfaces : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTGetTerrainSurfaces, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 0;
    }

public:
    // [PLCTNode]
    bool GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output) override;
};

/// <summary>
/// Samples a PLCT surface, generating points.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTSampleSurface : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTSampleSurface, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    API_FIELD(Attributes = "EditorOrder(10)")
    SurfaceSamplerSettings Settings;

    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 2;
    }

public:
    // [PLCTNode]
    bool GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output) override;
};

/// <summary>
/// Takes a list of points and debugs them.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTDebugDrawPoints : public PLCTNodeEnd
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTDebugDrawPoints, PLCTNodeEnd);
    API_AUTO_SERIALIZATION();

public:
    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 3;
    }

    API_FIELD(Attributes = "EditorOrder(10)")
    Color PointColor;

public:
    // [PLCTNodeEnd]
    bool Execute(PLCTGraphNode& node, PLCTVolume* volume) override;
};

API_STRUCT() struct FLAXENGINE_API PrefabSpawnEntry : ISerializable
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(PrefabSpawnEntry)
    API_AUTO_SERIALIZATION();

    /// <summary>
    /// The weight of this entry, from 0.0 to 1.0. If the total weight of all entries is greater than 1.0, entries that start past 1.0 will never spawn.
    /// </summary>
    API_FIELD() float Weight;

    /// <summary>
    /// The prefab to spawn. Can be set empty to spawn nothing.
    /// </summary>
    API_FIELD() AssetReference<Prefab> Prefab;
};

/// <summary>
/// Takes a list of points and spawns a prefab at each point.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTSpawnPrefabAtPoints : public PLCTNodeEnd
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTSpawnPrefabAtPoints, PLCTNodeEnd);
    API_AUTO_SERIALIZATION();

public:
    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 3;
    }

    API_FIELD(Attributes = "EditorOrder(10)")
    Array<PrefabSpawnEntry> Prefabs;

public:
    // [PLCTNodeEnd]
    bool Execute(PLCTGraphNode& node, PLCTVolume* volume) override;
};

/// <summary>
/// Samples a PLCT surface, generating points.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTFilterByRandom : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTFilterByRandom, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    API_FIELD(Attributes = "EditorOrder(10)")
    float MinValue;

    API_FIELD(Attributes = "EditorOrder(20)")
    float MaxValue;

    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 4;
    }

public:
    // [PLCTNode]
    bool GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output) override;
};

/* Node Runtime Cache Types */
/* This allows nodes to save their output once it runs. */

/// <summary>
/// Single surface list output cache.
/// </summary>
API_STRUCT() struct FLAXENGINE_API Arch0RuntimeCache
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(Arch0RuntimeCache)

    PLCTSurfaceList* SurfaceList;
};

/// <summary>
/// Single points output cache.
/// </summary>
API_STRUCT() struct FLAXENGINE_API Arch2RuntimeCache
{
    DECLARE_SCRIPTING_TYPE_MINIMAL(Arch2RuntimeCache)

    PLCTPointsContainer* Points;
};

