#pragma once
#ifndef CORE_TYPES_H
#define CORE_TYPES_H
//------------------------------------------------------------------------------
/**
    @class core/types.h

    Basic type definitions for Nebula3.
    
    (C) 2006 Radon Labs GmbH
*/
#include "memory/memory.h"
#include "utility/system.h"

//#include <vector>
#include <string>
#include <map>
#include <list>

// STL strings fully suffice for stellar' string implementation
using ::std::wstring;
using ::std::string;

// fixing Windows defines...
#ifdef DeleteFile
#undef DeleteFile
#endif
#ifdef CopyFile
#undef CopyFile
#endif
#ifdef GetObject
#undef GetObject
#endif

typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned int IndexT;    // the index type
typedef unsigned int SizeT;     // the size type
static const unsigned int InvalidIndex = 0xffffffff;

#define S_ARGB(a,r,g,b) ((uint)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define S_RGBA(r,g,b,a) S_ARGB(a,r,g,b)
#define S_XRGB(r,g,b)   S_ARGB(0xff,r,g,b)
#define S_COLORVALUE(r,g,b,a) S_RGBA((uint)((r)*255.f),(uint)((g)*255.f),(uint)((b)*255.f),(uint)((a)*255.f))

#if __WIN32__
#define s_stricmp stricmp
#define s_snprintf StringCchPrintf
#else
#error "Unsupported platform!"
#endif

#if __WIN32__
#define ThreadLocal __declspec(thread)
#else
#error "Unsupported platform!"
#endif

// alignment macro
#if __WIN32__
#define s_align(X)
//#define s_align(X) __declspec(align(X))
#else
#error "Unsupported platform!"
#endif

//------------------------------------------------------------------------------
#endif