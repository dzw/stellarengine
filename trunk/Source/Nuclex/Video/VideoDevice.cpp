//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoDevice.cpp - Rendering device instance                               //
// ### # #      ###                                                                            //
// # ### #      ###  Represents an instance of a rendering device                              //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Video/VertexDeclaration.h"
#include "Nuclex/Video/VertexShader.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::VideoDevice::SimpleVertex::getShader()                                     # //
// ############################################################################################# //
const shared_ptr<VertexShader> &VideoDevice::SimpleVertex::getShader() {
  static const shared_ptr<VertexShader> spSimpleShader(new VertexShader);
  return spSimpleShader;
}

// ############################################################################################# //
// # Nuclex::Video::VideoDevice::SimpleVertex::getDeclaration()                                # //
// ############################################################################################# //
const VertexDeclaration &VideoDevice::SimpleVertex::getDeclaration() {
  static VertexDeclaration SimpleVD = 
    VertexDeclaration() << VertexDeclaration::Position()
                        << VertexDeclaration::Color()
                        << VertexDeclaration::TexCoord();
  return SimpleVD;
}

// ############################################################################################# //
// # Nuclex::Video::VideoDevice::PretransformedVertex::getShader()                             # //
// ############################################################################################# //
const shared_ptr<VertexShader> &VideoDevice::PretransformedVertex::getShader() {
  static const shared_ptr<VertexShader> spPretransformedShader(new VertexShader);
  return spPretransformedShader;
}

// ############################################################################################# //
// # Nuclex::Video::VideoDevice::PretransformedVertex::getDeclaration()                        # //
// ############################################################################################# //
const VertexDeclaration &VideoDevice::PretransformedVertex::getDeclaration() {
  static VertexDeclaration PretransformedVertex = 
    VertexDeclaration() << VertexDeclaration::PretransformedPosition()
                        << VertexDeclaration::Color()
                        << VertexDeclaration::Color(1)
                        << VertexDeclaration::TexCoord();
  return PretransformedVertex;
}
