
#ifndef NUCLEX_PACKAGE_H
#define NUCLEX_PACKAGE_H
#pragma once

#include "Nuclex/Kernel.h"
#include "Nuclex/Video/VideoDriver.h"
#include "Nuclex/Audio/AudioDriver.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Audio/AudioDevice.h"
#include "Nuclex/Kernel.h"

#include "Luaplugin/luaplugin.h"
#include "FreetypePlugin/FreetypePlugin.h"
#include "DirectInput8Plugin/DirectInput8Plugin.h"
#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "CoronaPlugin/CoronaPlugin.h"
#include "AudierePlugin/AudierePlugin.h"
#include "PythonPlugin/PythonPlugin.h"
#include "RarPlugin/RarPlugin.h"
#include "ZipPlugin/ZipPlugin.h"


void LoadAllPlugin()
{
  USEPACKAGE(AudierePlugin);
  USEPACKAGE(CoronaPlugin);
  USEPACKAGE(Direct3D9Plugin);
  USEPACKAGE(DirectInput8Plugin);
  USEPACKAGE(FreeTypePlugin);
  USEPACKAGE(LuaPlugin);
  USEPACKAGE(PythonPlugin);
  USEPACKAGE(RarPlugin);
  //USEPACKAGE(TerrainPlugin);
  USEPACKAGE(ZipPlugin);
}

#endif