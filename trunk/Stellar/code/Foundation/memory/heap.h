#pragma once
#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H
//------------------------------------------------------------------------------
/**
    @class Memory::Heap
  
    Implements a private heap.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/config.h"
#if __WIN32__
#include "memory/win32/win32heap.h"
namespace Memory
{
typedef Win32::Win32Heap Heap;
}
#else
#error "IMPLEMENT ME!"
#endif
#endif
