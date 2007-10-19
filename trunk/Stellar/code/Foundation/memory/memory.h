#pragma once
#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H
//------------------------------------------------------------------------------
/**
    @class Memory::Memory
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "memory/win32/win32memory.h"
#else
#error "IMPLEMENT ME!"
#endif
#endif
