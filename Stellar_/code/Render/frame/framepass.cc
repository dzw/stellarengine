//------------------------------------------------------------------------------
//  framepass.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framepass.h"
#include "coregraphics/renderdevice.h"

namespace Frame
{
ImplementClass(Frame::FramePass, 'FPSS', Core::RefCounted);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePass::FramePass() :
    clearColor(0.0f, 0.0f, 0.0f, 0.0f),
    clearDepth(1.0f),
    clearStencil(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePass::~FramePass()
{
    // make sure Discard() has been called
    s_assert(!this->shader.isvalid());
    s_assert(!this->renderTarget.isvalid());
    s_assert(this->shaderVariables.empty());
    s_assert(this->batches.empty());
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    if (this->renderTarget.isvalid())
    {
        this->renderTarget = 0;
    }
    this->shaderVariables.clear();
    IndexT i;
    for (i = 0; i < this->batches.size(); i++)
    {
        this->batches[i]->Discard();
    }
    this->batches.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Render()
{
    s_assert(this->renderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    // update render target
    this->renderTarget->SetClearColor(this->clearColor);
    this->renderTarget->SetClearDepth(this->clearDepth);
    this->renderTarget->SetClearStencil(this->clearStencil);

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render batches...
    renderDevice->BeginPass(this->renderTarget, this->shader);
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.size(); batchIndex++)
    {
        this->batches[batchIndex]->Render();
    }
    renderDevice->EndPass();
}

} // namespace Frame
