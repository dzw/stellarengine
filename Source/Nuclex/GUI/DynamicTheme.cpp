//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Window.cpp - GUI window                                                   //
// ### # #      ###                                                                            //
// # ### #      ###  A window in a GUI, builds dialogs from groups of widgets                  //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/GUI/DynamicTheme.h"
#include "Nuclex/Storage/XMLSerializer.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Text/TextServer.h"
#include "Nuclex/Video/VertexDrawer.h"
#include "Nuclex/Support/TimeSpan.h"

using namespace Nuclex;
using namespace Nuclex::GUI;

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::placementFromString()                                          # //
// ############################################################################################# //
/** Converts a placement stored as a string into its matching enumeration value

    @param  sPlacement  String containing the placement name
    @return The matching enumeration value
*/
DynamicTheme::Style::State::Image::Placement
  DynamicTheme::placementFromString(const string &sPlacement) {

  if(sPlacement == "topleft")
    return DynamicTheme::Style::State::Image::P_TOPLEFT;
  else if(sPlacement == "top")
    return DynamicTheme::Style::State::Image::P_TOP;
  else if(sPlacement == "topright")
    return DynamicTheme::Style::State::Image::P_TOPRIGHT;
  else if(sPlacement == "left")
    return DynamicTheme::Style::State::Image::P_LEFT;
  else if(sPlacement == "center")
    return DynamicTheme::Style::State::Image::P_CENTER;
  else if(sPlacement == "right")
    return DynamicTheme::Style::State::Image::P_RIGHT;
  else if(sPlacement == "bottomleft")
    return DynamicTheme::Style::State::Image::P_BOTTOMLEFT;
  else if(sPlacement == "bottom")
    return DynamicTheme::Style::State::Image::P_BOTTOM;
  else if(sPlacement == "bottomright")
    return DynamicTheme::Style::State::Image::P_BOTTOMRIGHT;
  else
    throw InvalidArgumentException("Nuclex::GUI::DynamicTheme::placementFromString()",
                                    string("Unknown placement method: '") + sPlacement + "'");
}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::placementFromString()                                          # //
// ############################################################################################# //
/** Converts a alignment stored as a string into its matching enumeration value

    @param  sAlignment  String containing the alignment name
    @return The matching enumeration value
*/
Text::Font::Alignment DynamicTheme::alignmentFromString(const string &sAlignment) {
  if(sAlignment == "top")
    return Text::Font::A_TOP;
  else if(sAlignment == "vcenter")
    return Text::Font::A_VCENTER;
  else if(sAlignment == "bottom")
    return Text::Font::A_BOTTOM;
  else if(sAlignment == "left")
    return Text::Font::A_LEFT;
  else if(sAlignment == "hcenter")
    return Text::Font::A_RIGHT;
  else if(sAlignment == "center")
    return static_cast<Text::Font::Alignment>(
      Text::Font::A_HCENTER |
      Text::Font::A_VCENTER
    );
  else
    throw InvalidArgumentException("Nuclex::GUI::DynamicTheme::alignmentFromString()",
                                   string("Unknown text alignment: '") + sAlignment + "'");
}  

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::DynamicTheme()                                     Constructor # //
// ############################################################################################# //
/** Initializes a dynamic theme

    @param  spStorageServer  StorageServer from which to load resources
    @param  spVideoServer    VideoServer on which to load visuals
    @param  spTextServer     TextServer to use for text rendering and translation
*/
DynamicTheme::DynamicTheme(
  const shared_ptr<Storage::StorageServer> &spStorageServer,
  const shared_ptr<Video::VideoServer> &spVideoServer,
  const shared_ptr<Text::TextServer> &spTextServer
) :
  m_spVideoServer(spVideoServer),
  m_Resources("theme", spStorageServer, spVideoServer, spTextServer) {}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::~DynamicTheme()                                     Destructor # //
