#pragma once
#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H
//------------------------------------------------------------------------------
/**
    @file core/config.h

    Stellar compiler specific defines and configuration.
    
    (C) 2007 by Ctuo
*/

//------------------------------------------------------------------------------
/**
    configuration.
*/
#ifdef _DEBUG
#define STELLAR_DEBUG (1)
#endif

#ifdef STELLAR_DEBUG
#define STELLAR_MEMORY_STATS (1)
#else
#define STELLAR_MEMORY_STATS (0)
#endif

//------------------------------------------------------------------------------
/**
    Win32 specifics.
*/
#ifdef __WIN32__
#undef __WIN32__
#endif
#ifdef WIN32
#define __WIN32__ (1)
#endif

//------------------------------------------------------------------------------
/**
    GCC specifics.
*/
#ifndef __GNUC__
#  define  __attribute__(x)  /**/
#endif


#if defined(_MSC_VER)

// Microsoft Visual C++ specific pragmas.  MSVC6 is version 12.00, MSVC7.0 is
// version 13.00, and MSVC7.1 is version 13.10.  MSVC8.0 is version 14.00.
#if _MSC_VER < 1300
#define STELLAR_USING_VC6
#elif _MSC_VER < 1310
#define STELLAR_USING_VC70
#elif _MSC_VER < 1400
#define STELLAR_USING_VC71
#else
#define STELLAR_USING_VC80
#endif

#if defined(STELLAR_USING_VC6)

// Disable the warning "non dll-interface class FOO used as base for
// dll-interface class BAR."  These occur in the derivations
// class Binary2D : public ImageInt2D; class Binary3D : public ImageInt3D;
#pragma warning( disable : 4275 )

// Disable the warning about truncating the debug names to 255 characters.
// This warning shows up often with STL code in MSVC6, but not MSVC7.
#pragma warning( disable : 4786 )

// This warning is disabled because MSVC6 warns about not finding
// implementations for the pure virtual functions that occur in the template
// classes 'template <class Real>' when explicitly instantiating the classes.
// NOTE:  If you create your own template classes that will be explicitly
// instantiated, you should re-enable the warning to make sure that in fact
// all your member data and functions have been defined and implemented.
#pragma warning( disable : 4661 )

#endif

// The use of WM4_FOUNDATION_ITEM to export an entire class generates warnings
// when member data and functions involving templates or inlines occur.  To
// avoid the warning, WM4_FOUNDATION_ITEM can be applied only to those items
// that really need to be exported.
//#pragma warning( disable : 4251 ) 

#pragma warning( disable : 4995 )
#pragma warning( disable : 4819 )

typedef __int64 Integer64;

#endif

#endif