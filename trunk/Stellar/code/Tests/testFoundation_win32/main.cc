
#include "../testbase_win32/testrunner.h"
#include "testFactory.h"

using namespace Test;

void main()
{
    Ptr<TestRunner> testRunner = TestRunner::Create();
    testRunner->AttachTestCase(testFactory::Create());

    testRunner->Run();
    getchar();
}