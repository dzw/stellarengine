//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Scriptlet.h - Script node                           //
// ### # #      ###                                                      //
// # ### #      ###  Node which encapsulates a script as node in         //
// #  ## #   # ## ## the hierarchical object tree                        //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#define NCX_SCRIPTLET_H

#ifndef NCX_SCRIPTLET_H
#define NCX_SCRIPTLET_H

#include "Nuclex/Config.h"
#include "Nuclex/Support/String.h"
//#include "Nuclex/Support/Class.h"

#include <map>

namespace Nuclex {

class Kernel;

//  //
//  Nuclex::Scriptlet                                                       //
//  //
///
/**
*/
class Scriptlet {
  public:
    /// Constructor
    NUCLEX_API Scriptlet(CKernel *pKernel);
    /// Destructor
    NUCLEX_API ~Scriptlet(void);

  //
  // IClass implementation
  //
  public:
    /// Get factory name
    const String &getFactory(void) const {
      static String sFactory("Scriptlet");
      return sFactory;
    }

  private:
    Kernel *m_pKernel;                               ///< Kernel pointer
};

//  //
//  Nuclex::ScriptletFactory                                                //
//  //
/// Scriptlet factory
/** Creates Scriptlets
*/
class ScriptletFactory {
  public:
    /// Constructor
    /** Initializes an instance of CScriptletFactory
    */
    NUCLEX_API ScriptletFactory(void) {
      return;
    }
    /// Destuctor
    /** Destroys an instance of CScriptletFactory
    */
    NUCLEX_API virtual ~ScriptletFactory(void) {
      return;
    }

  //
  // IClassFactory implementation
  //
  public:
    /// Create new Scriptlet
    /** Creates a new instance of a CScriptlet
     
        @param  Arguments  Arguments to be passed to the constructor
        @return The new scriptlet
    */
    Class *createInstance(const ArgumentList &Arguments) {
      switch(Arguments.getNumArguments()) {
        case 0: return new CScriptlet(NULL);
        default: throw 1; //CInvalidArgException("Nuclex::CScriptletFactory::createInstance()", 
                          //                  "Too many arguments");
      }
    }
};

} // namespace Nuclex

#endif NCX_SCRIPTLET_H
