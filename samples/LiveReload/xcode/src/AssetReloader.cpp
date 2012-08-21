#include "AssetReloader.h"
#include "cinder/app/App.h"

using namespace rph;

AssetReloader::AssetReloader()
{
}


void AssetReloader::setup( int width, int height )
{    
    mWinWidth = width;
    mWinHeight = height;
    mWinSize = Vec2f( mWinWidth, mWinHeight );    
    
    mFont = gl::TextureFont::create( Font( "Helvetica", 12) );
    
    app::App::get()->registerMouseDown( this, &AssetReloader::mouseDown );
    app::App::get()->registerMouseUp( this, &AssetReloader::mouseUp );
//    app::App::get()->registerMouseWheel( mouseWheel );		
//    app::App::get()->registerMouseMove( mouseMove );
    app::App::get()->registerMouseDrag( this, &AssetReloader::mouseMove );
//    app::App::get()->registerKeyDown( keyDown );
//    app::App::get()->registerResize( resize );
}

AssetReloader::~AssetReloader()
{
    
}

bool AssetReloader::mouseDown( app::MouseEvent event )
{
    console() << "Reloader :: mousedown" << endl;
    
    // don't capture event
    return false;
}

bool AssetReloader::mouseUp( app::MouseEvent event )
{
    console() << "Reloader :: mouseup" << endl;
    return false;
}

bool AssetReloader::mouseMove( app::MouseEvent event )
{
    console() << "Reloader :: mousedrag" << endl;
    return false;
}


void AssetReloader::load( const fs::path assetPath )
{
    load( assetPath, assetPath.c_str() );
}

void AssetReloader::load( const fs::path assetPath, string key )
{
    console() << "AssetReloader :: loading asset :: " << assetPath << " with key :: " << key << endl;
    
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
    mFont->drawString("AssetReloader", Vec2f(10,10) );
    
    // iterate through map
    int i=0;
//    for( map<string, string>::iterator it = mKeyList.begin(); it!=mKeyList.end(); it++ ){
    for( auto it = mKeyList.begin(); it!=mKeyList.end(); it++ ){
        mFont->drawString( it->second , Vec2f(10, i * 18 + 118 ) );
        ++i;
    }
}

void AssetReloader::draw()
{
    gl::disableDepthRead();
    int w = getWindowWidth();
    gl::enableAlphaBlending();
    
//    console() << "(" << mWinWidth << ", " << mWinHeight << ")" << endl;
    
    gl::pushMatrices();{
        gl::translate( Vec2i( w-mWinWidth - 10, 10 ) );
        
        // bg
        gl::color( ColorA(0,0,0,0.3) );
        gl::drawSolidRect( Rectf( 0,0, mWinWidth, mWinHeight ) );
        
        // outline
        gl::color( Color::hex(0xc7c7c7) );
        gl::drawStrokedRect( Rectf( 0,0, mWinWidth, mWinHeight ) );
        
        // title
        gl::color( ColorA(0,0,0,0.8) );
        gl::drawSolidRect( Rectf( 0,0, mWinWidth, 15 ) );
        
        // text
        drawText();
        
        // separator
        gl::color( ColorA( 1,1,1,0.6) );
        gl::drawSolidRect( Rectf( 0,100, mWinWidth, 101 ) );
        
        gl::pushMatrices();{
            gl::color( ColorA(1,1,1,1) );
            gl::translate( Vec2f(20,20) );
            gl::scale( Vec2f::one() * 0.5 );
//            gl::draw( getAsset("TOP_IMAGE") );
        }gl::popMatrices();
        
    }gl::popMatrices(); 
    
    // reset color
    gl::color( Color::white() );
}