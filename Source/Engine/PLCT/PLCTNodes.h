#pragma once

#include "PLCTNode.h"
#include "Engine/Core/Collections/Array.h"

/// <summary>
/// Delay node that waits a specific amount of time while executed.
/// </summary>
API_CLASS(Sealed) class FLAXENGINE_API PLCTGetLandscape : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTGetLandscape, PLCTNode);
    API_AUTO_SERIALIZATION();

public:
    // Random value thing
    API_FIELD(Attributes = "EditorOrder(10), Limit(0)")
    float Spacing = 3.0f;
};
