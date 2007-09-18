//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## PersistableFactory.h - Factory for persistable objects                    //
// ### # #      ###                                                                            //
// # ### #      ###  Base class for factories on persistable objects                           //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_PERSISTABLEFACTORY_H
#define NUCLEX_STORAGE_PERSISTABLEFACTORY_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Persistable.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::PersistableFactory                                                        //
//  //
/// Factory for persistable objects
/**
*/
class PersistableFactory {
  public:
    /// Destructor
    NUCLEX_API virtual ~PersistableFactory() {}

  //
  // PersistableFactory implementation
  //
  public:
    /// Create instance of persistable class
    NUCLEX_API virtual Persistable *createInstance() = 0;
};

template<class PersistableClass>
class PersistableFactoryImpl<PersistableClass> :
  public PersistableFactory {
  
  public:
    /// Destructor
    NUCLEX_API virtual ~PersistableFactoryImpl() {}

  //
  // PersistableFactory implementation
  //
  public:
    /// Create instance of persistable class
    NUCLEX_API PersistableClass *createInstance() {
      return new PersistableClass();
    }
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_PERSISTABLEFACTORY_H