// ############################################################################################# //
/** Destroys a dynamic theme
*/
DynamicTheme::~DynamicTheme() {}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::load()                                                         # //
// ############################################################################################# //
/** Loads the theme's layout from a serialized layout description

    @param  spSerializer  Serializer to load from
*/
void DynamicTheme::load(const shared_ptr<Storage::Serializer> &spSerializer) {
  // Each theme has to have an unique name
  m_sName = spSerializer->get<string>("_name");
  
  // Check whether this theme required additional resources
  shared_ptr<Storage::Serializer> spResources = spSerializer->openScope("resources", true);
  if(spResources) 
    m_Resources.load(spResources);

  // Load all widget layouts contained in this theme
  shared_ptr<Storage::Serializer::ScopeEnumerator> spWidgetEnum =
    spSerializer->enumScopes("style");

  while(spWidgetEnum->next()) {
    // The type describes which kind of widget the layout is for
    Style &WD = m_Styles.insert(StyleMap::value_type(
      spWidgetEnum->get().second->get<string>("_name"),
      Style()
    )).first->second;

    // Load all states the theme defines for the widget
    shared_ptr<Storage::Serializer::ScopeEnumerator> spStateEnum =
      spWidgetEnum->get().second->enumScopes("state");

    while(spStateEnum->next()) {
      // The states name is used to decide when the state's layout will be shown
      Style::State &State = WD.States.insert(Style::StateMap::value_type(
        spStateEnum->get().second->get<string>("_name"),
        Style::State()
      )).first->second;

      // Load all images that make up the layouted widget
      shared_ptr<Storage::Serializer::ScopeEnumerator> spImageEnum =
        spStateEnum->get().second->enumScopes("image");

      while(spImageEnum->next()) {
        // Retrieve the placement of the image
        Style::State::Image::Placement ePlacement =
          placementFromString(spImageEnum->get().second->get<string>("_placement"));

        // Retrieve the region of the image in the resource bitmap
        Box2<size_t> ImageRegion(
          spImageEnum->get().second->get<size_t>("_x1"),
          spImageEnum->get().second->get<size_t>("_y1"),
          spImageEnum->get().second->get<size_t>("_x2"),
          spImageEnum->get().second->get<size_t>("_y2")
        );
        if((ImageRegion.BR.X <= ImageRegion.TL.X) || (ImageRegion.BR.Y <= ImageRegion.TL.Y))
          throw InvalidArgumentException(
            "Nuclex::GUI::DynamicTheme::load()",
            string("Invalid bitmap source region: ") + lexical_cast<string>(ImageRegion)
          );

        // Finally, get access to the bitmap that should be used as source
        shared_ptr<Video::Image> spSourceImage = m_Resources.getImage(
          spImageEnum->get().second->get<string>("_source")
        );
        
        Color ImageColor = Storage::XMLSerializer::colorFromHex(
          spImageEnum->get().second->get<string>("_color", "#FFFFFFFF")
        );

        // All informations are provided, so continue to insert the image
        Style::State::Image &Image = State.Images.insert(
          Style::State::ImageMap::value_type(
            ePlacement,
            Style::State::Image()
          )
        ).first->second;
        Image.spImage = m_spVideoServer->createImage(
          ImageRegion.getSize(), spSourceImage->getFormat()
        );
        spSourceImage->blitTo(Image.spImage, Point2<long>(0, 0), ImageRegion);
        Image.ImageColor = ImageColor;
      }

      // Is there a text style defined for the widget state ?
      shared_ptr<Storage::Serializer> spText = spStateEnum->get().second->openScope("text", true);
      if(spText) {
        State.spFont = m_Resources.getFont(spText->get<string>("_source"));
        State.eTextAlignment = alignmentFromString(spText->get<string>("_alignment"));
        State.TextColor = Storage::XMLSerializer::colorFromHex(
          spText->get<string>("_color", "#FFFFFFFF")
        );
        State.TextOffset = Point2<float>(
          spText->get<float>("_x", 0.0f),
          spText->get<float>("_y", 0.0f)
        );
      }
    }
  }
}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::drawCell()                                                     # //
// ############################################################################################# //
/** Draws a GUI cell. This could for example draw a single bitmap
    which is either fixed-size or will be scaled to match the cell's size
    or a cell made up from tiled edge and corner bitmaps to provide a crisp
    look in all sizes and resolutions. However, a cell is not required to
    be rectangular.
    
    @param  VD      VertexDrawer to use for drawing the cell
    @param  sStyle  Cell style. Typically the GUI item's name
    @param  Region  Region to cover
    @param  sState  State (kind of sub-style) of cell
*/
void DynamicTheme::drawCell(Video::VertexDrawer &VD, const string &sStyle, 
                            const Box2<float> &Region, const string &sState) {
  StyleMap::iterator DrawerIt = m_Styles.find(sStyle);
  if(DrawerIt == m_Styles.end())
    return;

  Style::StateMap::iterator StateIt = DrawerIt->second.States.find(sState);
  if(StateIt == DrawerIt->second.States.end())
    return;
  
  Box2<float> InnerRegion(Region);
  float       Begin, End;

  Style::State::ImageMap::iterator TopLeftImageIt =
    StateIt->second.Images.find(Style::State::Image::P_TOPLEFT);
  Style::State::ImageMap::iterator TopRightImageIt =
    StateIt->second.Images.find(Style::State::Image::P_TOPRIGHT);
  Style::State::ImageMap::iterator BottomLeftImageIt =
    StateIt->second.Images.find(Style::State::Image::P_BOTTOMLEFT);
  Style::State::ImageMap::iterator BottomRightImageIt =
    StateIt->second.Images.find(Style::State::Image::P_BOTTOMRIGHT);

  Style::State::ImageMap::iterator CenterImageIt =
    StateIt->second.Images.find(Style::State::Image::P_CENTER);
  
  Style::State::ImageMap::iterator LeftImageIt =
    StateIt->second.Images.find(Style::State::Image::P_LEFT);
  Style::State::ImageMap::iterator TopImageIt =
    StateIt->second.Images.find(Style::State::Image::P_TOP);
  Style::State::ImageMap::iterator RightImageIt =
    StateIt->second.Images.find(Style::State::Image::P_RIGHT);
  Style::State::ImageMap::iterator BottomImageIt =
    StateIt->second.Images.find(Style::State::Image::P_BOTTOM);

  if(LeftImageIt != StateIt->second.Images.end()) {
    InnerRegion.TL.X += LeftImageIt->second.spImage->getSize().X;
    
    Begin = Region.TL.Y;
    if(TopLeftImageIt != StateIt->second.Images.end())
      Begin += TopLeftImageIt->second.spImage->getSize().Y;
      
    End = Region.BR.Y;
    if(BottomLeftImageIt != StateIt->second.Images.end())
      End -= BottomLeftImageIt->second.spImage->getSize().Y;
      
    VD.drawImage(
      Box2<float>(
        Region.TL.X, Begin,
        Region.TL.X + LeftImageIt->second.spImage->getSize().X, End
      ),
      LeftImageIt->second.spImage,
      LeftImageIt->second.ImageColor
    );
  }

  if(RightImageIt != StateIt->second.Images.end()) {
    InnerRegion.BR.X -= RightImageIt->second.spImage->getSize().X;
    
    Begin = Region.TL.Y;
    if(TopRightImageIt != StateIt->second.Images.end())
      Begin += TopRightImageIt->second.spImage->getSize().Y;
      
    End = Region.BR.Y;
    if(BottomRightImageIt != StateIt->second.Images.end())
      End -= BottomRightImageIt->second.spImage->getSize().Y;
      
    VD.drawImage(
      Box2<float>(
        Region.BR.X - RightImageIt->second.spImage->getSize().X,
        Begin, Region.BR.X, End
      ),
      RightImageIt->second.spImage,
      RightImageIt->second.ImageColor
    );
  }

  if(TopImageIt != StateIt->second.Images.end()) {
    InnerRegion.TL.Y += TopImageIt->second.spImage->getSize().Y;
    
    Begin = Region.TL.X;
    if(TopLeftImageIt != StateIt->second.Images.end())
      Begin += TopLeftImageIt->second.spImage->getSize().X;
      
    End = Region.BR.X;
    if(TopRightImageIt != StateIt->second.Images.end())
      End -= TopRightImageIt->second.spImage->getSize().X;
      
    VD.drawImage(
      Box2<float>(
        Begin, Region.TL.Y,
        End, Region.TL.Y + TopImageIt->second.spImage->getSize().Y
      ),
      TopImageIt->second.spImage,
      TopImageIt->second.ImageColor
    );
  }

  if(BottomImageIt != StateIt->second.Images.end()) {
    InnerRegion.BR.Y -= BottomImageIt->second.spImage->getSize().Y;
    
    Begin = Region.TL.X;
    if(BottomLeftImageIt != StateIt->second.Images.end())
      Begin += BottomLeftImageIt->second.spImage->getSize().X;
      
    End = Region.BR.X;
    if(BottomRightImageIt != StateIt->second.Images.end())
      End -= BottomRightImageIt->second.spImage->getSize().X;
      
    VD.drawImage(
      Box2<float>(
        Begin, Region.BR.Y - BottomImageIt->second.spImage->getSize().Y,
        End, Region.BR.Y
      ),
      BottomImageIt->second.spImage,
      BottomImageIt->second.ImageColor
    );
  }
  
  if(CenterImageIt != StateIt->second.Images.end()) {
    if((InnerRegion.BR.X > InnerRegion.TL.X) &&
       (InnerRegion.BR.Y > InnerRegion.TL.Y))
      VD.drawImage(
        InnerRegion,
        CenterImageIt->second.spImage,
        CenterImageIt->second.ImageColor
      );
  }

  if(TopLeftImageIt != StateIt->second.Images.end()) {
    VD.drawImage(
      Box2<float>(
        Region.TL.X, Region.TL.Y,
        Region.TL.X + TopLeftImageIt->second.spImage->getSize().X,
        Region.TL.Y + TopLeftImageIt->second.spImage->getSize().Y
      ),
      TopLeftImageIt->second.spImage,
      TopLeftImageIt->second.ImageColor
    );
  }

  if(TopRightImageIt != StateIt->second.Images.end()) {
    VD.drawImage(
      Box2<float>(
        Region.BR.X - TopRightImageIt->second.spImage->getSize().X, Region.TL.Y,
        Region.BR.X, Region.TL.Y + TopRightImageIt->second.spImage->getSize().Y
      ),
      TopRightImageIt->second.spImage,
      TopRightImageIt->second.ImageColor
    );
  }

  if(BottomLeftImageIt != StateIt->second.Images.end()) {
    VD.drawImage(
      Box2<float>(
        Region.TL.X, Region.BR.Y - BottomRightImageIt->second.spImage->getSize().Y,
        Region.TL.X + BottomRightImageIt->second.spImage->getSize().X, Region.BR.Y
      ),
      BottomLeftImageIt->second.spImage,
      BottomLeftImageIt->second.ImageColor
    );
  }

  if(BottomRightImageIt != StateIt->second.Images.end()) {
    VD.drawImage(
      Box2<float>(
        Region.BR.X - BottomRightImageIt->second.spImage->getSize().X,
        Region.BR.Y - BottomRightImageIt->second.spImage->getSize().Y,
        Region.BR.X, Region.BR.Y
      ),
      BottomRightImageIt->second.spImage,
      BottomRightImageIt->second.ImageColor
    );
  }
}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::drawText()                                                     # //
// ############################################################################################# //
/** Simply draws text using a font defined by the theme implementation

    @param  VD      VertexDrawer to use for drawing
    @param  sStyle  Text style. Typically the GUI item's name
    @param  Region  Region to cover
    @param  sText   Text to be drawn
    @param  sState  State (kind of sub-style) of text
*/
void DynamicTheme::drawText(Video::VertexDrawer &VD, const string &sStyle,
                            const Box2<float> &Region, const wstring &sText,
                            const string &sState) {
  StyleMap::iterator DrawerIt = m_Styles.find(sStyle);
  if(DrawerIt == m_Styles.end())
    return;

  Style::StateMap::iterator StateIt = DrawerIt->second.States.find(sState);
  if(StateIt == DrawerIt->second.States.end())
    return;

  if(StateIt->second.spFont) {
    Text::Font::Alignment eAlignment = StateIt->second.eTextAlignment;
    Point2<float> Location;
    
    if((eAlignment & Text::Font::A_HCENTER) == Text::Font::A_HCENTER)
      Location.X = (Region.TL.X + Region.BR.X) / 2;
    else if(eAlignment & Text::Font::A_RIGHT)
      Location.X = Region.BR.X;
    else
      Location.X = Region.TL.X;

    if((eAlignment & Text::Font::A_VCENTER) == Text::Font::A_VCENTER) {
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2;
      eAlignment = static_cast<Text::Font::Alignment>(eAlignment & ~Text::Font::A_VCENTER);
    } else if(eAlignment & Text::Font::A_TOP) {
      Location.Y = Region.TL.Y;
    } else if(eAlignment & Text::Font::A_BOTTOM) {
      Location.Y = Region.BR.Y;
    } else {
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2;
    }

    VD.drawText(
      StateIt->second.spFont,
      sText,
      Location + StateIt->second.TextOffset,
      StateIt->second.TextColor,
      eAlignment
    );
  }
}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::measureRegion()                                                # //
// ############################################################################################# //
/** Measures the region a rendered text will occupy

    @param  sStyle  Text style. Typically the GUI item's name
    @param  sText   Text to be measured
    @param  sState  State (kind of sub-style) of text
    @return The region which will be covered by the text
*/
Box2<float> DynamicTheme::measureRegion(const string &sStyle, const wstring &sText,
                                        const string &sState) {
  StyleMap::iterator DrawerIt = m_Styles.find(sStyle);
  if(DrawerIt == m_Styles.end())
    return Box2<float>();

  Style::StateMap::iterator StateIt = DrawerIt->second.States.find(sState);
  if(StateIt == DrawerIt->second.States.end())
    return Box2<float>();
    
  return Box2<float>(StateIt->second.spFont->measureRegion(sText), StaticCastTag());
/*
  if(StateIt->second.spFont) {
    Text::Font::Alignment eAlignment = StateIt->second.eTextAlignment;
    Point2<float> Location;
    
    if((eAlignment & Text::Font::Alignment::A_HCENTER) == Text::Font::Alignment::A_HCENTER)
      Location.X = (Region.TL.X + Region.BR.X) / 2;
    else if(eAlignment & Text::Font::Alignment::A_RIGHT)
      Location.X = Region.BR.X;
    else
      Location.X = Region.TL.X;

    if((eAlignment & Text::Font::Alignment::A_VCENTER) == Text::Font::Alignment::A_VCENTER) {
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2;
      eAlignment = static_cast<Text::Font::Alignment>(eAlignment & ~Text::Font::Alignment::A_VCENTER);
    } else if(eAlignment & Text::Font::Alignment::A_TOP) {
      Location.Y = Region.TL.Y;
    } else if(eAlignment & Text::Font::Alignment::A_BOTTOM) {
      Location.Y = Region.BR.Y;
    } else {
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2;
    }

    VD.drawText(
      StateIt->second.spFont,
      sText,
      Location + StateIt->second.TextOffset,
      StateIt->second.TextColor,
      eAlignment
    );
  }
*/  
}

