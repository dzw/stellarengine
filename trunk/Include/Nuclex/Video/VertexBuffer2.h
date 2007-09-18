//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexBuffer.h - Set of vertices                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Stores a set of custom-format vertices which can be rendered              //
// #  ## #   # ## ## in a single or in multiple steps                                          //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXBUFFER2_H
#define NUCLEX_VIDEO_VERTEXBUFFER2_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Support/Color.h"

/*

    +--------------------+          +--------------------------+
    |  BaseVertexBuffer  |--------->|  BaseVertexBuffer::Impl  |
    +---------.----------+          +------------.-------------+
             /_\                                /_\
              |                                  |
              |                                  |
  +----------------------------+    +----------------------------+
  |  VertexBuffer<VertexType>  |    |  Direct3DVertexBufferImpl  |
  +----------------------------+    +----------------------------+

*/

namespace Nuclex { namespace Video {

class VideoDevice;

//  //
//  Nuclex::Video::BaseVertexBuffer2                                                           //
//  //
/// Vertex buffer
/** Stores the data of vertices, required to render primitives.
    Each vertex represents a connection point of a polygon, the data
    associated with each of these points can be configured through a
    vertex declaration.
*/
class BaseVertexBuffer2 {
  friend VideoDevice;
  public:
    //typedef VertexType VertexType;

    /// Lock access mode
    enum LockMode {
      LM_READONLY = 1,                                ///< Read-only lock
      LM_WRITEONLY,                                   ///< Write-only lock
      LM_READWRITE,                                   ///< Read-and-write lock
      LM_OVERWRITEALL,                                ///< Write-everything lock
      LM_APPENDONLY,                                  ///< Write-without-overwriting lock
    };

    /// Constructor
    NUCLEX_API BaseVertexBuffer2(const VertexDeclaration &Declaration, size_t VertexCount) {}
    /// Destructor
    NUCLEX_API virtual ~BaseVertexBuffer2() {}

  //
  // VertexBuffer implementation
  //
  public:
    /// Get buffer size
    /** Retrieves the vertex buffer's size

        @return The vertex buffer's size
    */
    NUCLEX_API size_t getSize() const { return m_VertexCount; }

    /// Get buffer size
    /** Retrieves the vertex buffer's size

        @return The vertex buffer's size
    */
    NUCLEX_API const VertexDeclaration &getDeclaration() const;

  private:
    /// Lock vertex buffer
    /** Locks the vertex buffer

        @param  eLockMode  Locking mode (read/write)
        @return The vertex buffer pointer
    */
    NUCLEX_API virtual void *lock(LockMode eLockMode) = 0;

    /// Unlock vertex buffer
    /** Unlocks the vertex buffer after it has been locked using lock()
    */
    NUCLEX_API virtual void unlock() = 0;
    
    /// Number of vertices in the vertex buffer
    size_t m_VertexCount;
};

//  //
//  Nuclex::Video::VertexBuffer2                                                               //
//  //
template<typename VertexType>
class VertexBuffer2 : public BaseVertexBuffer2 {
  public:
    VertexBuffer2(size_t VertexCount) :
      BaseVertexBuffer2(VertexType::getDeclaration(), VertexCount) {}

  //
  // VertexBuffer2 implementation
  //
  public:
    /// Lock vertex buffer
    /** Locks the vertex buffer

        @param  eLockMode    Locking mode (read/write)
        @param  StartVertex  First vertex to be locked
        @param  VertexCount  Number of vertices to be locked
        @return The vertex buffer pointer
    */
    NUCLEX_API Accessor access(LockMode eLockMode, size_t StartVertex, size_t VertexCount);

    /// Lock vertex buffer
    /** Locks the vertex buffer

        @param  eLockMode  Locking mode (read/write)
        @return The vertex buffer pointer
    */
    NUCLEX_API Accessor access(LockMode eLockMode);
}

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXBUFFER_H
