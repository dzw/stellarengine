//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RenderStates.h - Rendering parameters                                     //
// ### # #      ###                                                                            //
// # ### #      ###  Additional parameters for adjusting rendering behavior                    //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_RENDERSTATES_H
#define NUCLEX_VIDEO_RENDERSTATES_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Video.h"
#include "Nuclex/Video/VideoDevice.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::RenderStates                                                                //
//  //
/// Set of render states
/** 
*/
class RenderStates { // NOT cacheable because mutatable
  public:
    /// Blending mode
    enum BlendMode {
      BM_ZERO = 0,                                    ///< Ignore blend channel
      BM_ONE,                                         ///< Use blend channel
      BM_SRCCOLOR,                                    ///< Blend by src color
      BM_INVSRCCOLOR,                                 ///< Blend by inverted src color
      BM_SRCALPHA,                                    ///< Blend by src alpha
      BM_INVSRCALPHA,                                 ///< Blend by inverted src alpha
      BM_DESTALPHA,                                   ///< Blend by dest alpha
      BM_INVDESTALPHA,                                ///< Blend by inverted dest alpha
      BM_DESTCOLOR,                                   ///< Blend by dest color
      BM_INVDESTCOLOR,                                ///< Blend by inverted dest color
      BM_SRCALPHASAT,                                 ///< Blend by saturated src alpha
      BM_BOTHSRCALPHA,                                ///< Blend by both src's alpha
      BM_BOTHINVSRCALPHA                              ///< Blend by both src's inverted alpha
    };    

    /// Pixel addressing mode
    enum AddressMode {
      F_WRAP,                                         ///< Wrap around
      F_MIRROR,                                       ///< Mirror 
      F_CLAMP,                                        ///< Clamp to 0..1
      F_MIRRORONCE                                    ///< Mirror once
    };
    
    /// How to cull
    enum CullMode {
      CM_OFF,                                         ///< Don't cull
      CM_CLOCKWISE,                                   ///< Cull polygons that are not drawn clockwise
      CM_COUNTERCLOCKWISE                             ///< Cull polygons that are drawn clockwise
    };

    /// How to fill polygons    
    enum FillMode {
      FM_POINT,                                       ///< Draw points at vertex locations
      FM_WIREFRAME,                                   ///< Connect vertices to wireframe
      FM_SOLID                                        ///< Completely fill polygons
    };

    /// Channel blending operation    
    enum BlendOperation {
      BO_NONE = 1,                                    ///< Don't use texture stage
      BO_ARGUMENT1,                                   ///< Directly set to argument 1
      BO_ARGUMENT2,                                   ///< Directly set to argument 2
      BO_MODULATE,                                    ///< Multiply arguments
      BO_MODULATE2X,                                  ///< Multiply and double
      BO_MODULATE4X                                   ///< Multiply and quadrify
    };
    
    /// Texture operation argument
    enum BlendArgument {
      BA_COLOROPERATION = 1,                          ///< ?
      BA_COLORARGUMENT1,                              ///< Use color from texture 1
      BA_COLORARGUMENT2,                              ///< Use color from texture 2
      BA_ALPHAOPERATION,                              ///< ?
      BA_ALPHAARGUMENT1,                              ///< Use alpha from texture 1
      BA_ALPHAARGUMENT2                               ///< Use alpha from texture 2
    };

    /// Texture filter type
    enum TextureFilter {
      TF_NONE = 0,                                    ///< No texture
      TF_NEAREST,                                     ///< Use nearest texel
      TF_LINEAR,                                      ///< Linearly interpolate
      TF_ANISOTROPIC                                  ///< Linear with anisotropic correction
    };

    /// Texture addressing mode
    enum TextureAddressMode {
      TAM_WRAP = 1,                                   ///< Wrap around (repeat)
      TAM_MIRROR,                                     ///< Mirror texture
      TAM_CLAMP,                                      ///< Clamp border pixel
      TAM_BORDER,                                     ///< Draw border
      TAM_MIRRORONCE                                  ///< Mirror texture once
    };

    /// Dimension
    enum Dimension {
      D_U,                                            ///< Texel X
      D_V,                                            ///< Texel Y
      D_W                                             ///< Texel Z (volume textures)
    };

