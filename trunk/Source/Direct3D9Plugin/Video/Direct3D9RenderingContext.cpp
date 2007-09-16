//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## VideoContext.cpp - Video render context             //
// ### # #      ###                                                      //
// # ### #      ###  Interface used by scene managers to draw            //
// #  ## #   # ## ## the scene                                           //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9RenderingContext.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Direct3D9Plugin/Video/Direct3D9Texture.h"
#include "Direct3D9Plugin/Video/Direct3D9VertexBuffer.h"
#include "Direct3D9Plugin/Video/Direct3D9IndexBuffer.h"
#include "Direct3D9Plugin/Video/Direct3D9VertexShader.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderStates.h"

// #include "MemoryTracker/DisableMemoryTracker.h"
#include "directx/d3dx9.h"
// #include "MemoryTracker/MemoryTracker.h"

using namespace Nuclex;
using namespace Nuclex::Video;

namespace {

// ####################################################################### //
// # d3DMatrixFromMatrix44()                                             # //
// ####################################################################### //
/// Build D3DMATRIX from nuclex Matrix44
/** Converts a nuclex Matrix44 into a Direct3D D3DMATRIX performing
    the exact same transformation
    
    @param  Matrix  Matrix to convert
    @return The converted matrix
*/
inline D3DMATRIX d3dMatrixFromMatrix44(const Matrix44<real> &Matrix) {
  D3DMATRIX D3DMatrix;
  for(size_t nY = 0; nY < 4; nY++)
    for(size_t nX = 0; nX < 4; nX++)
      D3DMatrix.m[nY][nX] = Matrix.M[nY][nX];
  
  return D3DMatrix;
}
 
} // namespace

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::Direct3D9RenderingContext()            # //
// ####################################################################### //
/** Initializes an instance of Direct3D9RenderingContext
*/
Direct3D9RenderingContext::Direct3D9RenderingContext(Direct3D9VideoDevice *pVideoDevice) :
  m_pVideoDevice(pVideoDevice),
  m_pDirect3DDevice(pVideoDevice->m_spDirect3DDevice),
  m_Size(m_pVideoDevice->m_DisplayMode.Resolution),
  m_pActiveRenderStates(m_pVideoDevice->m_spActiveRenderStates.get()) {

  memset(m_ppVertexBuffer, 0, sizeof(m_ppVertexBuffer));
  memset(m_ppTexture, 0, sizeof(m_ppTexture));

  D3DCheck(
    "Nuclex::Direct3D9RenderingContext::Direct3D9RenderingContext()",
    "IDirect3DDevice9::Clear()",
    m_pDirect3DDevice->Clear(
      0,
      NULL,
      D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
      0xFF7F7F7F, // 0xFF3F7F3F, // 0xFF7F7F7F,
      1.0f,
      0
    )
  );

  D3DCheck("Nuclex::Direct3D9RenderingContext::Direct3D9RenderingContext()",
           "IDirect3DDevice9::BeginScene()",
           m_pDirect3DDevice->BeginScene());

  // Set default perspective projection matrix
  setProjectionMatrix(Matrix44<real>::perspectiveProjection(
    0.6f, //0.8f,
    static_cast<float>(m_pVideoDevice->m_DisplayMode.Resolution.X) /
      m_pVideoDevice->m_DisplayMode.Resolution.Y,
    1.0f,
    1000.0f
  ));

/*
  // Set default perspective projection matrix
  setViewMatrix(Matrix44<real>::perspectiveProjection(
    0.8f,
    static_cast<float>(m_pVideoDevice->m_DisplayMode.Resolution.X) / m_pVideoDevice->m_DisplayMode.Resolution.Y,
    1.0f,
    1000.0f
  ));
  m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
*/  
}

// ############################################################################################# //
// # Nuclex::Direct3D9RenderingContext::~Direct3D9RenderingContext()                      Destructor # //
// ############################################################################################# //
/** Destroys an instance of Direct3D9RenderingContext
*/
Direct3D9RenderingContext::~Direct3D9RenderingContext() {
  D3DCheck("Nuclex::Direct3D9RenderingContext::~Direct3D9RenderingContext()",
           "IDirect3DDevice9::EndScene()",
           m_pDirect3DDevice->EndScene());

  HRESULT hResult = m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
  if(hResult == D3DERR_DEVICELOST)
    ; //m_pVideoDevice->notifyDeviceLost();
  else
    D3DCheck("Nuclex::Direct3D9RenderingContext::~Direct3D9RenderingContext()",
             "IDirect3DDevice9::Present()", hResult);
}

