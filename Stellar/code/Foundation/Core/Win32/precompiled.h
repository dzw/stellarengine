#pragma once
#ifndef CORE_WIN32_PRECOMPILED_H
#define CORE_WIN32_PRECOMPILED_H
//------------------------------------------------------------------------------
/**
    @file core/win32/precompiled.h
    
    Contains precompiled headers on the Win32 platform.
    
    (C) 2007 Radon Labs GmbH
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WIN32_WINNT 0x500

#define NOGDICAPMASKS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCTLMGR
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSERVICE
#define NOSOUND
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#define _CRT_SECURE_NO_WARNINGS

// Windows headers
#include <windows.h>
#include <process.h>
#include <shfolder.h>
#include <windows.h>
#include <strsafe.h>
#include <wininet.h>
#include <winsock2.h>
#include <rpc.h>
#include <dbghelp.h>
#include <intrin.h>

// crt headers
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>

// stl
#include <string>
#include <vector>
#include <map>

// DirectX headers
#include <d3d9.h>
#include <d3dx9.h>
#include <xinput.h>
//------------------------------------------------------------------------------
#endif


