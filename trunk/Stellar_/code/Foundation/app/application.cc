//------------------------------------------------------------------------------
//  application.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "app/application.h"
#include "core/sysfunc.h"

namespace App
{
ImplementSingleton(App::Application);

//------------------------------------------------------------------------------
/**
*/
Application::Application() :
    companyName("Radon Labs GmbH"),
    appName("N3 Application"),
    isOpen(false),
    returnCode(0)
{
    ConstructSingleton;
    Core::SysFunc::Setup();
}

//------------------------------------------------------------------------------
/**
*/
Application::~Application()
{
    s_assert(!this->isOpen);
}

//------------------------------------------------------------------------------
/**
*/
bool
Application::Open()
{
    s_assert(!this->isOpen);
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
Application::Close()
{
    s_assert(this->isOpen);
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
Application::Run()
{
    // empty, override in subclass
}

//------------------------------------------------------------------------------
/**
    This method must be called right before the main() function's end.
    It will properly cleanup the Nebula3 runtime, its static objects,
    private heaps and finally produce a refcount leak and mem leak report
    (debug builds only).
*/
void
Application::Exit()
{
    Core::SysFunc::Exit(this->returnCode);
}

} // namespace App