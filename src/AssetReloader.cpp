#include "AssetReloader.h"
#include "cinder/app/App.h"

using namespace rph;
 
// Global static pointer used to ensure a single instance of the class.
AssetReloader* AssetReloader::m_pInstance = NULL;
AssetReloader* AssetReloader::getInstance(){
    if (!m_pInstance){ // Only allow one instance of class to be generated.
        m_pInstance = new AssetReloader;
        m_pInstance->setup();
    }
    return m_pInstance;
}


void AssetReloader::setup( int width, int height )
{
    bVisible = false;
    bDragging = false;
    
    mWinWidth = width;
    mWinHeight = height;
    mWinSize = Vec2f( mWinWidth, mWinHeight );
    
    mMaxThumbSize = Vec2f( 85-20, mWinWidth-20 );
    
    mPosition = Vec2f( getWindowWidth() - mWinWidth - 10, 10);
    mDragableRect = Rectf( 0,0, mWinWidth, 15 );
    
    mFont = gl::TextureFont::create( Font( "Helvetica", 12) );
    
    app::App::get()->registerMouseDown( this, &AssetReloader::mouseDown );
    app::App::get()->registerMouseUp( this, &AssetReloader::mouseUp );
    //    app::App::get()->registerMouseWheel( mouseWheel );
    //    app::App::get()->registerMouseMove( mouseMove );
    app::App::get()->registerMouseDrag( this, &AssetReloader::mouseMove );
    //    app::App::get()->registerKeyDown( keyDown );
    //    app::App::get()->registerResize( resize );
}

bool AssetReloader::mouseDown( app::MouseEvent event )
{
    if( !bVisible )
        return false;
    
    if( mDragableRect.contains( event.getPos() - mPosition ) ){
        bDragging = true;
        mStartDragPos = event.getPos() - mPosition;
        return true;
    }
    
    return false;
}

bool AssetReloader::mouseUp( app::MouseEvent event )
{
    if( !bVisible )
        return false;
    
    bDragging = false;
    
    return false;
}

bool AssetReloader::mouseMove( app::MouseEvent event )
{
    if( !bVisible )
        return false;
    
    if( bDragging ){
        mPosition = event.getPos() - mStartDragPos;
        return true;
    }
    
    return false;
}


void AssetReloader::load( const fs::path assetPath )
{
    load( assetPath, assetPath.c_str() );
}

void AssetReloader::load( const fs::path assetPath, string key )
{
//    console() << "AssetReloader :: loading asset :: " << assetPath << " with key :: " << key << endl;
    
    DataSourceRef asset = loadAsset( assetPath );
    
    //    console() << asset->getFilePath() << endl;
    
    gl::Texture tmp = gl::Texture( loadImage( asset ) );
    mLoadedImages[key] = tmp;
    mKeyList[ asset->getFilePath().c_str() ] = key;
}


//gl::Texture AssetReloader::getAsset( fs::path key )
//{
//    return getAsset( key.string() );
//}

gl::Texture AssetReloader:: getAsset( string key )
{
    return mLoadedImages[ key ];
}

void AssetReloader::reloadAsset( fs::path assetPath )
{
    
    //    console() << "reload :: " << mKeyList[ assetPath.c_str() ] << endl;
    
    string key = "";
    key = mKeyList[ assetPath.c_str() ];
    
    if( key != "" ){
        console() << "AssetReloader :: reloading asset :: " << assetPath.filename() << endl;
        load( assetPath.filename(), key );
        // fire signal
        sAssetReloaded();
    }else{
        console() << "AssetReloader :: can't reload " << assetPath.filename() << endl;
    }
}

void AssetReloader::drawText()
{
    gl::color( Color::white() );
    if( mFont )
        mFont->drawString("AssetReloader", Vec2f(10,10) );
    
    // iterate through map
    int i=0;
//    for( auto it = mKeyList.begin(); it!=mKeyList.end(); it++ ){
    for( map<string, string>::iterator it = mKeyList.begin(); it!=mKeyList.end(); it++ ){
        if( mFont )
            mFont->drawString( it->second , Vec2f(10, i * 18 + 118 ) );
        ++i;
    }
}

void AssetReloader::draw()
{
    if( !bVisible )
        return;
    
    gl::disableDepthRead();
//	glEnable( GL_TEXTURE_2D );
    gl::enableAlphaBlending();
    
    //    console() << "(" << mWinWidth << ", " << mWinHeight << ")" << endl;
    
    gl::pushMatrices();{
        gl::translate( mPosition );
        
        // bg
        gl::color( ColorA(0,0,0,0.3) );
        gl::drawSolidRect( Rectf( 0,0, mWinWidth, mWinHeight ) );
        
        // outline
        gl::color( Color::hex(0x999999) );
        gl::drawStrokedRect( Rectf( 0,0, mWinWidth, mWinHeight ) );
        gl::drawSolidRect( Rectf( 0,15, mWinWidth, 16 ) );
        
        // title
        gl::color( ColorA(0,0,0,0.8) );
        gl::drawSolidRect( mDragableRect );
        
        // text
        drawText();
        
        // separator
        gl::color( ColorA( 1,1,1,0.6) );
        gl::drawSolidRect( Rectf( 0,100, mWinWidth, 101 ) );
        
        if( !mKeyList.empty() )
        {
            gl::pushMatrices();{
                gl::color( ColorA(1,1,1,1) );
                gl::translate( Vec2f(5, mDragableRect.getHeight() + 5) );
                
                gl::Texture curThumb = getAsset( mKeyList.begin()->second );
                gl::draw( curThumb, Rectf(curThumb.getBounds()).getCenteredFit( Rectf(0, 0, mWinWidth-10, 85-10), true ) );
            }gl::popMatrices();
        }
        
    }gl::popMatrices();
    
    // reset color
    gl::color( Color::white() );
}