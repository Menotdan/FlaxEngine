#pragma once

#include "BoxVolume.h"
#include "Engine/PLCT/PLCTSurface.h"

API_CLASS(Attributes = "ActorContextMenu(\"New/Other/PLCT Volume\"), ActorToolbox(\"Other\")")
class FLAXENGINE_API PLCTVolume : public BoxVolume
{
    DECLARE_SCENE_OBJECT(PLCTVolume);

    API_FUNCTION() bool FindFirstSurface(PLCTSurface* surface);
};
