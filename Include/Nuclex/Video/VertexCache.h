//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexCache.h - Vertex buffer primitive drawer                             //
// ### # #      ###                                                                            //
// # ### #      ###  Utility class for drawing 2D primitives using a vertex buffer             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXCACHE_H
#define NUCLEX_VIDEO_VERTEXCACHE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Video.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Video/VertexBuffer.h"
#include "Nuclex/Video/TextureCache.h"
#include "ScopeGuard/ScopeGuard.h"

namespace Nuclex { namespace Video {

// How large the vertex cache's internal vertex buffer will be
// Has to be at least as large as the vertex batch size, of which it should also be a multiple
const size_t VertexCacheBufferSize = 4 * VertexBatchSize;

//  //
//  Nuclex::Video::VertexCache                                                                 //
//  //
/// Vertex cache
/** Can be used to group small amounts of vertices which should be send to the graphics
    adapter in an effective manner. Use this class if you manage a lot of small objects
    consisting of only a few primitives.
    
    @param  VertexType  The kind of vertices to be managed in the vertex cache

    @todo Add support for multiple textures ?
          Probably not required and would also lead to a lot more management work due to
          texture combiners and shaders having to be taken care of here.
          
          Maybe multi texture support is sufficient alone, since you would rather sort by
          shader than by texture, so you would set up the right combiners/shaders, start a
          new vertex cache and render away ?
*/
template<typename VertexType>
class VertexCache {
  public:
    /// The type of vertices managed in the cache
    typedef VertexType VertexType;
  
    /// Constructor
    VertexCache(const shared_ptr<VideoDevice> &spVideoDevice);
    /// Destructor
    ~VertexCache();
      
  //
  // VertexCache implementation
  //
  public:
    /// Begins rendering
    void begin(const shared_ptr<VideoDevice::RenderingContext> &spRC);
    
    /// Adds primitives to the vertex cache
    void addPrimitives(const VertexType *pVertices, size_t VertexCount,
                       VideoDevice::RenderingContext::PrimitiveType ePrimitiveType,
                       const shared_ptr<Texture> &spTexture = shared_ptr<Texture>());

    /// Flushes the rendering queue
    void flush();

    /// Ends rendering
    void end();

  private:
    /// Stores a queued rendering operation
    struct RenderOperation {
      /// Initializes the rendering operation structure
      RenderOperation(VideoDevice::RenderingContext::PrimitiveType ePrimitiveType,
                      size_t StartVertex, const shared_ptr<Texture> &spTexture) :
        ePrimitiveType(ePrimitiveType),
        StartVertex(StartVertex),
        EndVertex(StartVertex),
        spTexture(spTexture) {}

      /// Which kind of primitive to draw in this operation
      VideoDevice::RenderingContext::PrimitiveType ePrimitiveType;
      /// Starting vertex for this operation in the vertex buffer
      size_t StartVertex;
      /// Ending vertex for this operation
      size_t EndVertex;
      /// Which texture to use
      shared_ptr<Texture> spTexture;
    };

    /// Vector of rendering operations
    typedef std::vector<RenderOperation> RenderOperationVector;
  
