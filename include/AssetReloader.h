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
        static AssetReloader* getInstance();
        
        void setup( int width=200, int height=400 );
        void load( const fs::path assetPath );
        void load( const fs::path assetPath, string key );
        void reloadAsset( fs::path assetPath );
        
        void draw();
        void show( bool bShow=true ){ bVisible = bShow; };
        bool isVisible(){ return bVisible; };
        
        //    gl::Texture getAsset( fs::path key );
        gl::Texture getAsset( string key );
        
        boost::signals2::signal<void(void)> sAssetReloaded;
        
    protected:
        AssetReloader(){};
        AssetReloader(AssetReloader const&){}; // copy constructor is private
        AssetReloader& operator=(AssetReloader const&){return *m_pInstance;}; // assignment operator is private
        static AssetReloader* m_pInstance;
        
        map<string, gl::Texture>    mLoadedImages;
        map<string, string>         mKeyList;
        
        //
        bool bVisible, bDragging;
        Vec2f mStartDragPos;
        Vec2f mMaxThumbSize;
        
        // events
        bool mouseDown( app::MouseEvent event );
        bool mouseUp( app::MouseEvent event );
        bool mouseMove( app::MouseEvent event );
        
        // for drawing
        Rectf mDragableRect;
        float mWinWidth, mWinHeight;
        Vec2f mPosition, mWinSize;
        Font mBitFont;
        
        // text
        void drawText();
        gl::TextureFontRef mFont;
    };
    
}