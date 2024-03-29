#pragma once
#ifndef BASE_SHADERVARAIBLEINSTANCEBASE_H
#define BASE_SHADERVARIABLEINSTANCEBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::ShaderVariableInstanceBase
    
    A ShaderVariableInstance associates a value with a ShaderVariable
    and can apply that value at any time to the ShaderVariable.
    Setting the value on a ShaderVariableInstance will just store the
    value but not change the actual ShaderVariable. Only calling
    Apply() will set the stored value on the ShaderVariable. 
    ShaderVariableInstance objects are used to manage per-instance
    state when rendering ModelNodeInstances.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "utility/variant.h"
#include "coregraphics/texture.h"

namespace CoreGraphics
{
    class ShaderVariable;
}

//------------------------------------------------------------------------------
namespace Base
{
class ShaderVariableInstanceBase : public Core::RefCounted
{
    DeclareClass(ShaderVariableInstanceBase);
public:
    /// constructor
    ShaderVariableInstanceBase();
    /// destructor
    virtual ~ShaderVariableInstanceBase();

    /// get the associated shader variable 
    const Ptr<CoreGraphics::ShaderVariable>& GetShaderVariable() const;
    /// apply local value to shader variable
    void Apply();

    /// set int value
    void SetInt(int value);
    /// set int array values
    void SetIntArray(const int* values, SizeT count);
    /// set float value
    void SetFloat(float value);
    /// set float array values
    void SetFloatArray(const float* values, SizeT count);
    /// set float4 value
    void SetVector(const Math::float4& value);
    /// set float4 array values
    void SetVectorArray(const Math::float4* values, SizeT count);
    /// set matrix44 value
    void SetMatrix(const Math::matrix44& value);
    /// set matrix44 array values
    void SetMatrixArray(const Math::matrix44* values, SizeT count);    
    /// set bool value
    void SetBool(bool value);
    /// set bool array values
    void SetBoolArray(const bool* values, SizeT count);
    /// set texture value
    void SetTexture(const Ptr<CoreGraphics::Texture>& value);

private:
    friend class ShaderVariableBase;
    
    /// setup the object
    void Setup(const Ptr<CoreGraphics::ShaderVariable>& var);

    Ptr<CoreGraphics::ShaderVariable> shaderVariable;
    Util::Variant value;        // for scalar values
    SizeT arrayCount;
    Util::Blob arrayValues;     // for array values
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariable>&
ShaderVariableInstanceBase::GetShaderVariable() const
{
    return this->shaderVariable;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetInt(int val)
{
    s_assert(this->value.GetType() == Util::Variant::Int);
    this->value = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetIntArray(const int* values, SizeT count)
{
    s_assert((this->value.GetType() == Util::Variant::Int) && (this->arrayCount == count));
    this->arrayValues.Set(values, count * sizeof(int));
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetFloat(float val)
{
    s_assert(this->value.GetType() == Util::Variant::Float);
    this->value = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetFloatArray(const float* values, SizeT count)
{
    s_assert((this->value.GetType() == Util::Variant::Float) && (this->arrayCount == count));
    this->arrayValues.Set(values, count * sizeof(float));
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetVector(const Math::float4& val)
{
    s_assert(this->value.GetType() == Util::Variant::Float4);
    this->value = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetVectorArray(const Math::float4* values, SizeT count)
{
    s_assert((this->value.GetType() == Util::Variant::Float4) && (this->arrayCount == count));
    this->arrayValues.Set(values, count * sizeof(Math::float4));
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetMatrix(const Math::matrix44& val)
{
    s_assert(this->value.GetType() == Util::Variant::Matrix44);
    this->value = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetMatrixArray(const Math::matrix44* values, SizeT count)
{
    s_assert((this->value.GetType() == Util::Variant::Matrix44) && (this->arrayCount == count));
    this->arrayValues.Set(values, count * sizeof(Math::matrix44));
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetBool(bool val)
{
    s_assert(this->value.GetType() == Util::Variant::Bool);
    this->value = val;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderVariableInstanceBase::SetTexture(const Ptr<CoreGraphics::Texture>& value)
{
    s_assert(this->value.GetType() == Util::Variant::Object);
    this->value.SetObject((Core::RefCounted*)value.get());
}

} // namespace Base
//------------------------------------------------------------------------------
#endif


