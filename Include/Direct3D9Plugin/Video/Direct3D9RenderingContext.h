//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9VideoRenderer.h - Rendering context        //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9VIDEORENDERER_H
#define NUCLEX_VIDEO_DIRECT3D9VIDEORENDERER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderStates.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Video/Video.h"

namespace Nuclex { namespace Video {

class Direct3D9VideoDevice;
class Direct3D9Texture;
class Direct3D9VertexBuffer;
class Direct3D9IndexBuffer;
class Direct3D9VertexShader;
class Direct3D9RenderStates;

//  //
//  Nuclex::Video::Direct3D9RenderingContext                             //
//  //
/// NuclexGame renderer base class
/** 
*/
class Direct3D9RenderingContext :
  public VideoDevice::RenderingContext {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9RenderingContext(Direct3D9VideoDevice *pVideoDevice);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9RenderingContext();
    
  //
  // VideoRenderer implementation
  //
  public:
    /// Get current screen size
    NUCLEXDIRECT3D9_API const Point2<size_t> &getScreenSize() const;

    /// Access render states
    NUCLEXDIRECT3D9_API RenderStates &getRenderStates() { return *m_pActiveRenderStates; }

    /// Get view matrix
    NUCLEXDIRECT3D9_API const Matrix44<real> &getViewMatrix(void) const;
    /// Set view matrix
    NUCLEXDIRECT3D9_API void setViewMatrix(const Matrix44<real> &View);

    /// Get projection matrix
    NUCLEXDIRECT3D9_API const Matrix44<real> &getProjectionMatrix(void) const;
    /// Set projection matrix
    NUCLEXDIRECT3D9_API void setProjectionMatrix(const Matrix44<real> &Projection);

    /// Get world matrix
    NUCLEXDIRECT3D9_API const Matrix44<real> &getWorldMatrix() const;
    /// Set world matrix
    NUCLEXDIRECT3D9_API void setWorldMatrix(const Matrix44<real> &world);

    /// Get render states          
    NUCLEXDIRECT3D9_API const Direct3D9RenderStates &getRenderStates() const;

    /// Get current texture
    NUCLEXDIRECT3D9_API const shared_ptr<Texture> &getTexture(size_t nStage) const;
    /// Select texture
    NUCLEXDIRECT3D9_API void setTexture(size_t nStage, const shared_ptr<Texture> &spTexture);

    /// Retrieve current vertex buffer
    NUCLEXDIRECT3D9_API const shared_ptr<VertexBuffer> &getVertexBuffer(size_t nStream) const;
    /// Set vertex buffer
    NUCLEXDIRECT3D9_API void setVertexBuffer(size_t nStream, const shared_ptr<VertexBuffer> &spVertexBuffer);

    /// Retrieve current index buffer
    NUCLEXDIRECT3D9_API const shared_ptr<IndexBuffer> &getIndexBuffer() const;
    /// Set index buffer
    NUCLEXDIRECT3D9_API void setIndexBuffer(const shared_ptr<IndexBuffer> &spIndexBuffer);

    /// Retrieve current vertex shader
    NUCLEXDIRECT3D9_API const shared_ptr<VertexShader> &getVertexShader() const;
    /// Set vertex shader
    NUCLEXDIRECT3D9_API void setVertexShader(const shared_ptr<VertexShader> &VertexShader);

    /// Render primitive
    NUCLEXDIRECT3D9_API void drawPrimitive(PrimitiveType eType, size_t nStartVertex,
                                           size_t nNumPrimitives);

    /// Draw primitives from indexed vertex buffer
    NUCLEXDIRECT3D9_API void drawIndexedPrimitive(PrimitiveType eType, 
                                                  size_t nVertexCount,
                                                  size_t nFaceCount);

    /// Set scissor region for rendering
    NUCLEXDIRECT3D9_API void setScissorRegion(const Box2<size_t> &ScissorRegion);

/*
    /// Draw a screen-aligned line
    NUCLEXDIRECT3D9_API void drawLine(const Vector2<real> &Start, const Vector2<real> &End,
                                   const Color<float> &DrawingColor = Color<float>(1, 1, 1, 1)) {}

    /// Draws a screen-aligned box
    NUCLEXDIRECT3D9_API void drawBox(const Vector2<real> &Start, const Vector2<real> &End,
                                  const Color<float> &DrawingColor = Color<float>(1, 1, 1, 1)) {}

    /// Draws a screen-aligned filled box
    NUCLEXDIRECT3D9_API void drawFilledBox(const Vector2<real> &Start, const Vector2<real> &End,
                                        const Color<float> &DrawingColor = Color<float>(1, 1, 1, 1),
                                        const shared_ptr<Texture> &spTexture = shared_ptr<Texture>(),
                                        const Box2<float> &spTextureRegion = Box2<float>()) {}

    /// Draws screen-aligned text
    NUCLEXDIRECT3D9_API void drawText(const string &sText, const Vector2<real> &Position,
                                   const Color<float> &TextColor = Color<float>(1, 1, 1, 1),
                                   Font::Alignment eAlignment = Font::Alignment::A_NORMAL) {}
*/
  private:
    Direct3D9VideoDevice     *m_pVideoDevice;         ///< Video renderer
    IDirect3DDevice9         *m_pDirect3DDevice;      ///< Direct3D device
    Matrix44<real>            m_View;                 ///< View matrix
    Matrix44<real>            m_Projection;           ///< Projection matrix
    Matrix44<real>            m_World;                ///< World matrix
    Point2<size_t>            m_Size;                 ///< Rendering surface size

    shared_ptr<Texture>       m_pspTexture[MaxTextureStages]; ///< Current texture
    Direct3D9Texture         *m_ppTexture[MaxTextureStages]; ///< Current D3D9 texture
    shared_ptr<VertexBuffer>  m_pspVertexBuffer[MaxVertexStreams]; ///< Current vertex buffer
    Direct3D9VertexBuffer    *m_ppVertexBuffer[MaxVertexStreams]; ///< Current D3D9 vertex buffer
    shared_ptr<IndexBuffer>   m_spIndexBuffer;        ///< Current index buffer
    Direct3D9IndexBuffer     *m_pIndexBuffer;         ///< Current D3D9 index buffer
    shared_ptr<VertexShader>  m_spVertexShader;       ///< Current vertex shader
    Direct3D9VertexShader    *m_pVertexShader;        ///< Current D3D9 vertex shader

    RenderStates             *m_pActiveRenderStates;
/*
    unsigned long             m_nDrawPrimitiveCount;  ///< Number of DrawPrimitive() calls
    unsigned long             m_nSetTextureCount;
    unsigned long             m_nSetVertexBufferCount;
    unsigned long             m_nSetIndexBufferCount;
*/
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9VIDEORENDERER_H
