//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoDevice.h - Rendering device instance                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Represents an instance of a rendering device                              //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VIDEODEVICE_H
#define NUCLEX_VIDEO_VIDEODEVICE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VideoDriver.h"
#include "Nuclex/Video/VertexBuffer.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Math/Matrix44.h"
#include "Nuclex/Text/Font.h"
#include "Nuclex/Support/Cacheable.h"

namespace Nuclex { namespace Video {

class VertexBuffer;
class IndexBuffer;
class Texture;
class VertexShader;
class PixelShader;
class VertexDeclaration;
class RenderStates;
class RenderTarget;

//  //
//  Nuclex::Video::VideoDevice                                                                 //
//  //
/// Video renderer
/**
*/
class VideoDevice :
  public Cacheable {
  public:
    NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, InvalidVideoModeException);
  
    /// Resource usage classes
    enum ResourceClass {
      RC_TEMPORARY,                                   ///< Resource exists only for a short time
      RC_STATIC,                                      ///< Resource will not be modified
      RC_DYNAMIC                                      ///< Resource will be modified often
    };
    /// Alpha channel types
    enum AlphaChannel {
      AC_NONE = 0,                                    ///< No alpha channel required
      AC_KEY,                                         ///< At least 1 bit alpha channel required
      AC_BLEND                                        ///< Accurate alpha channel required
    };

    class RenderingContext;

    /// Destructor
    NUCLEX_API virtual ~VideoDevice() {}

    /// Predefined simple vertex
    struct SimpleVertex {
      Point3<float>::POD Position;                    ///< The vertex' location
      unsigned long      Color;                       ///< Color and opacity of the vertex
      Point2<float>::POD TexCoord;                    ///< Texture coordinates

      /// Get shader for rendering this type of vertex
      NUCLEX_API static const shared_ptr<VertexShader> &getShader();
      /// Get vertex declaration for this type of vertex
      NUCLEX_API static const VertexDeclaration &getDeclaration();
    };
    /// Predefined screen-coordinate vertex
    struct PretransformedVertex {
      Point3<float>::POD Position;                    ///< The vertex' location
      float              RHW;                         ///< RHW coordinate
      unsigned long      Color;                       ///< Color and opacity of the vertex
      unsigned long      Specular;                    ///< Highlight color
      Point2<float>::POD TexCoord;                    ///< Texture coordinates

      /// Get shader for rendering this type of vertex
      NUCLEX_API static const shared_ptr<VertexShader> &getShader();
      /// Get vertex declaration for this type of vertex
      NUCLEX_API static const VertexDeclaration &getDeclaration();
    };

  //
  // VideoDevice implementation
  //
  public:
    /// Retrieve the currently set display mode
    /** Returns the currently selected display mode

        @return  The current display mode
    */
    NUCLEX_API virtual const VideoDriver::DisplayMode &getDisplayMode() = 0;

    /// Select a display mode to use
    /** Selects a display mode either before the rendering output window
        is opened or tries to change the current display mode while the
        rendering output window is open.
        If the display mode does not specify a color depth (PF_NONE), the
        desktop color depth will be used for a rendering window instead
        of going fullscreen.

        @param  DisplayMode  Desired display mode
    */
    NUCLEX_API virtual void setDisplayMode(const VideoDriver::DisplayMode &DisplayMode) = 0;

    /// Render a frame
    /** Initiates the rendering of a new frame by returning a new renderer
        through which all drawing commands for the frame can be issued. The
        frame is completed by releasing the renderer.
        
        @return A new frame renderer
    */
    NUCLEX_API virtual shared_ptr<RenderingContext> renderFrame(
      const shared_ptr<RenderTarget> &spDestination = shared_ptr<RenderTarget>()
    ) = 0;
    
    /// Create a new texture
    /** Creates a new texture of the specified size and format on the
        device. The texture's contents are initially undefined.
        It is recommended to use texture sizes that are powers of 2,
        since, due to hardware limitations, in general, other sizes
        will waste memory up to the next power of 2.

        @param  Size            Resolution of the texture
        @param  eFormat         Pixel format of the texture
        @param  eResourceClass  Resource class in which to create the texture
        @return The new texture
    */
    NUCLEX_API virtual shared_ptr<Texture> createTexture(
      const Point2<size_t> &Size,
      Surface::PixelFormat eFormat,
      ResourceClass eResourceClass = RC_DYNAMIC
    ) = 0;

    /// Create a new texture
    /** Creates a new texture of the specified size and format on the
        device. The texture's contents are initially undefined.
        It is recommended to use texture sizes that are powers of 2,
        since, due to hardware limitations, in general, other sizes
        will waste memory up to the next power of 2.

        @param  Size            Resolution of the texture
        @param  eAlphaChannel   Required alpha channel type
        @param  eResourceClass  Resource class in which to create the texture
        @return The new texture
    */
    NUCLEX_API virtual shared_ptr<Texture> createTexture(
      const Point2<size_t> &Size,
      AlphaChannel eAlphaChannel,
      ResourceClass eResourceClass = RC_DYNAMIC
    ) = 0;

