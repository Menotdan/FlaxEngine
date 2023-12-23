#pragma once

#include "PLCTProperties.h"
#include "PLCTPoint.h"
#include "Engine/Core/Config.h"
#include "Engine/Scripting/ScriptingObject.h"
#include "Engine/Scripting/ScriptingType.h"

/// <summary>
/// PLCT Surface class used for sampling points.
/// </summary>
API_CLASS(NoSpawn) class FLAXENGINE_API PLCTSurface : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTSurface, ScriptingObject);
};
