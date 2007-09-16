//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Thread.cpp - Thread                                 //
// ### # #      ###                                                      //
// # ### #      ###  A thread of execution, running in parallel          //
// #  ## #   # ## ## to other threads                                    //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Support/Thread.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Support;

// Thread kann sich selbst suspendieren ?
// stop() führt resume durch

#ifdef NUCLEX_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//  //
//  Nuclex::Thread::Impl                                                 //
//  //
struct Thread::Impl {
  public:
    /// Threaded function
    static DWORD WINAPI threadProc(void *pParameter) {
      Thread::Impl *pThis = reinterpret_cast<Thread::Impl *>(pParameter);
      
      pThis->m_spFunction->operator()();
      pThis->release();      
      
      return 0;
    }

    /// Constructor
    Impl(std::auto_ptr<Thread::Function> spFunction) :
      m_spFunction(spFunction),
      m_nRefCount(2) {

      DWORD ThreadID = 0;
      m_hThread = ::CreateThread(NULL, 0, threadProc, this, 0, &ThreadID);      
      if(!m_hThread)
        throw UnexpectedException("Nuclex::Support::Thread::Impl::Impl()",
                                  "CreateThread() failed. Too many threads ?");
    }
    ~Impl() {
      ::CloseHandle(m_hThread);
    }

    /// Releases the thread function
    /// @todo Interlocked or mutex required
    void release() {
      if(!--m_nRefCount)
        delete this;
    }

    /// Check whether the thread is still running
    bool isRunning() const {
      return m_nRefCount > 1;
    }
    
    void requestStop() {
      m_spFunction->requestStop();
    }
    
    void join() {
      if(isRunning())
        ::WaitForSingleObject(m_hThread, INFINITE);
    }

  private:
    HANDLE                          m_hThread;
    std::auto_ptr<Thread::Function> m_spFunction;
    size_t                          m_nRefCount;
};

#else
#error Not implemented yet
#endif // NUCLEX_WIN32

// ####################################################################### //
// # Nuclex::Thread::Thread()                                Constructor # //
// ####################################################################### //
/** Initializes the thread.

    @param  spThreadFunction  Function to execute in a thread
*/    
Thread::Thread(std::auto_ptr<Function> spThreadFunction) :
  m_pImpl(new Impl(spThreadFunction)) {}

// ####################################################################### //
// # Nuclex::Thread::~Thread()                                Destructor # //
// ####################################################################### //
Thread::~Thread() {
  m_pImpl->release();
}

// ####################################################################### //
// # Nuclex::Thread::requestStop()                                       # //
// ####################################################################### //
void Thread::requestStop() {
  m_pImpl->requestStop();
}

// ####################################################################### //
// # Nuclex::Thread::requestStop()                                       # //
// ####################################################################### //
bool Thread::isRunning() const {
  return m_pImpl->isRunning();
}

// ####################################################################### //
// # Nuclex::Thread::join()                                              # //
// ####################################################################### //
void Thread::join() {
  m_pImpl->join();
}