    /// Create a new vertex buffer
    /** Creates a new vertex buffer using the specified number of vertices
        and vertex format. The vertex buffer's contents are initially undefined
        and have to be filled before using it in a rendering operation.

        @param  Declaration     Vertex declaration describing the buffer's data format
        @param  nSize           Number of elements in the buffer
        @param  eResourceClass  Resource class in which to create the texture
        @return The new vertex buffer
    */
    NUCLEX_API virtual shared_ptr<VertexBuffer> createVertexBuffer(
      const VertexDeclaration &Declaration,
      size_t nSize,
      ResourceClass eResourceClass = RC_DYNAMIC
    ) = 0;

    /// Create a new index buffer
    /** Creates a new index buffer with the specified number of indices.
        The index buffer's contents are initially undefined and have to be
        filled before using it in a rendering operation.

        @param  nSize           Number of elements in the buffer
        @param  eResourceClass  Resource class in which to create the texture
        @return The new index buffer
    */
    NUCLEX_API virtual shared_ptr<IndexBuffer> createIndexBuffer(
      size_t nSize,
      ResourceClass eResourceClass = RC_DYNAMIC
    ) = 0;

    /// Create a new render state set
    /** Creates a new render state set using a state block or similar
        techniques where possible.

        @return The new render state set
    */
    NUCLEX_API virtual shared_ptr<RenderStates> createRenderStates() = 0;

    /// Create a new vertex shader
    /** Creates a new vertex shader
        
        @param  pnFunction  The compiled vertex shader function
        @return The new vertex shader
    */
    NUCLEX_API virtual shared_ptr<VertexShader> createVertexShader(
      const string &sShader
    ) = 0;

    /// Create a new pixel shader
    /** Creates a new pixel shader
        
        @param  pnFunction  The compiled pixel shader function
        @return The new pixel shader
    */
    NUCLEX_API virtual shared_ptr<PixelShader> createPixelShader(
      const string &sShader
    ) = 0;

    /// Retrieve the maximum texture size supported
    /** Returns the maximum texture size the hardware can support

        @return The maximum texture size
    */
    NUCLEX_API virtual const Point2<size_t> &getMaxTextureSize() const = 0;
    
    /// Retrieve the amount of video memory in megabytes
    /** Returns the amount of video memory rounded to the nearest megabyte
    
        @return The amount of video memory
    */
    NUCLEX_API virtual size_t getVideoMemorySize() const = 0;
    
    /// Set cursor
    /** Selects the hardware cursor to be displayed
    
        @param  spCursor  Cursor bitmap or NULL to disable the cursor
    */
    NUCLEX_API virtual void setCursor(const shared_ptr<Image> &spCursor) = 0;
};

//  //
//  Nuclex::Video::VideoDevice::RenderingContext                                               //
//  //
/// Video rendering context
/**
*/
class VideoDevice::RenderingContext {
  public:
    /// How to interpret vertices
    enum PrimitiveType {
      PT_NONE = 0,                                    ///< No primitive type
      PT_POINTLIST,                                   ///< Draw a point for each vertex
      PT_LINELIST,                                    ///< Connect vertices 1-2, 3-4 ... with lines
      PT_LINESTRIP,                                   ///< Connect vertices 1-2-3-4... with lines
      PT_TRIANGLELIST,                                ///< Draw a triangle from each three vertices
      PT_TRIANGLESTRIP,                               ///< Draw triangles of vertices 1-2-3, 3-4-5, ...
      PT_TRIANGLEFAN,                                 ///< Draw triangles of vetices 1-2-3, 1-3-4, ...
    };

    /// Destructor
    NUCLEX_API virtual ~RenderingContext() {}
    
  //
  // VideoRenderer implementation
  //
  public:
    /// Get current screen size
    /** Retrieves the size of the rendering output surface

        @return The rendering surface's size
    */
    NUCLEX_API virtual const Point2<size_t> &getScreenSize() const = 0;

    /// Access render states
    /** Provides access to the render states currently set on the renderr
    
        @return A set with the currently active render states
    */
    NUCLEX_API virtual RenderStates &getRenderStates() = 0;

    /// Get view matrix
    /** Retrieves the current view matrix

        @return The view matrix
    */
    NUCLEX_API virtual const Matrix44<real> &getViewMatrix() const = 0;
    /// Set view matrix
    /** Sets the view matrix

        @param  View  New view matrix
    */
    NUCLEX_API virtual void setViewMatrix(const Matrix44<real> &View) = 0;

    /// Get projection matrix
    /** Retrieves the current projection matrix

        @return The projection matrix
    */
    NUCLEX_API virtual const Matrix44<real> &getProjectionMatrix() const = 0;
    /// Set projection matrix
    /** Sets the projection matrix

        @param  Projection  New projection matrix
    */
    NUCLEX_API virtual void setProjectionMatrix(const Matrix44<real> &Projection) = 0;