    /// Vertex buffer for rendering
    shared_ptr<VertexBuffer> m_spVertexBuffer;
    /// Index for next VB refill
    size_t m_VertexBufferStartIndex;
    /// The collected vertices to be filled into the vertex buffer
    VertexType m_pVertices[VertexBatchSize];
    /// List of queued render operations
    RenderOperationVector m_RenderOperations;
    /// Quick lookup for the current render operation
    RenderOperation *m_pCurrentRenderOperation;
    /// Current rendering context, valid between begin() and end() only
    shared_ptr<VideoDevice::RenderingContext> m_spCurrentRC;
};

// ############################################################################################# //
// # Nuclex::Video::VertexCache::VertexCache()                                     Constructor # //
// ############################################################################################# //
/** Initializes a new vertex cache

    @param  spVideoDevice  Video device for which vertices are to be cached
*/
template<typename VertexType>
VertexCache<VertexType>::VertexCache(const shared_ptr<VideoDevice> &spVideoDevice) :
  m_spVertexBuffer(spVideoDevice->createVertexBuffer(
    VertexType::getDeclaration(), VertexCacheBufferSize
  )),
  m_VertexBufferStartIndex(0) {}

// ############################################################################################# //
// # Nuclex::Video::VertexCache::~VertexCache()                                     Destructor # //
// ############################################################################################# //
/** Destroys a vertex cache, executing all queued rendering commands
*/
template<typename VertexType>
VertexCache<VertexType>::~VertexCache() {
  end();
}
  
// ############################################################################################# //
// # Nuclex::Video::VertexCache::begin()                                                       # //
// ############################################################################################# //
/** Call this method before you begin sending vertices to the cache. The vertex cache
    will be cleared and be ready to take on a new set of vertices
    
    @param  spRC  Rendering context onto which to draw
*/
template<typename VertexType>
void VertexCache<VertexType>::begin(const shared_ptr<VideoDevice::RenderingContext> &spRC) {
  if(m_spCurrentRC)
    throw FailedException("Nuclex::Video::VertexCache::begin()",
                          "end() was not called properly");
                          
  m_spCurrentRC = spRC;
  m_RenderOperations.clear();
  m_RenderOperations.push_back(RenderOperation(
    VideoDevice::RenderingContext::PT_NONE, 0, shared_ptr<Texture>()
  ));
  m_pCurrentRenderOperation = &m_RenderOperations[0];
}

// ############################################################################################# //
// # Nuclex::Video::VertexCache::addPrimitives()                                               # //
// ############################################################################################# //
/** Adds primitives into the cache. Unlike direct rendering calls, you can add low amounts
    of vertices here without major speed losses. There is no size limit to the amount of vertices
    you can send in at a time, either. The vertex cache might or might not render the primitives
    you provide immediately. 
*/
template<typename VertexType>
void VertexCache<VertexType>::addPrimitives(
  const VertexType *pVertices, size_t VertexCount,
  VideoDevice::RenderingContext::PrimitiveType ePrimitiveType,
  const shared_ptr<Texture> &spTexture = shared_ptr<Texture>()
) {
  // Collect some data about the vertices the user is trying to render
  size_t NextVertex = 0;
  size_t VerticesPerPrimitive;
  switch(ePrimitiveType) {
    case VideoDevice::RenderingContext::PT_POINTLIST: {
      VerticesPerPrimitive = 1; break;
    }
    case VideoDevice::RenderingContext::PT_LINELIST: {
      VerticesPerPrimitive = 2; break;
    }
    case VideoDevice::RenderingContext::PT_TRIANGLELIST: {
      VerticesPerPrimitive = 3; break;
    }
    default: {
      throw InvalidArgumentException(
        "Nuclex::Video::VertexCache::addPrimitives()", 
        "Invalid primitive type. Only primitive lists are supported!"
      );
    }
  }

  // For as long as there are more vertices left than fit in the vertex buffer
  do {
    bool bNeedRender = false;
    
    // Calculate the number of vertices to process in this go
    size_t VerticesToProcess = VertexBatchSize - m_pCurrentRenderOperation->EndVertex;
    if(VertexCount < VerticesToProcess)
      VerticesToProcess = VertexCount;
    else
      bNeedRender = true;

    VerticesToProcess -= VerticesToProcess % VerticesPerPrimitive;
    VertexCount -= VerticesToProcess;

    // If the primitive type or texture changed since the last call, begin a new operation
    if((m_pCurrentRenderOperation->ePrimitiveType != ePrimitiveType) ||
       (m_pCurrentRenderOperation->spTexture != spTexture)) {
      m_RenderOperations.push_back(RenderOperation(
        ePrimitiveType, m_pCurrentRenderOperation->EndVertex, spTexture
      ));
      m_pCurrentRenderOperation = &m_RenderOperations.back();
    }
    
    std::memcpy(
      &m_pVertices[m_pCurrentRenderOperation->EndVertex],
      &pVertices[NextVertex],
      sizeof(VertexType) * VerticesToProcess
    );
    m_pCurrentRenderOperation->EndVertex += VerticesToProcess;
    NextVertex += VerticesToProcess;

    // If the vertex buffer is filled to its limits, send the batch to the video card
    if(bNeedRender)
      flush();

  } while(VertexCount >= VerticesPerPrimitive);
}

// ############################################################################################# //
// # Nuclex::Video::VertexCache::end()                                                         # //
// ############################################################################################# //
/** Flushes the currently queued primitives to the graphics adapter. Call this method to
    ensure all outstanding drawing commands are sent to the graphics adapter. Automatically
    called from within end() as well as the destructor of this class
*/
template<typename VertexType>
void VertexCache<VertexType>::flush() {
  // If there is anything to render at all...
  if(m_RenderOperations.size() > 1) {

    // Lock the vertex buffer
    void *pMemory = m_spVertexBuffer->lock(
      m_VertexBufferStartIndex == 0 ?
        VertexBuffer::LM_OVERWRITEALL :
        VertexBuffer::LM_APPENDONLY,
      m_VertexBufferStartIndex, VertexBatchSize
    );
    
    // Copy the cached vertices into the vertex buffer
    { ScopeGuard Unlock_VertexBuffer = MakeObjGuard(*m_spVertexBuffer.get(), &VertexBuffer::unlock);
      std::memcpy(pMemory, m_pVertices, m_pCurrentRenderOperation->EndVertex * sizeof(VertexType));
    }

    // Select the now filled vertex buffer as the first vertex stream
    m_spCurrentRC->setVertexBuffer(0, m_spVertexBuffer);

    // Now select the default shader for these vertices (REMOVE FOR MULTITEX-VERSION!)
    m_spCurrentRC->setVertexShader(VertexType::getShader());
    shared_ptr<Texture> spCurrentTexture;
    m_spCurrentRC->setTexture(0, spCurrentTexture);

    // Execute all queued rendering operations
    for(size_t OperationIndex = 1; OperationIndex < m_RenderOperations.size(); ++OperationIndex) {
      RenderOperation &CurrentOperation = m_RenderOperations[OperationIndex];
      
      // If this operation requires a different texture, select it
      if(CurrentOperation.spTexture != spCurrentTexture) {
        spCurrentTexture = CurrentOperation.spTexture;
        m_spCurrentRC->setTexture(0, spCurrentTexture);
      }

      size_t PrimitiveCount = CurrentOperation.EndVertex - CurrentOperation.StartVertex;
      switch(CurrentOperation.ePrimitiveType) {
        case VideoDevice::RenderingContext::PT_LINELIST: {
          PrimitiveCount /= 2; break;
        }
        case VideoDevice::RenderingContext::PT_TRIANGLELIST: {
          PrimitiveCount /= 3; break;
        }
      }

      // Now we render all primitives which belong to this operation in a single step
      m_spCurrentRC->drawPrimitive(
        CurrentOperation.ePrimitiveType,
        CurrentOperation.StartVertex + m_VertexBufferStartIndex,
        PrimitiveCount
      );
    }

    // The vertex buffer is filled incrementally
    m_VertexBufferStartIndex += VertexBatchSize;
    if(m_VertexBufferStartIndex + VertexBatchSize >= VertexCacheBufferSize)
      m_VertexBufferStartIndex = 0;

    m_RenderOperations.clear();
    m_RenderOperations.push_back(RenderOperation(
      VideoDevice::RenderingContext::PT_NONE, 0, shared_ptr<Texture>()
    ));
    m_pCurrentRenderOperation = &m_RenderOperations[0];
  }
}

// ############################################################################################# //
// # Nuclex::Video::VertexCache::end()                                                         # //
// ############################################################################################# //
/** Ends drawing. Should be called after all primitives have been added into the cache
*/
template<typename VertexType>
void VertexCache<VertexType>::end() {
  flush();
  
  m_spCurrentRC.reset();
}

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXCACHE_H
