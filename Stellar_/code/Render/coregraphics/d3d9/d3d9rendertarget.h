#pragma once
#ifndef DIRECT3D9_D3D9RENDERTARGET_H
#define DIRECT3D9_D3D9RENDERTARGET_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9RenderTarget
  
    D3D9 implementation of RenderTarget.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/rendertargetbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9RenderTarget : public Base::RenderTargetBase
{
    DeclareClass(D3D9RenderTarget);
public:
    /// constructor
    D3D9RenderTarget();
    /// destructor
    virtual ~D3D9RenderTarget();
    
    /// setup the render target object
    void Setup();
    /// discard the render target object
    void Discard();
    /// begin a render pass
    void BeginPass();
    /// end current render pass
    void EndPass();
    /// generate mipmap levels
    void GenerateMipLevels();

protected:
    friend class D3D9RenderDevice;

    /// setup compatible multisample type
    void SetupMultiSampleType();

    IDirect3DSurface9* d3d9RenderTarget;
    IDirect3DSurface9* d3d9DepthStencil;
    IDirect3DTexture9* d3d9ResolveTexture;
    D3DMULTISAMPLE_TYPE d3d9MultiSampleType;
    DWORD d3d9MultiSampleQuality;
    D3DFORMAT d3d9ColorBufferFormat;
    bool needsResolve;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
