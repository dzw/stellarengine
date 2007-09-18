//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Main.h - Nuclex program entry wrapper                                     //
// ### # #      ###                                                                            //
// # ### #      ###  Provides a better alternative to the main() function                      //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEXMAIN_MAIN_H
#define NUCLEXMAIN_MAIN_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

namespace Nuclex {

//  //
//  Nuclex::Environment()                                                //
//  //
/// Environment information provider
/** Provides informations about the environment in which the
    application was launched (eg. which path, operating system)
*/
class Environment {
  public:
    /// Operating systems
    enum OperatingSystem {
      OS_UNKNWON = 0,                                 ///< Unknown OS
      OS_WIN32,                                       ///< Win32
      OS_LINUX                                        ///< Linux
    };

    /// Destructor
    /** Destroys an instance of CEnvironment
    */
    virtual ~Environment() {}

  //
  // Environment implementation
  //
  public:
    /// Display a message box
    virtual void showMessageBox(const string &sCaption, const string &sText) = 0;
    /// Display an error message with ok / cancel buttons
    virtual bool showErrorBox(const string &sCaption, const string &sText) = 0;

  
    /// Get command line arguments
    /** Returns the command line arguments with which the application
        was launched

        @return The command line string
    */
    virtual const string &getCommandLine() const = 0;

    /// Get launch path
    /** Retrieves the path from which the application was launched

        @return The launch path
    */
    virtual const string &getLaunchPath() const = 0;

    /// Get application path
    /** Retrieves the path in which the application's executable resides

        @return The application's path
    */
    virtual const string &getApplicationPath() const = 0;

    /// Get operating system
    /** Returns the operating system under which the application runs

        @return The current operating system
    */
    virtual OperatingSystem getOperatingSystem() const = 0;
    
    /// Get total amount of memory
    /** Returns the amount of memory installed on the system in megabyte

        @return The amount of memory
    */
    virtual size_t getMemorySize() const = 0;
};

} // namespace Nuclex

extern void NuclexMain(const Nuclex::Environment &Environment);

#endif // NUCLEXMAIN_MAIN_H
