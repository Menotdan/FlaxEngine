#pragma once

#include "PLCTProperties.h"

API_CLASS(NoSpawn) class FLAXENGINE_API PLCTPoint : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTPoint);
};

API_CLASS(NoSpawn) class FLAXENGINE_API PLCTPointsContainer : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTPointsContainer);

public:
    API_PROPERTY() Array<PLCTPoint> GetPoints()
    {
        return _points;
    }

private:
    Array<PLCTPoint> _points;
};
