//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexBuffer.h - Set of vertices                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Stores a set of custom-format vertices which can be rendered              //
// #  ## #   # ## ## in a single or in multiple steps                                          //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXBUFFER_H
#define NUCLEX_VIDEO_VERTEXBUFFER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Support/Color.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::VertexBuffer                                                                //
//  //
/// Vertex buffer
/** Stores the data of vertices, required to render primitives.
    Each vertex represents a connection point of a polygon, the data
    associated with each of these points can be configured through a
    vertex declaration.
*/
class VertexBuffer {
  public:
    /// Lock access mode
    enum LockMode {
      LM_READONLY = 1,                                ///< Read-only lock
      LM_WRITEONLY,                                   ///< Write-only lock
      LM_READWRITE,                                   ///< Read-and-write lock
      LM_OVERWRITEALL,                                ///< Write-everything lock
      LM_APPENDONLY,                                  ///< Write-without-overwriting lock
    };

    template<typename VarType>
    class Accessor {
      public:
        /// Constructor
        /** Initializes an instance of Accessor
            
            @param  pMemory    Memory buffer on which to operate
            @param  nElements  Number of elements contained in the buffer
            @param  nPadding   Padding, in bytes, between two elements
        */
        inline Accessor(const shared_ptr<VertexBuffer> &spVB, LockMode eLockMode, size_t nPadding = 0) :
          m_spVB(spVB),
          m_pMemory(NULL),
          m_nElements(m_spVB->getSize()),
          m_nPadding(nPadding) {
          
          m_pMemory = m_spVB->lock(eLockMode);
        }
        
        inline ~Accessor() {
          if(m_pMemory)
            m_spVB->unlock();
        }

        inline VarType &operator [](size_t nIndex) {
          return *reinterpret_cast<VarType *>(
            static_cast<unsigned char *>(m_pMemory) + (nIndex * (sizeof(VarType) + m_nPadding))
          );
        }  
      
      private:
        shared_ptr<VertexBuffer>  m_spVB;             ///< Vertex buffer
        void                     *m_pMemory;          ///< External memory buffer
        size_t                    m_nElements;        ///< Number of elements
        size_t                    m_nPadding;         ///< Padding between two elements
    };
    
    /// Destructor
    NUCLEX_API virtual ~VertexBuffer() {}

  //
  // VertexBuffer implementation
  //
  public:
    /// Get buffer size
    /** Retrieves the vertex buffer's size

        @return The vertex buffer's size
    */
    NUCLEX_API virtual size_t getSize() const = 0;

    /// Lock vertex buffer
    /** Locks the vertex buffer

        @param  eLockMode    Locking mode (read/write)
        @param  StartVertex  First vertex to be locked
        @param  VertexCount  Number of vertices to be locked
        @return The vertex buffer pointer
    */
    NUCLEX_API virtual void *lock(LockMode eLockMode, size_t StartVertex, size_t VertexCount) = 0;

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
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXBUFFER_H
