//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## IndexBuffer.h - Index buffer                                              //
// ### # #      ###                                                                            //
// # ### #      ###  An index buffer defining the order in which to draw vertices              //
// #  ## #   # ## ## that are contained in a vertex buffer                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_INDEXBUFFER_H
#define NUCLEX_VIDEO_INDEXBUFFER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Support/Color.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::IndexBuffer                                                                 //
//  //
/// Index buffer
/** An index buffer defining the order in which the vertices of
    a vertex buffer will be processed
*/
class IndexBuffer {
  public:
    /// Locking mode flags
    enum LockMode {
      LM_NONE = 0,                                    ///< No lock
      LM_READONLY,                                    ///< Read access
      LM_OVERWRITEALL,                                ///< Write access, overwrite all
      LM_APPENDONLY                                   ///< Write access, overwrite nothing
    };

    /// Destructor
    NUCLEX_API virtual ~IndexBuffer() {}

  //
  // IndexBuffer implementation
  //
  public:
    /// Get buffer size
    /** Retrieves the Index buffer's size
     
        @return The Index buffer's size
    */
    NUCLEX_API virtual size_t getSize() const = 0;

    /// Lock Index buffer
    /** Locks the Index buffer

        @param  eLockMode  Locking mode (read/write)
        @return The Index buffer pointer
    */
    NUCLEX_API virtual unsigned short *lock(LockMode eLockMode) = 0;
    /// Unlock Index buffer
    /** Unlocks the Index buffer after it has been locked using lock()
    */
    NUCLEX_API virtual void unlock() = 0;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_INDEXBUFFER_H