    /// Destructor
    NUCLEX_API virtual ~RenderStates() {}
  
  //
  // RenderStates implementation
  //
  public:
    /// Remove all render states from the set
    NUCLEX_API virtual void clear();

    /// Integrate another state set
    NUCLEX_API virtual RenderStates &operator +=(const RenderStates &Other);
    NUCLEX_API RenderStates &operator +=(const shared_ptr<RenderStates> &Other) {
      return operator +=(*Other.get());
    }

    /// Check whether alpha blending is enabled
    NUCLEX_API bool alphaBlendEnabled() const { return m_bAlphaBlendEnabled; }
    /// Enable or disable alpha blending
    NUCLEX_API virtual void enableAlphaBlend(bool bEnable) { ++m_Changes; m_bAlphaBlendEnabled = bEnable; }

    /// Check whether lighting is enabled
    NUCLEX_API bool lightingEnabled() const { return m_bLightingEnabled; }
    /// Enable or disable lighting
    NUCLEX_API virtual void enableLighting(bool bEnable) { ++m_Changes; m_bLightingEnabled = bEnable; }

    /// Check if z buffering or depth sorting of some kind is active
    NUCLEX_API bool depthBufferEnabled() const { return m_bDepthBufferEnabled; }
    /// Enable z buffering or depth sorting
    NUCLEX_API virtual void enableDepthBuffer(bool bEnable) { ++m_Changes; m_bDepthBufferEnabled = bEnable; }
    
    /// Get blend mode to use for the source surface (eg. a texture to be rendered)
    NUCLEX_API BlendMode getSourceBlendMode() const { return m_eSourceBlendMode; }
    /// Set blend mode to use for the source surface (eg. a texture to be rendered)
    NUCLEX_API virtual void setSourceBlendMode(BlendMode eMode) { ++m_Changes; m_eSourceBlendMode = eMode; }
    
    /// Get blend mode to use for the destination surface (eg. a texture to be rendered)
    NUCLEX_API BlendMode getDestinationBlendMode() const { return m_eDestinationBlendMode; }
    /// Set blend mode to use for the destination surface (eg. a texture to be rendered)
    NUCLEX_API virtual void setDestinationBlendMode(BlendMode eMode) { ++m_Changes; m_eDestinationBlendMode = eMode; }
    
    /// Get culling mode
    NUCLEX_API CullMode getCullMode() const { return m_eCullMode; }
    /// Set culling mode
    NUCLEX_API virtual void setCullMode(CullMode eMode) { ++m_Changes; m_eCullMode = eMode; }

    /// Get polygon fill mode
    NUCLEX_API FillMode getFillMode() const { return m_eFillMode; }
    /// Set polygon fill mode
    NUCLEX_API virtual void setFillMode(FillMode eMode) { ++m_Changes; m_eFillMode = eMode; }

    /// Get color blend operation
    NUCLEX_API BlendOperation getColorOperation(size_t Stage) const { return m_peColorOperation[Stage]; }
    /// Set color blend operation
    NUCLEX_API virtual void setColorOperation(size_t Stage, BlendOperation eOperation) { ++m_Changes; m_peColorOperation[Stage] = eOperation; }

    /// Get color blend argument
    NUCLEX_API BlendArgument getColorArgument(size_t Stage, size_t Argument) const { return m_ppeColorArgument[Stage][Argument]; }
    /// Set color blend argument
    NUCLEX_API virtual void setColorArgument(size_t Stage, size_t Argument, BlendArgument eArgument) { ++m_Changes; m_ppeColorArgument[Stage][Argument] = eArgument; }

    /// Get alpha blend operation
    NUCLEX_API BlendOperation getAlphaOperation(size_t Stage) const { return m_peAlphaOperation[Stage]; }
    /// Set alpha blend operation
    NUCLEX_API virtual void setAlphaOperation(size_t Stage, BlendOperation eOperation) { ++m_Changes; m_peAlphaOperation[Stage] = eOperation; }

