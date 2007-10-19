//------------------------------------------------------------------------------
//  debug.cc
//  (C) 2002 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/types.h"
#include "utility/system.h"
#include <iostream>

//------------------------------------------------------------------------------
/**
*/
void s_format(char* buffer, size_t len, const char* fmt, va_list argList)
{
  #ifdef STELLAR_USING_VC80
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintf(buffer, len, fmt, argList);
    //vsprintf_s(acDst,uiDstSize,acFormat,acArgs);
  #else
    vsprintf(buffer,fmt,argList);
  #endif
}

//------------------------------------------------------------------------------
/**
    This function is called by s_assert() when the assertion fails.
*/
void 
s_barf(const char* exp, const char* file, int line)
{
    s_error("*** STELLAR ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
}

//------------------------------------------------------------------------------
/**
    This function is called by s_assert2() when the assertion fails.
*/
void
s_barf2(const char* exp, const char* msg, const char* file, int line)
{
    s_error("*** ATELLAR ASSERTION ***\nprogrammer says: %s\nexpression: %s\nfile: %s\nline: %d\n", msg, exp, file, line);
}

//------------------------------------------------------------------------------
/**
    This function is called when a serious situation is encountered which
    requires abortion of the application.
*/
void __cdecl
s_error(const char* msg, ...)
{
    char buf[4096];
    va_list argList;
    va_start(argList, msg);
    s_format(buf, sizeof(buf), msg, argList);

    std::string sOut(buf);
    std::clog << sOut << std::endl;

    #if defined(__WIN32__) && defined(STELLAR_DEBUG)
    s_strcat(buf, 4096, "\n");
    OutputDebugString(buf);
    #endif

    abort();
}

//------------------------------------------------------------------------------
/**
    This function is called when a warning should be issued which doesn't
    require abortion of the application.
*/
void __cdecl
s_warning(const char* msg, ...)
{
    char buf[4096];
    va_list argList;
    va_start(argList, msg);
    s_format(buf, sizeof(buf), msg, argList);

    std::string sOut(buf);
    std::clog << sOut << std::endl;

    #if defined(__WIN32__) && defined(STELLAR_DEBUG)
    s_strcat(buf, 4096, "\n");
    OutputDebugString(buf);
    #endif
}        

//------------------------------------------------------------------------------
/**
    Nebula's printf replacement. Will redirect the text to the console
    and/or logfile.

     - 27-Nov-98   floh    created
*/
void __cdecl
s_printf(const char *msg, ...)
{
    char buf[4096];
    va_list argList;
    va_start(argList, msg);
    s_format(buf, sizeof(buf), msg, argList);

    std::string sOut(buf);
    std::clog << buf << std::endl;
}

//------------------------------------------------------------------------------
/**
    Put the message to the debug window.

    - 26-Mar-05    kims    created
*/
void __cdecl
s_dbgout(const char *msg, ...)
{
    char buf[4096];
    va_list argList;
    va_start(argList, msg);
    s_format(buf, sizeof(buf), msg, argList);

    std::string sOut(buf);
    std::clog << sOut << std::endl;

    #if defined(__WIN32__) && defined(STELLAR_DEBUG)
    s_strcat(buf, 4096, "\n");
    OutputDebugString(buf);
    #endif
}

//------------------------------------------------------------------------------
/**
    Put process to sleep.

     - 21-Dec-98   floh    created
*/
void 
s_sleep(double sec)
{
  DWORD dwSec = (DWORD)(sec * 1000.0f);
  ::Sleep(dwSec);
}

