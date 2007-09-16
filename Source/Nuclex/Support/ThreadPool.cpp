//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ThreadPool.cpp - Thread pool                        //
// ### # #      ###                                                      //
// # ### #      ###  A pool of worker threads for parallel processing    //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Support/ThreadPool.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Support;

namespace { void doNothing() {} }

// ####################################################################### //
// # Nuclex::ThreadPool::ThreadPool()                        Constructor # // 
// ####################################################################### //
/** Initializes a thread pool and starts the worker threads

    @param  NumThreads  Number of worker threads to use
*/
ThreadPool::ThreadPool(size_t NumThreads) :
  m_WorkerThreads(NumThreads) {
  
  // Create the threads and put them in sleep mode
  for(WorkerThreadVector::iterator It = m_WorkerThreads.begin();
      It != m_WorkerThreads.end();
      ++It) {
    It->second = new WorkerThread(*this);
    It->second->getSignal().set(false);
    It->first = shared_ptr<Thread>(new Thread(std::auto_ptr<Thread::Function>(It->second)));
  }
}

// ####################################################################### //
// # Nuclex::ThreadPool::~ThreadPool()                        Destructor # // 
// ####################################################################### //
/** Destroy the thread pool, stopping and joining all threads
    currently belonging to the pool.
*/    
ThreadPool::~ThreadPool() {
  // Request stop and wake up on all threads
  for(WorkerThreadVector::iterator It = m_WorkerThreads.begin();
      It != m_WorkerThreads.end();
      ++It) {
    It->first->requestStop();
    It->second->getSignal().set();
  }

  // Wait for the threads to finish
  for(WorkerThreadVector::iterator It = m_WorkerThreads.begin();
      It != m_WorkerThreads.end();
      ++It)
    It->first->join();
}

// ####################################################################### //
// # Nuclex::ThreadPool::enqueue()                                       # // 
// ####################################################################### //
/** Puts a new task in the queue to be worked on by a thread

    @param  spTask  Function to be queued for execution on a thread
*/
void ThreadPool::enqueue(std::auto_ptr<Thread::Function> spTask) {
  // Put the task in the queue
  { Mutex::ScopedLock TaskLock(m_TasksMutex);

    // Grab away the auto_ptr's object. It was only used to make it
    // clear to the user that the object will be deleted after completion
    m_Tasks.push_back(shared_ptr<Thread::Function>(spTask.release()));
  }
  
  // Look for a free thread to perform this task
  for(WorkerThreadVector::iterator It = m_WorkerThreads.begin();
      It != m_WorkerThreads.end();
      ++It) {
    // If the thread is free, wake it up and end the search
    if(!It->second->isWorking()) {
      It->second->getSignal().set();
      break;
    }
  }  
}

// ####################################################################### //
// # Nuclex::ThreadPool::WorkerThread::operator()                        # // 
// ####################################################################### //
/** The thread's main method. Takes an open task from the queue and
    executes it, if available, or goes to sleep.
*/
void ThreadPool::WorkerThread::operator()() {
  while(!m_bStopRequested) {
    shared_ptr<Thread::Function> spTask;
    bool                         bSleep;
    
    // Look for a new taks in the task queue
    { Mutex::ScopedLock TaskLock(m_Owner.m_TasksMutex);
      
      if(m_Owner.m_Tasks.size() > 0) {
        spTask = m_Owner.m_Tasks.front();
        m_Owner.m_Tasks.pop_front();
        bSleep = false;
      } else {
        bSleep = true;
      }
    }
    
    // Go to sleep if no tasks are available
    if(bSleep) {
      m_bWorking = false;
      m_Signal.set(false);
      m_Signal.wait();
    } else {
      spTask->operator()();
    }
  }
}

// ne kw hm, bt he ws ster tn te wd
// ...ad stth he wd nd, fr he ws up agst hs fl chge
