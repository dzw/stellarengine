
#include "stdneb.h"
#include "testcase.h"

namespace Test
{
ImplementClass(TestCase, 'TSTC', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
TestCase::TestCase() :
    m_NumVerified(0),
    m_NumSucceeded(0),
    m_NumFailed(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestCase::~TestCase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Override this method in your derived class.
*/    
void
TestCase::Run()
{
    // empty
}

//------------------------------------------------------------------------------
/*
*/
void
TestCase::Verify(bool b)
{
    if (b)
    {
        s_printf("%s #%d: ok\n", this->GetClassName(), m_NumVerified);
        m_NumSucceeded++;
    }
    else
    {
        s_printf("%s #%d: FAILED\n", this->GetClassName(), m_NumVerified);
        m_NumFailed++;
    }
    m_NumVerified++;
}

};