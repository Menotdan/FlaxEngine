#pragma once

#include "PLCTNode.h"
#include "Engine/Core/Collections/Array.h"

/// <summary>
/// Gets the first landscape found in the PLCT volume.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTGetTerrain : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTGetTerrain, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    // Random value thing
    API_FIELD(Attributes = "EditorOrder(10), Limit(0)")
    float Spacing = 3.0f;
};

/// <summary>
/// Samples a PLCT surface, generating points.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTSampleSurface : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTSampleSurface, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    API_PROPERTY() FORCE_INLINE int NodeArchetypeIndex() const override
    {
        return 2;
    }
};
