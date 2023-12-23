#pragma once

#include "Engine/Core/Config.h"
#include "Engine/Core/Types/String.h"
#include "Engine/Core/Math/Quaternion.h"
#include "Engine/Core/Math/Transform.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Vector3.h"
#include "Engine/Core/Math/Vector4.h"
#include "Engine/Core/Types/Variant.h"
#include "Engine/Core/Collections/Array.h"
#include "Engine/Core/Log.h"
#include "Engine/Scripting/ScriptingType.h"
#include "Engine/Scripting/ScriptingObject.h"

/// <summary>
/// PLCT Property class. Represents one property of a PLCT object.
/// </summary>
API_CLASS(NoSpawn) class FLAXENGINE_API PLCTProperty : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTProperty);
    PLCTProperty()
        : ScriptingObject(SpawnParams(Guid::New(), PLCTProperty::TypeInitializer))
    {}

public:
    API_FIELD() String Name;
    API_FIELD() Variant Data;

    API_FUNCTION() FORCE_INLINE void SetValue(Variant value)
    {
        if (value.Type == VariantType::Null)
        {
            LOG(Warning, "PLCT Property '{0}' set as null! (Ignored)", Name);
            return;
        }

        if (!(Data.Type == VariantType::Null))
        {
            if (!(Data.Type == value.Type))
            {
                LOG(Warning, "PLCT Property '{0}' type overridden! (Ignored)", Name);
                return;
            }
        }

        Data = value;
    }

public:
    /// <summary>
    /// Gets a Uint64 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetUint64(API_PARAM(Out) uint64& output)
    {
        if (Data.Type == VariantType::Uint64)
        {
            output = Data.AsUint64;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets an Int64 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetInt64(API_PARAM(Out) int64& output)
    {
        if (Data.Type == VariantType::Int64)
        {
            output = Data.AsInt64;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets a Uint32 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetUint32(API_PARAM(Out) uint32& output)
    {
        if (Data.Type == VariantType::Uint)
        {
            output = Data.AsUint;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets an Int32 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetInt32(API_PARAM(Out) int32& output)
    {
        if (Data.Type == VariantType::Int)
        {
            output = Data.AsInt;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets a Uint16 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetUint16(API_PARAM(Out) uint16& output)
    {
        if (Data.Type == VariantType::Uint16)
        {
            output = Data.AsUint16;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets an Int16 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetInt16(API_PARAM(Out) int16& output)
    {
        if (Data.Type == VariantType::Int16)
        {
            output = Data.AsInt16;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets a Quaternion from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetQuaternion(API_PARAM(Out) Quaternion& output)
    {
        if (Data.Type == VariantType::Quaternion)
        {
            output = Data.AsQuaternion();;
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets a Vector4 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetVector4(API_PARAM(Out) Vector4& output)
    {
        if (Data.Type == VariantType::Vector4)
        {
            output = Data.AsVector4();
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets a Vector3 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetVector3(API_PARAM(Out) Vector3& output)
    {
        if (Data.Type == VariantType::Vector3)
        {
            output = Data.AsVector3();
            return true;
        }

        return false;
    }

    /// Gets a Vector2 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetVector2(API_PARAM(Out) Vector2& output)
    {
        if (Data.Type == VariantType::Vector2)
        {
            output = Data.AsVector2();
            return true;
        }

        return false;
    }

    /// Gets a Vector2 from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetVector2(API_PARAM(Out) Transform& output)
    {
        if (Data.Type == VariantType::Transform)
        {
            output = Data.AsTransform();
            return true;
        }

        return false;
    }

    /// Gets a String from this property.
    /// </summary>
    /// <param name="output">The output.</param>
    /// <returns>True if got the value, otherwise false.</returns>
    API_FUNCTION() FORCE_INLINE bool GetString(API_PARAM(Out) String& output)
    {
        if (Data.Type == VariantType::String)
        {
            output = Data.ToString();
            return true;
        }

        return false;
    }
};

/// <summary>
/// PLCT Property list storage. 
/// </summary>
API_CLASS(NoSpawn) class FLAXENGINE_API PLCTPropertyStorage : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE_NO_SPAWN(PLCTPropertyStorage);

public:
    API_FUNCTION() FORCE_INLINE PLCTProperty* GetProperty(String name)
    {
        PLCTProperty *result = nullptr;
        for (int i = 0; i < _properties.Count(); i++)
        {
            if (!_properties[i].Name.Compare(name))
            {
                result = &_properties[i];
                break;
            }
        }

        return result;
    }

    API_FUNCTION() FORCE_INLINE PLCTProperty* EnsureProperty(String name)
    {
        PLCTProperty* result = GetProperty(name);
        if (result != nullptr)
        {
            return result;
        }

        PLCTProperty newProperty = PLCTProperty();
        newProperty.Data = Variant(nullptr);
        newProperty.Name = name;
        _properties.Add(newProperty);

        return result;
    }

    API_FUNCTION() FORCE_INLINE Variant GetPropertyValue(String name)
    {
        PLCTProperty* property = GetProperty(name);
        if (property == nullptr)
        {
            return Variant(nullptr);
        }

        return property->Data;
    }

    API_FUNCTION() FORCE_INLINE bool SetPropertyValue(String name, Variant value)
    {
        PLCTProperty* property = GetProperty(name);
        if (property == nullptr)
        {
            return false;
        }

        property->SetValue(value);
        return true;
    }

private:
    Array<PLCTProperty> _properties;
};
