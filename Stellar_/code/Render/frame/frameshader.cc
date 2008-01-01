//------------------------------------------------------------------------------
//  frameshader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameshader.h"

namespace Frame
{
ImplementClass(Frame::FrameShader, 'FSHD', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
FrameShader::FrameShader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FrameShader::~FrameShader()
{
    // make sure Discard() has been called
    s_assert(!this->mainRenderTarget.isvalid());
    s_assert(this->renderTargets.IsEmpty());
    s_assert(this->shaderVariables.IsEmpty());
    s_assert(this->framePasses.IsEmpty());
    s_assert(this->postEffects.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShader::Discard()
{
    IndexT i;
    this->mainRenderTarget = 0;
    /*for (i = 0; i < this->renderTargets.Size(); i++)
    {
        this->renderTargets.ValueAtIndex(i)->Discard();
    }*/

	std::map<Resources::ResourceId, Ptr<CoreGraphics::RenderTarget>>::iterator itr;
	itr = this->renderTargets.GetMap().begin();
	for (;itr != this->renderTargets.GetMap().end(); itr++)
	{
		itr->second->Discard();
	}

    this->renderTargets.Clear();
    this->shaderVariables.Clear();
    for (i = 0; i < this->framePasses.Size(); i++)
    {
        this->framePasses[i]->Discard();
    }
    this->framePasses.Clear();
    for (i = 0; i < this->postEffects.Size(); i++)
    {
        this->postEffects[i]->Discard();
    }
    this->postEffects.Clear();

}

//------------------------------------------------------------------------------
/**
*/
void
FrameShader::Render()
{
    // render passes
    IndexT i;
    for (i = 0; i < this->framePasses.Size(); i++)
    {
        this->framePasses[i]->Render();
    }

    // render post effects
    for (i = 0; i < this->postEffects.Size(); i++)
    {
        this->postEffects[i]->Render();
    }
}

} // namespace Frame