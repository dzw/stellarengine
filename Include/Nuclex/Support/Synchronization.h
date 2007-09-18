//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Synchronization.h - Thread synchronization          //
// ### # #      ###                                                      //
// # ### #      ###  Helper classes for thread synchronization           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_SYNCHRONIZATION_H
#define NUCLEX_SUPPORT_SYNCHRONIZATION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Mutex                                               //
//  //
/// Mutex
/** A mutex can only be entered by one thread at a time. If another
    thread tries to enter the mutex while it is used, it will be suspended
    until the mutex becomes free again.
*/
class Mutex {
  public:
    /// Scoped mutex lock
    /** Automatically enters and leaves a thread gate
    */
    struct ScopedLock {
      /// Constructor
      NUCLEX_API inline ScopedLock(Mutex &Mutex) :
        m_Mutex(Mutex) {
        m_Mutex.enter();
      }

      /// Destructor
      NUCLEX_API inline ~ScopedLock() {
        m_Mutex.leave();
      }

      private:
        Mutex &m_Mutex;                     ///< Acquired thread lock
    };
  
    /// Constructor
    NUCLEX_API inline Mutex();
    /// Destructor
    NUCLEX_API inline ~Mutex();

  //
  // Mutex implementation
  //
  public:
    /// Enter mutex
    NUCLEX_API inline void enter();
    /// Leave mutex
    NUCLEX_API inline void leave();

  private:
    #ifdef NUCLEX_WIN32
    CRITICAL_SECTION m_CriticalSection;               ///< Critical section
    #else
    #error Not implemented yet
    #endif
};

// ####################################################################### //
// # Nuclex::Mutex::Mutex()                                  Constructor # // 
// ####################################################################### //
inline Mutex::Mutex() {
#ifdef NUCLEX_WIN32
  ::InitializeCriticalSection(&m_CriticalSection);
#else
  #error Not implemented yet
#endif
}
    
// ####################################################################### //
// # Nuclex::Mutex::~Mutex()                                  Destructor # // 
// ####################################################################### //
inline Mutex::~Mutex() {
#ifdef NUCLEX_WIN32
  ::DeleteCriticalSection(&m_CriticalSection);
#else
  #error Not implemented yet
#endif
}

// ####################################################################### //
// # Nuclex::Mutex::enter()                                              # // 
// ####################################################################### //
inline void Mutex::enter() {
#ifdef NUCLEX_WIN32
  ::EnterCriticalSection(&m_CriticalSection);
#else
  #error Not implemented yet
#endif
}

// ####################################################################### //
// # Nuclex::Mutex::leave()                                              # // 
// ####################################################################### //
inline void Mutex::leave() {
#ifdef NUCLEX_WIN32
  ::LeaveCriticalSection(&m_CriticalSection);
#else
  #error Not implemented yet
#endif
}

//  //
//  Nuclex::Signal                                                       //
//  //
/// Thread Signal
class Signal {
  public:
    NUCLEX_API inline Signal(bool bAutoReset = true);
    NUCLEX_API inline ~Signal();
    
  //
  // Signal implementation
  //
  public:
    NUCLEX_API inline void wait();
    NUCLEX_API inline void set(bool bSignalled = true);
    
  private:
#ifdef NUCLEX_WIN32
    HANDLE m_hEvent;
#endif
    
};

// ####################################################################### //
// # Nuclex::Signal::Signal()                                Constructor # // 
// ####################################################################### //
inline Signal::Signal(bool bAutoReset) :
#ifdef NUCLEX_WIN32
  m_hEvent(::CreateEvent(NULL, !bAutoReset, false, NULL)) {}
#else
  #error Not implemented yet
#endif

// ####################################################################### //
// # Nuclex::Signal::~Signal()                                Destructor # // 
// ####################################################################### //
inline Signal::~Signal() {
#ifdef NUCLEX_WIN32
  ::CloseHandle(m_hEvent);
#endif
}

// ####################################################################### //
// # Nuclex::Signal::wait()                                              # // 
// ####################################################################### //
inline void Signal::wait() {
#ifdef NUCLEX_WIN32
  ::WaitForSingleObject(m_hEvent, INFINITE);
#else
  #error Not implemented yet  
#endif
}

// ####################################################################### //
// # Nuclex::Signal::set()                                               # // 
// ####################################################################### //
inline void Signal::set(bool bSignalled) {
#ifdef NUCLEX_WIN32
  if(bSignalled)
    ::SetEvent(m_hEvent);
  else
    ::ResetEvent(m_hEvent);
#else
  #error Not implemented yet  
#endif
}

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_SYNCHRONIZATION_H