// ############################################################################################# //
// # Nuclex::GUI::DynamicTheme::drawCursor()                                                   # //
// ############################################################################################# //
/** Draws a cursor, covering both the text input caret as well as the
    mouse arrow in different states.
    
    @param  VD        VertexDrawer to use for drawing
    @param  sStyle    Text style. Typically the kind (caret or cursor)
    @param  Location  Region to cover
    @param  sState    State (kind of sub-style) of cursor
*/
void DynamicTheme::drawCaret(Video::VertexDrawer &VD, const string &sStyle,
                             const Box2<float> &Region, const wstring &sText,
                             size_t CursorPos, bool bInsertMode, const string &sState) {
  StyleMap::iterator DrawerIt = m_Styles.find(sStyle);
  if(DrawerIt == m_Styles.end())
    return;

  Style::StateMap::iterator StateIt = DrawerIt->second.States.find(sState);
  if(StateIt == DrawerIt->second.States.end())
    return;

  if(StateIt->second.spFont &&
     ((TimeSpan::getRunningTime() % TimeSpan(1000)).Microseconds >= 500000)) {
    
    Text::Font::Alignment eAlignment = StateIt->second.eTextAlignment;
    Point2<float> Location;
    
    if((eAlignment & Text::Font::A_HCENTER) == Text::Font::A_HCENTER)
      Location.X = (Region.TL.X + Region.BR.X) / 2;
    else if(eAlignment & Text::Font::A_RIGHT)
      Location.X = Region.BR.X;
    else
      Location.X = Region.TL.X;

    if((eAlignment & Text::Font::A_VCENTER) == Text::Font::A_VCENTER)
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2.0f;
    else if(eAlignment & Text::Font::A_TOP)
      Location.Y = Region.TL.Y + StateIt->second.spFont->getHeight();
    else if(eAlignment & Text::Font::A_BOTTOM)
      Location.Y = Region.BR.Y;
    else
      Location.Y = (Region.TL.Y + Region.BR.Y + StateIt->second.spFont->getHeight()) / 2.0f;
    
    Location += StateIt->second.TextOffset;
    //Location.Y += StateIt->second.spFont->getDescenderHeight();
    
    Box2<float> TextRegion(
      StateIt->second.spFont->measureRegion(sText.substr(0, CursorPos)),
      StaticCastTag()
    );
    
    if(bInsertMode) {
      VD.drawLine(
        Point2<float>(Location.X + TextRegion.BR.X - 1, Location.Y - StateIt->second.spFont->getHeight() + 2),
        Point2<float>(Location.X + TextRegion.BR.X - 1, Location.Y + 2),
        StateIt->second.TextColor
      );
    } else {
      float fWidth = static_cast<float>(StateIt->second.spFont->getMetrics(
        sText.at(CursorPos)
      ).Size.X);
        
      VD.drawLine(
        Point2<float>(Location.X + TextRegion.BR.X, Location.Y + 1),
        Point2<float>(Location.X + TextRegion.BR.X + fWidth + 1, Location.Y + 1),
        StateIt->second.TextColor
      );
    }
  }
}
