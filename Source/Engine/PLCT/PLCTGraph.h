#pragma once

#include "Engine/Content/BinaryAsset.h"
#include "Engine/Visject/VisjectGraph.h"
#include "PLCTNode.h"

class PLCTNode;

/// <summary>
/// PLCT Graph Node.
/// </summary>
class PLCTGraphNode : public VisjectGraphNode<>
{
public:
    // Instance of the graph node.
    PLCTNode* Instance = nullptr;

    ~PLCTGraphNode();
};

/// <summary>
/// PLCT (Procedural Level Creation Tools) graph
/// </summary>
class VisjectPLCTGraph :
    public VisjectGraph<PLCTGraphNode>
{
    
};

/// <summary>
/// PLCT Graph
/// </summary>
API_CLASS(NoSpawn, Sealed) class FLAXENGINE_API PLCTGraph : public BinaryAsset {
    DECLARE_BINARY_ASSET_HEADER(PLCTGraph, 1);
};
