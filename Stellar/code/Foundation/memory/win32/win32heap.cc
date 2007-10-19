//------------------------------------------------------------------------------
//  win32heap.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/win32/win32heap.h"

namespace Win32
{
using namespace Threading;
//using namespace Util;

#if STELLAR_MEMORY_STATS
std::list<Win32Heap*>* Win32Heap::m_pList = 0;
CriticalSection* Win32Heap::m_pCriticalSection = 0;
#endif

//------------------------------------------------------------------------------
/**
    This method must be called at the beginning of the application because
    any threads are spawned (usually called by Util::Setup().
*/
void
Win32Heap::Setup()
{
    #if STELLAR_MEMORY_STATS
    s_assert(0 == m_pList);
    s_assert(0 == m_pCriticalSection);
    m_pList = s_new(std::list<Win32Heap*>);
    m_pCriticalSection = s_new(CriticalSection);
    #endif
}

//------------------------------------------------------------------------------
/**
*/
Win32Heap::Win32Heap(const char* heapName)
{
    s_assert(0 != heapName);
    this->name = heapName;
    this->heap = HeapCreate(0, 0, 0);
    s_assert(0 != this->heap);
    
    // enable low-fragmentatio-heap
    ULONG heapFragValue = 2;
    HeapSetInformation(this->heap, HeapCompatibilityInformation, &heapFragValue, sizeof(heapFragValue));

    // link into Heap list
    #if STELLAR_MEMORY_STATS
    s_assert(0 != m_pCriticalSection);
    this->allocCount = 0;
    this->allocSize  = 0;
    m_pCriticalSection->Enter();
    m_pList->push_back(this);
    this->listIterator = m_pList->end();
    --this->listIterator;
    m_pCriticalSection->Leave();
    #endif
}

//------------------------------------------------------------------------------
/**
*/
Win32Heap::~Win32Heap()
{
    BOOL success = HeapDestroy(this->heap);
    s_assert(0 != success);
    this->heap = 0;

    // unlink from Heap list
    #if STELLAR_MEMORY_STATS
    //s_assert(0 == this->allocCount);
    //s_assert(0 != m_pCriticalSection);
    //s_assert(0 != this->listIterator);
    //m_pCriticalSection->Enter();
    //m_pList->erase(this->listIterator);
    //m_pCriticalSection->Leave();
    //this->listIterator = 0;
    #endif   
}

#if STELLAR_MEMORY_STATS
//------------------------------------------------------------------------------
/**
    Validate the heap. This walks over the heap's memory block and checks
    the control structures. If somethings wrong the function will
    stop the program, otherwise the functions returns true.
*/
bool
Win32Heap::ValidateHeap() const
{
    BOOL result = HeapValidate(this->heap, 0, NULL);
    return (0 != result);
}


//------------------------------------------------------------------------------
/**
*/
int
Win32Heap::GetAllocCount() const
{
    return this->allocCount;
}

//------------------------------------------------------------------------------
/**
*/
int
Win32Heap::GetAllocSize() const
{
    return this->allocSize;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Win32Heap::Stats>
Win32Heap::GetAllHeapStats()
{
    s_assert(0 != m_pCriticalSection);
    std::vector<Stats> result;
    m_pCriticalSection->Enter();
    std::list<Win32Heap*>::iterator iter;
    for (iter = m_pList->begin(); iter != m_pList->end(); iter++)
    {
        Stats stats;
        stats.name       = (*iter)->GetName();
        stats.allocCount = (*iter)->GetAllocCount();
        stats.allocSize  = (*iter)->GetAllocSize();        
        result.push_back(stats);
    }
    m_pCriticalSection->Leave();
    return result;
}

//------------------------------------------------------------------------------
/**
    This static method calls the ValidateHeap() method on all heaps.
*/
bool
Win32Heap::ValidateAllHeaps()
{
    s_assert(0 != m_pCriticalSection);
    m_pCriticalSection->Enter();
    bool result = true;
    std::list<Win32Heap*>::iterator iter;
    for (iter = m_pList->begin(); iter != m_pList->end(); iter++)
    {
        result &= (*iter)->ValidateHeap();
    }
    m_pCriticalSection->Leave();
    return result;
}
#endif // STELLAR_MEMORY_STATS

} // namespace Memory
