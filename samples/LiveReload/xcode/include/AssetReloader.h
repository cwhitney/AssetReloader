#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"

#include <boost/signals2.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace rph {

class AssetReloader {

  public:
    AssetReloader();
//    AssetReloader( int width, int height );
    ~AssetReloader();
    
    void setup( int width=200, int height=400 );
    void load( const fs::path assetPath );
    void load( const fs::path assetPath, string key );
    void reloadAsset( fs::path assetPath );
    
    void draw();
    
//    gl::Texture getAsset( fs::path key );
    gl::Texture getAsset( string key );
    
    boost::signals2::signal<void(void)> sAssetReloaded;
    
  protected:
    
    map<string, gl::Texture>    mLoadedImages;
    map<string, string>         mKeyList;
    
    // events
    bool mouseDown( app::MouseEvent event );
    bool mouseUp( app::MouseEvent event );
    bool mouseMove( app::MouseEvent event );
    
    // for drawing
    float mWinWidth, mWinHeight;
    Vec2f mWinSize;
    Font mBitFont;
    
    // text
    void drawText();
    gl::TextureFontRef mFont;
};
    
}