    /// Get alpha blend argument
    NUCLEX_API BlendArgument getAlphaArgument(size_t Stage, size_t Argument) const { return m_ppeAlphaArgument[Stage][Argument]; }
    /// Set alpha blend argument
    NUCLEX_API virtual void setAlphaArgument(size_t Stage, size_t Argument, BlendArgument eArgument) { ++m_Changes; m_ppeAlphaArgument[Stage][Argument] = eArgument; }

    /// Get minification filter type
    NUCLEX_API TextureFilter getMinificationFilter(size_t Stage) const { return m_peMinificationFilter[Stage]; }
    /// Set minification filter type
    NUCLEX_API virtual void setMinificationFilter(size_t Stage, TextureFilter eFilter) { ++m_Changes; m_peMinificationFilter[Stage] = eFilter; }

    /// Get magnification filter type
    NUCLEX_API TextureFilter getMagnificationFilter(size_t Stage) const { return m_peMagnificationFilter[Stage]; }
    /// Set magnification filter type
    NUCLEX_API virtual void setMagnificationFilter(size_t Stage, TextureFilter eFilter) { ++m_Changes; m_peMagnificationFilter[Stage] = eFilter; }

    /// Get texture addressing mode
    NUCLEX_API TextureAddressMode getTextureAddressMode(size_t Stage, Dimension eDimension) const { return m_ppeTextureAddressMode[Stage][eDimension]; }
    /// Set texture addressing mode
    NUCLEX_API virtual void setTextureAddressMode(size_t Stage, Dimension eDimension, TextureAddressMode eMode) { ++m_Changes; m_ppeTextureAddressMode[Stage][eDimension] = eMode; }

  protected:
    /// Returns whether any of the settings has been changed
    NUCLEX_API bool hasChanged() const { return (m_Changes > 0); }
    
    /// RenderState 
    template<typename VarType>
    struct RenderState {
      /// Status of the render state
      enum Status {
        S_UNSET,                                      ///< Not set (setting doesn't matter)
        S_SET,                                        ///< Set (changed, activation deferred)
        S_ACTIVE                                      ///< Active (written to hw or state block)
      };
      
      /// Constructor
      RenderState(VarType Value) :
        m_eStatus(S_UNSET),
        m_Value(Value) {}

      /// Another Constructor
      RenderState() :
        m_eStatus(S_UNSET) {}

      /// Mimics as its own type      
      operator VarType() const { return m_Value; }
      VarType operator =(VarType Value) {
        m_eStatus = S_SET;
        return m_Value = Value;
      }
      
      /// Get current status of the render state
      Status getStatus() const { return m_eStatus; }
      /// Set status of the render state
      void setStatus(Status eStatus) { m_eStatus = eStatus; }
      
      private:
        mutable Status m_eStatus;                     ///< The render states' status
        VarType m_Value;                              ///< Set value
    };

    size_t                          m_Changes;            ///< Number of changes
    RenderState<bool>               m_bAlphaBlendEnabled; ///< Alpha blending enabled ?
    RenderState<bool>               m_bLightingEnabled;   ///< Lighting enabled ?
    RenderState<bool>               m_bDepthBufferEnabled; ///< Z buffer/depth sorting enabled ?
    RenderState<BlendMode>          m_eSourceBlendMode;   ///< Selected source blend mode
    RenderState<BlendMode>          m_eDestinationBlendMode; ///< Selected destination blend mode
    RenderState<CullMode>           m_eCullMode;          ///< Culling mode
    RenderState<FillMode>           m_eFillMode;          ///< Fill mode
    RenderState<BlendOperation>     m_peColorOperation[MaxTextureStages]; ///< 
    RenderState<BlendArgument>      m_ppeColorArgument[MaxTextureStages][2];
    RenderState<BlendOperation>     m_peAlphaOperation[MaxTextureStages];
    RenderState<BlendArgument>      m_ppeAlphaArgument[MaxTextureStages][2];
    RenderState<TextureFilter>      m_peMinificationFilter[MaxTextureStages];
    RenderState<TextureFilter>      m_peMagnificationFilter[MaxTextureStages];
    RenderState<TextureAddressMode> m_ppeTextureAddressMode[MaxTextureStages][3];
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_RENDERSTATES_H
