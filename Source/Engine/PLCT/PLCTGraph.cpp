#include "PLCTGraph.h"
#include "Engine/Content/Factories/BinaryAssetFactory.h"

REGISTER_BINARY_ASSET(PLCTGraph, "FlaxEngine.PLCTGraph", false);

PLCTGraphNode::~PLCTGraphNode() {
    SAFE_DELETE(Instance);
}
