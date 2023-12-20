#pragma once

#include "Engine/Scripting/SerializableScriptingObject.h"

/// <summary>
/// Base class for PLCT Graph nodes.
/// </summary>
API_CLASS(Abstract) class FLAXENGINE_API PLCTNode : public SerializableScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTNode, SerializableScriptingObject);

};
