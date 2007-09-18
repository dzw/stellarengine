//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Persistable.h - Class for persistable (serializable) objects              //
// ### # #      ###                                                                            //
// # ### #      ###  Base class for persistent objects (which can be stored between            //
// #  ## #   # ## ## application sessions)                                                     //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_PERSISTABLE_H
#define NUCLEX_STORAGE_PERSISTABLE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Serializer.h"
#include "Nuclex/Support/String.h"
#include <typeinfo>

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::Persistable                                                               //
//  //
/// Base class for persistable objects
/**
*/
class Persistable {
  public:
    /// Destructor
    NUCLEX_API virtual ~Persistable() {}

  //
  // Persistable implementation
  //
  public:
    /// Load object from serializer
    /** Lets the object load its data from a serializer

        @param  spSerializer  Serializer to load from
    */
    NUCLEX_API virtual void load(const shared_ptr<Serializer> &spSerializer) = 0;

    /// Save object to serializer
    /** Saves the object into a serializer
         
        @param  spSerializer  Serializer to save to
    */
    NUCLEX_API virtual void save(const shared_ptr<Serializer> &spSerializer) const = 0;
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_PERSISTABLE_H
