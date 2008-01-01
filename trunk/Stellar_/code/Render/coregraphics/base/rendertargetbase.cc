//------------------------------------------------------------------------------
//  rendertargetbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/rendertargetbase.h"
#include "resources/sharedresourceserver.h"

namespace Base
{
ImplementClass(Base::RenderTargetBase, 'RTGB', Core::RefCounted);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::RenderTargetBase() :
    batchType(BatchType::InvalidBatchType),
    width(0),
    height(0),
    resolveTextureDimensionsValid(false),
    resolveTextureWidth(0),
    resolveTextureHeight(0),
    resolveRectValid(false),
    resolveRect(0, 0, 0, 0),
    clearColor(0.0f, 0.0f, 1.0f, 0.0f),
    clearDepth(1.0f),
    clearStencil(0),
    antiAliasQuality(AntiAliasQuality::None),
    numColorBuffers(0),
    mipMapsEnabled(false),
    isValid(false),    
    inBeginPass(false),
    inBeginBatch(false),
    hasDepthStencilBuffer(false),
    isDefaultRenderTarget(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderTargetBase::~RenderTargetBase()
{
    s_assert(!this->isValid);
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Setup()
{
    s_assert(!this->isValid);
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::Discard()
{
    s_assert(this->isValid);
    s_assert(!this->inBeginPass);
    s_assert(!this->inBeginBatch);
    if (this->resolveTexture.isvalid())
    {
        SharedResourceServer* resServer = SharedResourceServer::Instance();
        if (resServer->HasSharedResource(this->resolveTexture->GetResourceId()))
        {
            resServer->UnregisterSharedResource(this->resolveTexture->GetResourceId());
        }
        this->resolveTexture = 0;
    }
    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginPass()
{
    s_assert(this->isValid);
    s_assert(!this->inBeginPass);
    s_assert(!this->inBeginBatch);
    this->inBeginPass = true;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::BeginBatch(BatchType::Code t)
{
    s_assert(this->inBeginPass);
    s_assert(!this->inBeginBatch);
    this->inBeginBatch = true;
    this->batchType = t;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndBatch()
{
    s_assert(this->inBeginBatch);
    this->inBeginBatch = false; 
    this->batchType = BatchType::InvalidBatchType;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::EndPass()
{
    s_assert(this->isValid);
    s_assert(this->inBeginPass);
    s_assert(!this->inBeginBatch);
    this->inBeginPass = false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderTargetBase::GenerateMipLevels()
{
    s_assert(this->mipMapsEnabled);
}

} // namespace Base