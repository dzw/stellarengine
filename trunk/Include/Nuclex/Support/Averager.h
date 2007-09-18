//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Averager.cpp - Average calculator                   //
// ### # #      ###                                                      //
// # ### #      ###  Optimized calculation of averages                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_AVERAGER_H
#define NUCLEX_SUPPORT_AVERAGER_H

#include "Nuclex/Config.h"
#include <vector>

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Averager                                                                  //
//  //
template<typename T>
class Averager {
  public:
    /// Constructor
    /** Initializes an averager

        @param  SampleCount  Number of samples over which to average
    */
    Averager(size_t SampleCount) :
      m_Samples(SampleCount),
      m_Total(T()),
      m_NextSample(0),
      m_bFirstRun(true) {}

    /// Add new sample
    /** Add a sample into the averaging pool
            
        @param  Sample  Sample to add
    */
    void add(T Sample) {
      if(!m_bFirstRun)
        m_Total -= m_Samples[m_NextSample];

      m_Samples[m_NextSample] = Sample;
      m_Total += Sample;
        
      ++m_NextSample;
      if(m_NextSample >= m_Samples.size()) {
        m_NextSample = 0;
        m_bFirstRun = false;
      }
    }

    /// Get the current average
    /** Returns the calculated average of the samples within
        the sample count over which to average.
    
        @return The current average value
    */
    T getAverage() const {
      if(m_bFirstRun)
        return m_Total / m_NextSample;
      else
        return m_Total / m_Samples.size();
    }

    /// Reset averager
    /** Begins a new averaging session
    */
    void reset() {
      m_bFirstRun = true;
      m_NextSample = 0;
      m_Total = T();
    }

  private:
    typedef std::vector<T> SampleVector;

    SampleVector m_Samples;
    size_t       m_NextSample;
    T            m_Total;
    bool         m_bFirstRun;
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_AVERAGER_H