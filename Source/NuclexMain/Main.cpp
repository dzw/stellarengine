//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Main.cpp - Nuclex program entry wrapper                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Provides a better alternative to the main() function                      //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Nuclex.h"
#include "NuclexMain/Main.h"
#include "Nuclex/Kernel.h"
#include <typeinfo>
#include <iostream>

#ifdef NUCLEX_WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif NUCLEX_WIN32

using namespace Nuclex;

namespace {

// ####################################################################### //
// # autoLoadPlugIns()                                                   # // 
// ####################################################################### //
/*
    string sMessage = string(
      "An unhandled exception has caused the application to quit:\n") +
      typeid(Exception).name() + "\n" +
      "\n" +
      "Source of the error:\n" +
      Exception.getSource() + "\n" +
      "\n" +
      "Error description:\n" +
      Exception.what() + "\n"
    );
*/
/** Loads all plugins in the specified path

    @param  sPath  Path which to search for plugins
*/
void autoLoadPlugIns(const string &sPath) {
/*
#ifdef NUCLEX_WIN32
  HANDLE          hSearchHandle;
  WIN32_FIND_DATA m_FindFileData;

  string sDir(sPath);
  if(sDir.length() == 0)
    sDir = ".\\";
  else if(sDir.operator[](sDir.length() - 1) != '\\')
    sDir += "\\";

  #ifdef _DEBUG
  string sMask(sDir + "*-d.Plugin.dll");
  #else
  string sMask(sDir + "*.Plugin.dll");
  #endif
  
  hSearchHandle = ::FindFirstFile(sMask.c_str(), &m_FindFileData);
  if(hSearchHandle && hSearchHandle != INVALID_HANDLE_VALUE) {
    if(hSearchHandle) {
      int nResult;

      do {
        try {
#ifndef _DEBUG
          if(string(m_FindFileData.cFileName).find("-d.Plugin.dll") == string::npos)
#endif
          Kernel::loadPlugin(sDir + m_FindFileData.cFileName);
        }
        catch(const Plugin<Kernel>::CantLoadPluginException &) {
          // Ignore
        }
        nResult = ::FindNextFile(hSearchHandle, &m_FindFileData);
      } while(nResult && nResult != ERROR_NO_MORE_FILES);
    }

    ::FindClose(hSearchHandle);
  }
#endif // NUCLEX_WIN32
*/
}

} // namespace

//  //
//  Win32Environment                                                     //
//  //
/// Environment information provider
/** Default implementation of the evironmental information provider
*/
class Win32Environment : public Environment {
  public:
    /// Constructor
    /** Initializes an instance of Win32Environment

        @param  sCommandLine  Command line string
        @param  sPath         Application executable path
    */
    Win32Environment(const string &sLaunchPath) :
      m_sLaunchPath(sLaunchPath),
      m_sCommandLine(::GetCommandLine()) {

      MEMORYSTATUS MemoryStatus;
      ::GlobalMemoryStatus(&MemoryStatus);
      m_MemorySize = MemoryStatus.dwTotalPhys / 1048576;

      { char  pszPath[MAX_PATH];
        char *pszFilenameInPath = NULL;

        // Locate the application's directory
        ::GetModuleFileName(NULL, pszPath, sizeof(pszPath));
        ::GetFullPathName(pszPath, sizeof(pszPath), pszPath, &pszFilenameInPath);
        m_sApplicationPath = string(pszPath, pszFilenameInPath - pszPath);
      }
    }

    /// Destructor
    /** Destroys an instance of Win32Environment
    */
    virtual ~Win32Environment() {}

  //
  // Win32Environment implementation
  //
  public:
    /// Display a message box
    void showMessageBox(const string &sCaption, const string &sText) {
      MessageBox(NULL, sText.c_str(), sCaption.c_str(), MB_ICONINFORMATION | MB_OK);
    }
    /// Display an error message with ok / cancel buttons
    bool showErrorBox(const string &sCaption, const string &sText) {
      return MessageBox(
        NULL, sText.c_str(), sCaption.c_str(), MB_ICONERROR | MB_OKCANCEL
      ) == IDOK;
    }

    /// Get command line string
    const string &getCommandLine() const { return m_sCommandLine; }

    /// Get launch path
    const string &getLaunchPath() const { return m_sLaunchPath; }

    /// Get application path
    const string &getApplicationPath() const { return m_sApplicationPath; }
    
    /// Get operating system
    OperatingSystem getOperatingSystem() const { return OS_WIN32; }

    /// Get amount of available memory
    size_t getMemorySize() const { return m_MemorySize; }

  private:
    string m_sCommandLine;                           ///< Command line string
    string m_sLaunchPath;                            ///< Launch path
    string m_sApplicationPath;                       ///< Application path
    size_t m_MemorySize;                             ///< Amount of installed memory
};

// ####################################################################### //
// # main()                                                              # // 
// ####################################################################### //
/** Console application entry point

    @param  nArgC     Number of arguments
    @param  ppszArgV  Program path and arguments
    @return Zero on success
*/
int main(int nArgC, char *ppszArgV[]) {
  string sLaunchPath;
  
  { char  pszPath[MAX_PATH];
    char *pszFilenameInPath = NULL;

    ::GetCurrentDirectory(sizeof(pszPath), pszPath);
    sLaunchPath = pszPath;
  }

  #ifdef NUCLEX_WIN32  
  Win32Environment TheEnvironment(sLaunchPath);
  ::SetCurrentDirectory(TheEnvironment.getApplicationPath().c_str());
  #else
  #error Not implemented yet
  #endif

  try {
    Kernel::logMessage(Kernel::MT_INFO, "Entering main application.");
    NuclexMain(TheEnvironment);
  }
  catch(const Exception &Exception) {
    if(!TheEnvironment.showErrorBox(
      "Nuclex error report",
      string("An unhandled exception has caused the application to quit:\n") +
        typeid(Exception).name() + "\n" +
        "\n" +
        "Source of the error:\n" +
        Exception.getSource() + "\n" +
        "\n" +
        "Error description:\n" +
        Exception.what() + "\n" +
        "\n" +
        "Use cancel to let the exception pass to the debugger\n"
    ))
      throw;
  }

  Kernel::logMessage(Kernel::MT_INFO, "Clean exit.");

  return 0;
}


#ifdef NUCLEX_WIN32
// ####################################################################### //
// # WinMain()                                                           # // 
// ####################################################################### //
/** Win32 native program entry point

    @param  hInstance      Instance handle
    @param  hPrevInstance  Handle of previously active instance
    @param  pszCmdParam    Command line
    @param  nCmdShow       Initial window mode
    @return Zero on success
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR pszCmdParam, int nCmdShow) {
  return main(0, NULL);
}
#endif // NUCLEX_WIN32
