//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## PythonLanguage.cpp - Nuclex Python language         //
// ### # #      ###                                                      //
// # ### #      ###  Python scripting language for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "PythonPlugin/Script/PythonLanguage.h"
#include "PythonPlugin/Script/PythonScript.h"

using namespace Nuclex;
using namespace Nuclex::Script;

// ####################################################################### //
// # Nuclex::PythonLanguage::PythonLanguage()                Constructor # // 
// ####################################################################### //
/** Initializes an instance of CPythonLanguage
*/
PythonLanguage::PythonLanguage() {}

// ####################################################################### //
// # Nuclex::PythonLanguage::~PythonLanguage()                Destructor # // 
// ####################################################################### //
/** Destroys an instance of CPythonLanguage
*/
PythonLanguage::~PythonLanguage() {}

// ####################################################################### //
// # Nuclex::PythonLanguage::createScript()                              # // 
// ####################################################################### //
/** Creates a new script of the implemented language

    @param  sScript  Initial script contents
*/
shared_ptr<Nuclex::Script::Script> PythonLanguage::createScript(const string &sScript) {
  return shared_ptr<Nuclex::Script::Script>(new PythonScript(sScript));
}
