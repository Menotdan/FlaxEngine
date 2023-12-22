#include "PLCTVolume.h"
#include "Engine/Core/Log.h"

PLCTVolume::PLCTVolume(const SpawnParams& params)
    : BoxVolume(params)
{
    LOG(Warning, "Created new PLCT volume.");
}
