#ifndef TEST_TESTRUNNER_H
#define TEST_TESTRUNNER_H

#include "core/refcounted.h"
#include "core/ptr.h"
#include <vector>
#include "testcase.h"

namespace Test
{
    class TestRunner : public Core::RefCounted
    {
        DeclareClass(TestRunner);

    public:
        void AttachTestCase(TestCase* testCase);
        void Run();

    private:
        std::vector<Ptr<TestCase>> m_CaseList;
    };
};
#endif