    /// Get world matrix
    /** Retrieves the current world matrix

        @return The world matrix
    */
    NUCLEX_API virtual const Matrix44<real> &getWorldMatrix() const = 0;
    /// Set world matrix
    /** Sets the world matrix

        @param  World  New world matrix
    */
    NUCLEX_API virtual void setWorldMatrix(const Matrix44<real> &World) = 0;

    /// Get current texture
    NUCLEX_API virtual const shared_ptr<Texture> &getTexture(size_t nStage) const = 0;
    /// Select texture
    NUCLEX_API virtual void setTexture(size_t nStage, const shared_ptr<Texture> &Texture) = 0;

    /// Retrieve current vertex buffer
    /** Retrieves the currently set vertex buffer

        @param  nStream  Stream number
        @return The vertex buffer or NULL if unset
    */
    NUCLEX_API virtual const shared_ptr<VertexBuffer> &getVertexBuffer(size_t nStream) const = 0;
    /// Set vertex buffer
    /** Selects a vertex buffer

        @param  nStream         Stream number
        @param  spVertexBuffer  Vertex buffer to select
    */
    NUCLEX_API virtual void setVertexBuffer(size_t nStream, const shared_ptr<VertexBuffer> &spVertexBuffer) = 0;

    /// Retrieve current index buffer
    /** Retrieves the currently set index buffer

        @return The index buffer or NULL if unset
    */
    NUCLEX_API virtual const shared_ptr<IndexBuffer> &getIndexBuffer() const = 0;
    /// Set index buffer
    /** Selects a index buffer

        @param  spIndexBuffer  Index buffer to select
    */
    NUCLEX_API virtual void setIndexBuffer(const shared_ptr<IndexBuffer> &spIndexBuffer) = 0;

    /// Retrieve current vertex shader
    /** Retrieves the currently set vertex shader

        @return The vertex shader or NULL if unset
    */
    NUCLEX_API virtual const shared_ptr<VertexShader> &getVertexShader() const = 0;
    /// Set vertex shader
    /** Selects a vertex shader

        @param  spVertexShader  Vertex shader to select
    */
    NUCLEX_API virtual void setVertexShader(const shared_ptr<VertexShader> &spVertexShader) = 0;

    /// Draw primitives from vertex buffer
    /** Draws a primitive using a range of the vertices contained in
        the currently selected vertex buffer
        
        @param  eType           Type of primitive to draw
        @param  nStartVertex    First vertex to use for drawing
        @param  nNumPrimitives  Number of primitives to draw
    */
    NUCLEX_API virtual void drawPrimitive(PrimitiveType eType,
                                          size_t nStartVertex, size_t nNumPrimitives) = 0;
    
    /// Draw primitives from indexed vertex buffer
    /** Draws a primitive using a range of the vertices contained in
        the currently selected vertex buffer which are sepcified through
        the index buffer this is currently selected
        
        @param  eType         Type of primitive to draw
        @param  nVertexCount  First vertex to use for drawing ?
        @param  nFaceCount    Number of primitives to draw
    */
    NUCLEX_API virtual void drawIndexedPrimitive(PrimitiveType eType, 
                                                 size_t nVertexCount, size_t nFaceCount) = 0;

    /// Set scissor region for rendering
    /** Sets a region within which all rendering operations will be clipped
    
        @param  ScissorRegion  Scissoring region
    */
    NUCLEX_API virtual void setScissorRegion(const Box2<size_t> &ScissorRegion) = 0;

/* Would be useful if the gui should be able to render 3d objects at the same time

    /// Draw a screen-aligned line
    NUCLEX_API virtual void drawLine(const Point2<float> &Start,
                                     const Point2<float> &End,
                                     const Color<float> &LineColor = Color<float>(1, 1, 1, 1));

    /// Draws a screen-aligned box
    NUCLEX_API virtual void drawBox(const Point2<float> &Start,
                                    const Point2<float> &End,
                                    const Color<float> &BoxColor = Color<float>(1, 1, 1, 1)) {
      drawBox(Start, End, shared_ptr<Texture>(), Point2<float>(0), Point2<float>(0), BoxColor);
    }

    /// Draws a screen-aligned textured box
    NUCLEX_API virtual void drawBox(const Point2<float> &Start,
                                    const Point2<float> &End,
                                    const shared_ptr<Texture> &spTexture,
                                    const Point2<float> &TexStart,
                                    const Point2<float> &TexEnd,
                                    const Color<float> &BoxColor = Color<float>(1, 1, 1, 1)) = 0;

    /// Draws a screen-aligned bitmap
    NUCLEX_API virtual void drawImage(const Point2<float> &Start, const Point2<float> &End,
                                      const shared_ptr<Image> &spBitmap,
                                      const Color<float> &BoxColor = Color<float>(1, 1, 1, 1)) = 0;

    /// Draws screen-aligned text
    NUCLEX_API virtual void drawText(const shared_ptr<Font> &spFont, const String &sText,
                                     const Vector2<float> &Position,
                                     const Color<float> &TextColor = Color<float>(1, 1, 1, 1),
                                     Font::Alignment eAlignment = Font::Alignment::A_NORMAL) = 0;
*/
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VIDEODEVICE_H
