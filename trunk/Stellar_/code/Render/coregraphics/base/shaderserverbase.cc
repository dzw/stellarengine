//------------------------------------------------------------------------------
//  shaderserverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaderserverbase.h"
#include "coregraphics/streamshaderloader.h"
#include "io/ioserver.h"

namespace Base
{
ImplementClass(Base::ShaderServerBase, 'SSRV', Core::RefCounted);
ImplementSingleton(Base::ShaderServerBase);

using namespace CoreGraphics;
using namespace IO;
using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ShaderServerBase::ShaderServerBase() :
    isOpen(false),
    paramBindMode(BindBySemantic),
    curShaderFeatureBits(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShaderServerBase::~ShaderServerBase()
{
    s_assert(!this->IsOpen());
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
ShaderServerBase::Open()
{
    s_assert(!this->isOpen);
    s_assert(this->shaders.IsEmpty());

    // load all shaders from disk
    Array<String> files = IoServer::Instance()->ListFiles("shd:", "*");
    IndexT i;
    for (i = 0; i < files.Size(); i++)
    {
        ResourceId resId("shd:" + files[i]);
        Ptr<Shader> newShader = Shader::Create();
        newShader->SetResourceId(resId);
        newShader->SetLoader(StreamShaderLoader::Create());
        newShader->SetAsyncEnabled(false);
        newShader->Load();
        if (newShader->IsLoaded())
        {
            newShader->SetLoader(0);
            this->shaders.Add(resId, newShader);
        }
        else
        {
            s_error("Failed to load shader '%s'!", files[i].c_str());
        }
    }
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShaderServerBase::Close()
{
    s_assert(this->isOpen);

    // unload all currently loaded shaders
    IndexT i;
    for (i = 0; i < this->shaders.Size(); i++)
    {
        this->shaders.ValueAtIndex(i)->Unload();
    }
    this->shaders.Clear();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    This creates a clone of a template shader. This is the only method
    to create a new shader object. When the shader instance is no longer
    needed, call UnregisterShaderInstance() for proper cleanup.
*/
Ptr<ShaderInstance>
ShaderServerBase::CreateShaderInstance(const ResourceId& resId)
{
    s_assert(resId.Value().IsValid());

    // first check if the shader is already loaded
    if (!this->shaders.Contains(resId))
    {
        s_error("ShaderServer: shader '%s' not found!", resId.Value().c_str());
    }

    // create a shader instance object from the shader
    Ptr<ShaderInstance> shaderInstance = this->shaders[resId]->CreateShaderInstance();
    return shaderInstance;
}

} // namespace Base
