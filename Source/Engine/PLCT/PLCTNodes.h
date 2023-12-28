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
