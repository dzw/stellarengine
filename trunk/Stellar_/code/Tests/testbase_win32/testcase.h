#ifndef TEST_TESTCASE_H
#define TEST_TESTCASE_H

#include "core/refcounted.h"

namespace Test
{
class TestCase : public Core::RefCounted
{
    DeclareClass(TestCase);

public:
    TestCase();
    virtual ~TestCase();

    virtual void Run();
    void Verify(bool b);
    int GetNumSucceeded() const;
    int GetNumFailed() const;
    int GetNumVerified() const;

private:
    int m_NumVerified;
    int m_NumSucceeded;
    int m_NumFailed;
};

//------------------------------------------------------------------------------
/*
*/
inline
int 
TestCase::GetNumSucceeded() const
{
    return m_NumSucceeded;
}

//------------------------------------------------------------------------------
/*
*/
inline
int 
TestCase::GetNumFailed() const
{
    return m_NumFailed;
}

//------------------------------------------------------------------------------
/*
*/
inline
int
TestCase::GetNumVerified() const
{
    return m_NumVerified;
}

};

#endif