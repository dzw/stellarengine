//------------------------------------------------------------------------------
//  shaderinstancebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/shader.h"
#include "coregraphics/shadervariation.h"
#include "coregraphics/shaderserver.h"

namespace Base
{
ImplementClass(Base::ShaderInstanceBase, 'SIBS', Core::RefCounted);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShaderInstanceBase::ShaderInstanceBase() :
    inBegin(false),
    inBeginPass(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShaderInstanceBase::~ShaderInstanceBase()
{
    // check if Discard() has been called...
    s_assert(!this->IsValid());
    s_assert(this->variables.IsEmpty());
    s_assert(this->variablesByName.IsEmpty());
    s_assert(this->variablesBySemantic.IsEmpty());
    s_assert(this->variations.IsEmpty());    
    s_assert(!this->activeVariation.isvalid());
    s_assert(this->preShaders.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderInstanceBase::IsValid() const
{
    return this->originalShader.isvalid();
}

//------------------------------------------------------------------------------
/**
    This method must be called when the object is no longer needed
    for proper cleanup.
*/
void
ShaderInstanceBase::Discard()
{
    s_assert(this->IsValid());
    this->originalShader->DiscardShaderInstance((ShaderInstance*)this);
}

//------------------------------------------------------------------------------
/**
    Override this method in an API-specific subclass to setup the
    shader instance, and call the parent class for proper setup.
*/
void
ShaderInstanceBase::Setup(const Ptr<Shader>& origShader)
{
    s_assert(!this->IsValid());
    this->originalShader = origShader;
}

//------------------------------------------------------------------------------
/**
    Override this method in an API-specific subclass to undo the
    setup in OnInstantiate(), then call parent class to finalize
    the cleanup.
*/
void
ShaderInstanceBase::Cleanup()
{
    s_assert(this->IsValid());
    this->originalShader = 0;
    this->variables.Clear();
    this->variablesByName.Clear();
    this->variablesBySemantic.Clear();
    this->variations.Clear();
    this->activeVariation = 0;
    /*IndexT i;
    for (i = 0; i < this->preShaders.Size(); i++)
    {
        this->preShaders[i]->OnDetach();
    }*/
    this->preShaders.Clear();
}

//------------------------------------------------------------------------------
/**
*/
//void
//ShaderInstanceBase::AddPreShader(const Ptr<PreShader>& preShader)
//{
//    preShader->OnAttach((ShaderInstance*)this);
//    this->preShaders.Append(preShader);
//}

//------------------------------------------------------------------------------
/**
*/
//void
//ShaderInstanceBase::RemovePreShader(const Ptr<PreShader>& preShader)
//{
//    IndexT index = this->preShaders.FindIndex(preShader);
//    s_assert(InvalidIndex != index);
//    preShader->OnDetach();
//    this->preShaders.EraseIndex(index);
//}

//------------------------------------------------------------------------------
/**
*/
SizeT
ShaderInstanceBase::Begin()
{
    s_assert(!this->inBegin);
    s_assert(!this->inBeginPass);
    this->inBegin = true;
    
    // invoke pre-shaders
    /*IndexT i;
    for (i = 0; i < this->preShaders.Size(); i++)
    {
        this->preShaders[i]->OnApply();
    }*/
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderInstanceBase::BeginPass(IndexT passIndex)
{
    s_assert(this->inBegin);
    s_assert(!this->inBeginPass);
    this->inBeginPass = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderInstanceBase::Commit()
{
    s_assert(this->inBeginPass);
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderInstanceBase::EndPass()
{
    s_assert(this->inBeginPass);
    this->inBeginPass = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderInstanceBase::End()
{
    s_assert(this->inBegin);
    s_assert(!this->inBeginPass);
    this->inBegin = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderInstanceBase::SelectActiveVariation(CoreGraphics::ShaderFeature::Mask featureMask)
{
    IndexT i = this->variations.FindIndex(featureMask);
    if (InvalidIndex != i)
    {
        const Ptr<ShaderVariation>& shdVar = this->variations.ValueAtIndex(i);
        if (shdVar != this->activeVariation)
        {
            this->activeVariation = shdVar;
            return true;
        }
    }
    else
    {
        s_error("Unknown shader variation '%s' in shader '%s'\n",
            ShaderServer::Instance()->FeatureMaskToString(featureMask).AsCharPtr(),
            this->originalShader->GetResourceId().Value().AsCharPtr());
    }
    return false;
}

} // namespace Base