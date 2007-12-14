#include "stdneb.h"
#include "testrunner.h"

namespace Test
{
    ImplementClass(TestRunner, 'TSTR', Core::RefCounted);

    void TestRunner::AttachTestCase(TestCase* testCase)
    {
        s_assert(testCase);
        m_CaseList.push_back(testCase);
    }

    void TestRunner::Run()
    {
        uint i;
        int numSucceeded = 0;
        int numFailed = 0;

        for ( i = 0; i < m_CaseList.size(); ++i )
        {
            TestCase* curTestCase = m_CaseList[i];
            
            s_printf("-> Running test: %s\n", curTestCase->GetClassName());

            curTestCase->Run();
            if (curTestCase->GetNumFailed() == 0)
            {
                s_printf("-> SUCCESS: %s runs %d tasks ok!\n", curTestCase->GetClassName(), curTestCase->GetNumSucceeded());
            }
            else
            {
                s_printf("-> FAILED: %d of %d tests failed in %s!\n",
                    curTestCase->GetNumFailed(),
                    curTestCase->GetNumVerified(),
                    curTestCase->GetClassName());
            }
            numFailed += curTestCase->GetNumFailed();
            numSucceeded += curTestCase->GetNumSucceeded();
            s_printf("\n");
        }
        s_printf("*Test RESULT: %d Succeeded, %d Failed!\n", numSucceeded, numFailed); 
    }

};