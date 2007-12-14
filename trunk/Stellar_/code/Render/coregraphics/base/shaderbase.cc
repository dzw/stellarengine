//------------------------------------------------------------------------------
//  shaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaderbase.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/shader.h"

namespace Base
{
ImplementClass(Base::ShaderBase, 'SHDB', Resources::Resource);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShaderBase::ShaderBase() 
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderBase::~ShaderBase()
{
    s_assert(0 == this->shaderInstances.Size());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderInstance>
ShaderBase::CreateShaderInstance()
{
    Ptr<ShaderInstance> newInst = ShaderInstance::Create();
    Ptr<ShaderBase> thisPtr(this);
    newInst->Setup(thisPtr.downcast<Shader>());	// 加载并解析FX文件（直接调用平台相关API）
    this->shaderInstances.push_back(newInst);
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderBase::DiscardShaderInstance(const Ptr<ShaderInstance>& inst)
{
    inst->Cleanup();
    IndexT i = this->shaderInstances.find(inst);
    s_assert(InvalidIndex != i);
    this->shaderInstances.erase(i);
}

} // namespace Base