#ifndef TEST_TESTFACTORY_H
#define TEST_TESTFACTORY_H

#include "../testbase_win32/testcase.h"

namespace Test
{
class testFactory : public Test::TestCase
{
    DeclareClass(testFactory);

public:
    virtual void Run();
};

};

#endif