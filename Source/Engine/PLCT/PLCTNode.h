#pragma once

#include "Engine/Scripting/SerializableScriptingObject.h"
#include "Engine/Visject/VisjectGraph.h"
#include "Engine/Core/Types/Variant.h"
#include "PLCTTypes.h"
#include "PLCTGraph.h"

class PLCTVolume;

/// <summary>
/// Base class for PLCT Graph nodes.
/// </summary>
API_CLASS(Abstract) class FLAXENGINE_API PLCTNode : public SerializableScriptingObject
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTNode, SerializableScriptingObject);
    friend class PLCTGraph;

public:
    ~PLCTNode() = default;

    virtual bool GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
    {
        output = Variant(nullptr);
        return false;
    }

    API_PROPERTY() FORCE_INLINE virtual int NodeArchetypeIndex() const
    {
        return 0;
    }

public:
    bool GetInputBox(VisjectGraphBox box, PLCTGraphNode*& outConnectedNode, VisjectGraphBox*& outConnectedBox)
    {
        if (!box.HasConnection())
            return false;

        outConnectedBox = box.FirstConnection();
        outConnectedNode = outConnectedBox->GetParent<PLCTGraphNode>();
        return true;
    }
};

API_CLASS(Abstract) class FLAXENGINE_API PLCTNodeEnd : public PLCTNode
{
    DECLARE_SCRIPTING_TYPE_WITH_CONSTRUCTOR_IMPL(PLCTNodeEnd, PLCTNode);

public:
    virtual bool Execute(PLCTGraphNode& node, PLCTVolume* volume)
    {
        return false;
    }
};
