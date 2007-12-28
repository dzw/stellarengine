//------------------------------------------------------------------------------
//  renderapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/renderapplication.h"
#include "scripting/lua/luaserver.h"
#include "io/filestream.h"
#include "commands/iolibrary.h"
#include "commands/stdlibrary.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/texture.h"
#include "coregraphics/mesh.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"
#include "frame/frameserver.h"
#include "coregraphics/debug/displaypagehandler.h"
#include "coregraphics/debug/texturepagehandler.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/debug/shaderpagehandler.h"
#include "scripting/debug/scriptingpagehandler.h"
#include "io/debug/iopagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "core/debug/corepagehandler.h"

namespace App
{
using namespace Core;
using namespace Util;
using namespace Math;
using namespace IO;
using namespace Interface;
using namespace Scripting;
using namespace CoreGraphics;
using namespace Resources;
using namespace Models;
using namespace Graphics;
using namespace Lighting;
using namespace Input;
using namespace Frame;
using namespace Http;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
RenderApplication::RenderApplication() :
    time(0.0),
    frameTime(0.0),
    quitRequested(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RenderApplication::~RenderApplication()
{
    s_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
RenderApplication::Open()
{
    s_assert(!this->IsOpen());
    if (Application::Open())
    {
        // setup core subsystem
        this->coreServer = CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->Open();

        // setup io subsystem
        this->ioServer = IoServer::Create();
        this->ioServer->RegisterUriScheme("file", FileStream::RTTI);
        this->ioServer->SetAssign(Assign("shd", "home:export/shaders"));
        this->ioServer->SetAssign(Assign("frame", "home:export/frame"));
        this->ioServer->SetAssign(Assign("msh", "home:export/meshes"));
        this->ioServer->SetAssign(Assign("tex", "home:export/textures"));
        this->ioServer->SetAssign(Assign("ani", "home:export/anims"));
        this->ioServer->SetAssign(Assign("mdl", "home:export/gfxlib"));        

        // Nebul2 backward compat assigns:
        this->ioServer->SetAssign(Assign("meshes", "home:export/meshes"));
        this->ioServer->SetAssign(Assign("textures", "home:export/textures"));
        this->ioServer->SetAssign(Assign("anims", "home:export/anims"));

        this->ioInterface = IOInterface::Create();
        this->ioInterface->Open();

        // mount asset zip archives
        if (IoServer::Instance()->FileExists("home:export.zip"))
        {
            // main thread
            this->ioServer->MountZipArchive("home:export.zip");

            // io thread
            Ptr<Interface::MountZipArchive> mountZipArchiveMsg = Interface::MountZipArchive::Create();
            mountZipArchiveMsg->SetURI("home:export.zip");
            this->ioInterface->Send(mountZipArchiveMsg.upcast<Messaging::Message>());
        }

        // setup debug http server
        this->httpServer = HttpServer::Create();
        this->httpServer->Open();
        this->httpServer->AttachRequestHandler(Debug::MemoryPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::CorePageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::IoPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::ScriptingPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::DisplayPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::TexturePageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::MeshPageHandler::Create());
        this->httpServer->AttachRequestHandler(Debug::ShaderPageHandler::Create());

        // setup scripting subsystem
        this->scriptServer = LuaServer::Create();
        this->scriptServer->Open();
        Commands::IOLibrary::Register();
        Commands::StdLibrary::Register();
    
        // setup core graphics subsystem
        this->displayDevice = DisplayDevice::Create();
        this->OnConfigureDisplayDevice();
        if (!this->displayDevice->Open())
        {
            s_error("RenderApplication: Failed to open display device!");
            return false;
        }
        this->renderDevice = RenderDevice::Create();
        if (!this->renderDevice->Open())
        {
            s_error("RenderApplication: Failed to open render device!");
            return false;
        }
        this->vertexLayoutServer = VertexLayoutServer::Create();
        this->vertexLayoutServer->Open();
        this->transformDevice = TransformDevice::Create();
        this->transformDevice->Open();
        this->shaderServer = ShaderServer::Create();
        this->shaderServer->Open();
        this->shapeRenderer = ShapeRenderer::Create();
        this->shapeRenderer->Open();

        // setup resource subsystem
        this->sharedResourceServer = SharedResourceServer::Create();
        this->sharedResourceServer->Open();
        this->resourceManager = ResourceManager::Create();
        this->resourceManager->Open();

        // setup frame server
        this->frameServer = FrameServer::Create();
        this->frameServer->Open();

        // setup resource mapper for textures
        Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
        texMapper->SetPlaceholderResourceId(ResourceId("tex:system/placeholder.dds"));
        texMapper->SetResourceClass(Texture::RTTI);
        texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
        texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
        this->resourceManager->AttachMapper(texMapper.upcast<ResourceMapper>());

        // setup resource mapper for meshes
        Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
        meshMapper->SetPlaceholderResourceId(ResourceId("msh:system/placeholder_s_0.nvx2"));
        meshMapper->SetResourceClass(Mesh::RTTI);
        meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
        meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
        this->resourceManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

        // setup input subsystem
        this->inputServer = InputServer::Create();
        this->inputServer->Open();
        
        // setup model server
        this->modelServer = ModelServer::Create();
        this->modelServer->Open();

        // setup graphics subsystem
        this->graphicsServer = GraphicsServer::Create();
        this->graphicsServer->Open();

        // setup lighting subsystem
        this->lightServer = LightServer::Create();
        this->lightServer->Open();
        this->shadowServer = ShadowServer::Create();
        this->shadowServer->Open();

        // setup frame timer
        this->timer.Start();
        this->time = 0.0;
        this->frameTime = 0.01;

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnConfigureDisplayDevice()
{
    // display adapter
    Adapter::Code adapter = Adapter::Primary;
    if (this->args.HasArg("-adapter"))
    {
        adapter = Adapter::FromString(this->args.GetString("-adapter"));
        if (this->displayDevice->AdapterExists(adapter))
        {
            this->displayDevice->SetAdapter(adapter);
        }
    }

    // display mode
    DisplayMode displayMode;
    if (this->args.HasArg("-x"))
    {
        displayMode.SetXPos(this->args.GetInt("-x"));
    }
    if (this->args.HasArg("-y"))
    {
        displayMode.SetYPos(this->args.GetInt("-y"));
    }
    if (this->args.HasArg("-w"))
    {
        displayMode.SetWidth(this->args.GetInt("-w"));
    }
    if (this->args.HasArg("-h"))
    {
        displayMode.SetHeight(this->args.GetInt("-h"));
    }
    this->displayDevice->SetDisplayMode(displayMode);
    this->displayDevice->SetFullscreen(this->args.GetBool("-fullscreen"));
    this->displayDevice->SetAlwaysOnTop(this->args.GetBool("-alwaysontop"));
    this->displayDevice->SetVerticalSyncEnabled(this->args.GetBool("-vsync"));
    if (this->args.HasArg("-aa"))
    {
        this->displayDevice->SetAntiAliasQuality(AntiAliasQuality::FromString(this->args.GetString("-aa")));
    }

    // window title
    if (this->args.HasArg("-windowtitle"))
    {
        this->displayDevice->SetWindowTitle(this->args.GetString("-windowtitle"));
    }
    else
    {
        this->displayDevice->SetWindowTitle("N3 Viewer");
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Close()
{
    s_assert(this->IsOpen());

    this->shadowServer->Close();
    this->shadowServer = 0;

    this->lightServer->Close();
    this->lightServer = 0;

    this->graphicsServer->Close();
    this->graphicsServer = 0;

    this->modelServer->Close();
    this->modelServer = 0;

    this->resourceManager->Close();
    this->resourceManager = 0;

    this->inputServer->Close();
    this->inputServer = 0;

    this->frameServer->Close();
    this->frameServer = 0;

    this->sharedResourceServer->Close();
    this->sharedResourceServer = 0;

    this->shapeRenderer->Close();
    this->shapeRenderer = 0;

    this->shaderServer->Close();
    this->shaderServer = 0;

    this->transformDevice->Close();
    this->transformDevice = 0;

    this->vertexLayoutServer->Close();
    this->vertexLayoutServer = 0;

    this->renderDevice->Close();
    this->renderDevice = 0;

    this->displayDevice->Close();
    this->displayDevice = 0;

    this->scriptServer->Close();
    this->scriptServer = 0;

    this->httpServer->Close();
    this->httpServer = 0;
        
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;

    this->coreServer->Close();
    this->coreServer = 0;

    Application::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::Run()
{
    s_assert(this->IsOpen());
    while (!(this->displayDevice->IsQuitRequested() || this->IsQuitRequested()))
    {
        this->httpServer->OnFrame();
        this->resourceManager->Prepare();
        this->inputServer->BeginFrame();
        this->displayDevice->ProcessWindowMessages();
        this->inputServer->OnFrame();
        this->OnProcessInput();
        this->UpdateTime();
        this->OnUpdateFrame();
        if (this->renderDevice->BeginFrame())
        {
            this->OnRenderFrame();
            this->renderDevice->EndFrame();
            this->renderDevice->Present();
        }
        this->resourceManager->Update();
        this->inputServer->EndFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnProcessInput()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnUpdateFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::OnRenderFrame()
{
    // empty, override this method in a subclass
}

//------------------------------------------------------------------------------
/**
*/
void
RenderApplication::UpdateTime()
{
    Time curTime = this->timer.GetTime();
    this->frameTime = curTime - this->time;
    this->time = curTime;
}

} // namespace App