// ############################################################################################# //
// # Nuclex::Direct3D9RenderingContext::getScreenSize()                                           # //
// ############################################################################################# //
/** Retrieves the size of the rendering output surface

    @return The rendering surface's size
*/
const Point2<size_t> &Direct3D9RenderingContext::getScreenSize() const {
  return m_Size;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getTexture()                     # //
// ####################################################################### //
/** Retrieves the texture currently used

    @param  nStage  Stage of which to return the texture
    @return The current texture
*/
const shared_ptr<Texture> &Direct3D9RenderingContext::getTexture(size_t nStage) const {
  return m_pspTexture[nStage];
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setTexture()                     # //
// ####################################################################### //
/** Changes the texture to use for rendering

    @param  nStage     Stage of which to set the texture
    @param  spTexture  Texture to set
*/
void Direct3D9RenderingContext::setTexture(size_t nStage, const shared_ptr<Texture> &spTexture) {
  Texture *pTexture = spTexture.get();
  if(pTexture) {
    Direct3D9Texture *pD3D9Texture = dynamic_cast<Direct3D9Texture *>(pTexture);
    if(!pD3D9Texture)
      throw InvalidArgumentException("Nuclex::Direct3D9RenderingContext::setTexture()",
                                     "This is not a Direct3D texture");

    m_ppTexture[nStage] = pD3D9Texture;
    D3DCheck("Nuclex::Direct3D9RenderingContext::setTexture()",
             "IDirect3DDevice9::SetTexture()",
             m_pDirect3DDevice->SetTexture(nStage, pD3D9Texture->getDirect3DTexture()));

  } else {

    m_ppTexture[nStage] = NULL;
    D3DCheck("Nuclex::Direct3D9RenderingContext::setTexture()",
             "IDirect3DDevice9::SetTexture()",
             m_pDirect3DDevice->SetTexture(nStage, NULL));
  }

  m_pspTexture[nStage] = spTexture;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getViewMatrix()                  # //
// ####################################################################### //
/** Retrieves the ViewMatrix currently used

    @return The current view matrix
*/
const Matrix44<real> &Direct3D9RenderingContext::getViewMatrix() const {
  return m_View;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setViewMatrix()                  # //
// ####################################################################### //
/** Changes the view matrix to use for rendering
*/
void Direct3D9RenderingContext::setViewMatrix(const Matrix44<real> &View) {
  m_View = View;

  D3DMATRIX D3DMatrix = d3dMatrixFromMatrix44(View);
  D3DCheck("Direct3D9RenderingContext::setViewMatrix()", "IDirect3DDevice9::SetTransform()",
           m_pDirect3DDevice->SetTransform(D3DTS_VIEW, &D3DMatrix));
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getProjectionMatrix()            # //
// ####################################################################### //
/** Retrieves the projection matrix currently used

    @return The current projection matrix
*/
const Matrix44<real> &Direct3D9RenderingContext::getProjectionMatrix() const {
  return m_Projection;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setProjectionMatrix()            # //
// ####################################################################### //
/** Changes the projection matrix to use for rendering
*/
void Direct3D9RenderingContext::setProjectionMatrix(const Matrix44<real> &Projection) {
  m_Projection = Projection;

  D3DMATRIX D3DMatrix = d3dMatrixFromMatrix44(Projection);
  D3DCheck("Direct3D9RenderingContext::setProjectionMatrix()", "IDirect3DDevice9::SetTransform()",
           m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &D3DMatrix));
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getWorldMatrix()                 # //
// ####################################################################### //
/** Retrieves the world matrix currently used

    @return The current world matrix
*/
const Matrix44<real> &Direct3D9RenderingContext::getWorldMatrix() const {
  return m_World;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setWorldMatrix()                 # //
// ####################################################################### //
/** Changes the world matrix to use for rendering
*/
void Direct3D9RenderingContext::setWorldMatrix(const Matrix44<real> &World) {
  m_World = World;

  D3DMATRIX D3DMatrix = d3dMatrixFromMatrix44(World);
  D3DCheck("Direct3D9RenderingContext::setWorldMatrix()", "IDirect3DDevice9::SetTransform()",
           m_pDirect3DDevice->SetTransform(D3DTS_WORLD, &D3DMatrix));
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getVertexBuffer()                # //
// ####################################################################### //
/** Retrieves the VertexBuffer currently used

    @return The current VertexBuffer
*/
const shared_ptr<VertexBuffer> &Direct3D9RenderingContext::getVertexBuffer(size_t nStream) const {
  return m_pspVertexBuffer[nStream];
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setVertexBuffer()                # //
// ####################################################################### //
/** Changes the VertexBuffer to use for rendering
*/
void Direct3D9RenderingContext::setVertexBuffer(size_t nStream, const shared_ptr<VertexBuffer> &spVertexBuffer) {
  Direct3D9VertexBuffer *pD3D9VertexBuffer = dynamic_cast<Direct3D9VertexBuffer *>(spVertexBuffer.get());
  if(!pD3D9VertexBuffer)
    throw InvalidArgumentException("Nuclex::Direct3D9RenderingContext::setVertexBuffer()",
                                   "This is not a Direct3D VertexBuffer");

  D3DCheck("Nuclex::Direct3D9RenderingContext::setVertexBuffer()",
           "IDirect3DDevice9::SetStreamSource()",
           m_pDirect3DDevice->SetStreamSource(nStream, pD3D9VertexBuffer->getDirect3DVertexBuffer(), 0, pD3D9VertexBuffer->getStride()));

//  m_pVideoDevice->m_pDirect3DDevice->SetFVF(pD3D9VertexBuffer->getFormat());

  m_ppVertexBuffer[nStream] = pD3D9VertexBuffer;
  m_pspVertexBuffer[nStream] = spVertexBuffer;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getIndexBuffer()                 # //
// ####################################################################### //
/** Retrieves the IndexBuffer currently used

    @return The current IndexBuffer
*/
const shared_ptr<IndexBuffer> &Direct3D9RenderingContext::getIndexBuffer() const {
  return m_spIndexBuffer;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setIndexBuffer()                 # //
// ####################################################################### //
/** Changes the IndexBuffer to use for rendering
*/
void Direct3D9RenderingContext::setIndexBuffer(const shared_ptr<IndexBuffer> &spIndexBuffer) {
  Direct3D9IndexBuffer *pD3D9IndexBuffer = dynamic_cast<Direct3D9IndexBuffer *>(spIndexBuffer.get());
  if(!pD3D9IndexBuffer)
    throw InvalidArgumentException("Nuclex::Direct3D9RenderingContext::setIndexBuffer()",
                                   "This is not a Direct3D IndexBuffer");

  D3DCheck("Nuclex::Direct3D9RenderingContext::setIndexBuffer()",
           "IDirect3DDevice9::SetStreamSource()",
           m_pDirect3DDevice->SetIndices(pD3D9IndexBuffer->getDirect3DIndexBuffer()));

  m_pIndexBuffer = pD3D9IndexBuffer;
  m_spIndexBuffer = spIndexBuffer;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::drawPrimitive()                  # //
// ####################################################################### //
void Direct3D9RenderingContext::drawPrimitive(PrimitiveType eType, size_t nStartVertex,
                                              size_t nNumPrimitives) {
  D3DCheck("Nuclex::Direct3D9RenderingContext::drawPrimitive()",
           "IDirect3DDevice9::DrawPrimitive()",
           m_pDirect3DDevice->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(eType), nStartVertex, nNumPrimitives));
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::drawIndexedPrimitive()           # //
// ####################################################################### //
void Direct3D9RenderingContext::drawIndexedPrimitive(PrimitiveType eType, 
                                                     size_t nVertexCount,
                                                     size_t nFaceCount) {
  D3DCheck(
    "Nuclex::Direct3D9RenderingContext::drawPrimitive()",
    "IDirect3DDevice9::DrawPrimitive()",
    m_pDirect3DDevice->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(eType), 0, 0, nVertexCount, 0, nFaceCount));
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setScissorRegion()               # //
// ####################################################################### //
void Direct3D9RenderingContext::setScissorRegion(const Box2<size_t> &ScissorRegion) {
  RECT ScissorRect = {
    ScissorRegion.TL.X, ScissorRegion.TL.Y,
    ScissorRegion.BR.X, ScissorRegion.BR.Y
  };

  D3DCheck(
    "Nuclex::Direct3D9RenderingContext::setScissorRegion()",
    "IDirect3DDevice9::SetScissorRect()",
    m_pDirect3DDevice->SetScissorRect(&ScissorRect)
  );
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::getVertexShader()                # //
// ####################################################################### //
const shared_ptr<VertexShader> &Direct3D9RenderingContext::getVertexShader() const {
  return m_spVertexShader;
}

// ####################################################################### //
// # Nuclex::Direct3D9RenderingContext::setVertexShader()                # //
// ####################################################################### //
/** Selects the vertex shader to use

    @param  spVertexShader  The vertex shader
*/
void Direct3D9RenderingContext::setVertexShader(const shared_ptr<VertexShader> &spVertexShader) {
  m_pVertexShader = NULL;

  // Default shader for sample vertices requested ?
  if(spVertexShader == VideoDevice::SimpleVertex::getShader()) {
    D3DCheck("Nuclex::Direct3D9RenderingContext::setVertexShader()",
             "IDirect3DDevice9::SetVertexShader()",
             m_pDirect3DDevice->SetVertexShader(NULL));
    D3DCheck("Nuclex::Direct3D9RenderingContext::setVertexShader()",
             "IDirect3DDevice9::SetFVF()",
             m_pDirect3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1));
             
  // Default shader for pretransformed vertices requested ?
  } else if(spVertexShader == VideoDevice::PretransformedVertex::getShader()) {
    D3DCheck("Nuclex::Direct3D9RenderingContext::setVertexShader()",
             "IDirect3DDevice9::SetVertexShader()",
             m_pDirect3DDevice->SetVertexShader(NULL));
    D3DCheck("Nuclex::Direct3D9RenderingContext::setVertexShader()",
             "IDirect3DDevice9::SetFVF()",
             m_pDirect3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1));
             
  // User defined shader specified
  } else {
    Direct3D9VertexShader *pD3D9VertexShader = dynamic_cast<Direct3D9VertexShader *>(spVertexShader.get());
    if(!pD3D9VertexShader)
      throw InvalidArgumentException("Nuclex::Direct3D9RenderingContext::setVertexShader()",
                                     "This is not a Direct3D VertexShader");

    m_pVertexShader = pD3D9VertexShader;
  }

  m_spVertexShader = spVertexShader;
}
