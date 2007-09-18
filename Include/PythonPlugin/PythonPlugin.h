//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NuclexPython.h - Rar plugin implementation          //
// ### # #      ###                                                      //
// # ### #      ###  The plugin implementation for nuclex                //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_PYTHON_H
#define NUCLEX_PYTHON_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Kernel.h"


#include "Nuclex/Script/ScriptServer.h"
#include "Python/Python.h"

// The following block will decide whether symbols are imported from a
// dll (client app) or exported to a dll (library). The NUCLEX_EXPORTS symbol
// should only be used for compiling the nuclex library and nowhere else.
//
// If you dynamically link against nuclex, you should define NUCLEX_DLL for
// your project in order to import the dll symbols from nuclex.
//
#ifdef PYTHONPLUGIN_EXPORTS
  #define NUCLEXPYTHON_API __declspec(dllexport)
#else
  #define NUCLEXPYTHON_API __declspec(dllimport)
#endif

using namespace Nuclex;
using namespace Nuclex::Script;

//  //
//  Nuclex::PythonInitTerm                                               //
//  //
/// Python global initialization class
/** Initializes global DLL data. Currently includes loading D3D8.dll
    (which is required only once per process) and searching for the
    function pointer to create the main Python interface.
*/
class PythonInitTerm {
  public:
    /// Constructor
    /** Initializes global data when the dll enters a process
    */
    PythonInitTerm() {
      // Initialize the Python library
      Py_Initialize();
      
      Kernel::logMessage(
        Kernel::MT_INFO,
        "Python successfully initialized."
      );
    }

    /// Destructor
    /** Destroys global data when the dll leaves a process
    */
    ~PythonInitTerm() {
      Py_Finalize();
    }
} ;

DECLARATIONPACKAGE(PythonPlugin);

#endif // NUCLEX_PYTHON_H