#include "stdneb.h"
#include "testFactory.h"
#include "core/factory.h"

namespace Test
{
    ImplementClass(Test::testFactory, 'TFac', Test::TestCase);

    //------------------------------------------------------------------------------
    /*
    */
    void testFactory::Run()
    {
        Ptr<testFactory> tFObj = testFactory::Create();
        Ptr<testFactory> tFObj1 = (testFactory*)Core::Factory::Instance()->Create("Test::testFactory");
        Ptr<testFactory> tFObj2 = (testFactory*)Core::Factory::Instance()->Create(Util::FourCC('TFac'));

        Verify(tFObj != 0 );
        Verify(tFObj1 != 0 );
        Verify(tFObj2 != 0 );
    }
};