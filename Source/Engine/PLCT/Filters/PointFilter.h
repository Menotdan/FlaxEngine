#pragma once

#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"
#include "../PLCTPoint.h"

/// <summary>
/// PLCT Point Filter.
/// </summary>
API_CLASS(Abstract) class FLAXENGINE_API PointFilter : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PointFilter, ScriptingObject);

public:
    API_FUNCTION() virtual bool FilterPoints(PLCTPointsContainer* points)
    {
        return false;
    }
};
