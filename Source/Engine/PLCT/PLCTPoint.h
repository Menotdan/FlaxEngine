#pragma once

#include "PLCTProperties.h"

/// <summary>
/// PLCT object used to represent points.
/// </summary>
API_CLASS(NoSpawn) class FLAXENGINE_API PLCTPoint : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTPoint);

    PLCTPoint()
        : ScriptingObject(SpawnParams(Guid::New(), PLCTPoint::TypeInitializer))
    {}

public:
    /// <summary>
    /// Gets the properties of the point.
    /// </summary>
    /// <returns>The point properties.</returns>
    API_PROPERTY() PLCTPropertyStorage* GetProperties()
    {
        return &_properties;
    }

private:
    PLCTPropertyStorage _properties;
};

/// <summary>
/// PLCT points container, which holds an array of points and allows you to perform operations on them.
/// </summary>
API_CLASS(NoSpawn) class FLAXENGINE_API PLCTPointsContainer : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTPointsContainer);

public:
    /// <summary>
    /// Get the array of points in this container.
    /// </summary>
    /// <returns>The points array.</returns>
    API_PROPERTY() Array<PLCTPoint> GetPoints()
    {
        return _points;
    }

private:
    Array<PLCTPoint> _points;
};
