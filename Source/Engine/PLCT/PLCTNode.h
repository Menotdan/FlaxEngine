#pragma once

#include "Engine/Scripting/SerializableScriptingObject.h"
#include "PLCTTypes.h"

/// <summary>
/// Base class for PLCT Graph nodes.
/// </summary>
API_CLASS(Abstract) class FLAXENGINE_API PLCTNode : public SerializableScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTNode, SerializableScriptingObject);
    friend class PLCTGraph;

public:
    /// <summary>
    /// Initializes node state. Called after whole graph is setup.
    /// </summary>
    /// <param name="graph">Node owner asset.</param>
    API_FUNCTION() virtual void Init(PLCTGraph* graph)
    {
    }

    API_PROPERTY() FORCE_INLINE virtual int NodeArchetypeIndex() const
    {
        return 0;
    }
};
