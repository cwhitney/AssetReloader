#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "AssetReloader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LiveReloadApp : public AppBasic {
  public:
	void setup();
	void update();
	void draw();
    void reloadAssets();
    void fileDrop( FileDropEvent event );
    void prepareSettings( Settings *settings );
    
  protected:
    rph::AssetReloader mReloader;
    gl::Texture mTex1, mTex2;
};

void LiveReloadApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 640+200, 480 );
}

void LiveReloadApp::setup()
{
    mReloader.setup();
    
    // load images
    mReloader.load( "room 036.png", "TOP_IMAGE" );
    mReloader.load( "room 082.png", "BOTTOM_IMAGE" );
    
    // get loaded textures
    mTex1 = mReloader.getAsset( "TOP_IMAGE" );
    mTex2 = mReloader.getAsset( "BOTTOM_IMAGE" );
    
    // connect for reloading // lambda
//    mReloader["TOP_IMAGE"].connect( [=]( char *s ) { mTex1 = mReloader.getAsset( s ); } );
    mReloader.sAssetReloaded.connect(
        boost::bind( &LiveReloadApp::reloadAssets, this )
    );
}

void LiveReloadApp::reloadAssets()
{
    mTex1 = mReloader.getAsset( "TOP_IMAGE" );
    mTex2 = mReloader.getAsset( "BOTTOM_IMAGE" );
}

void LiveReloadApp::update()
{
}

void LiveReloadApp::draw()
{
	gl::clear( Color( 1, 1, 1 ) );
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    if( mTex1 ){
        gl::draw( mTex1 );
        gl::draw( mTex2, Vec2f(0, mTex1.getHeight() + 10) );
        
        mTex1.enableAndBind();
        
        gl::pushMatrices();{
            gl::translate( Vec2f(mTex1.getWidth() + 30, 30) + Vec2f::one() * 120 );
            gl::rotate( Vec3f( 0, getElapsedSeconds() * -20.0f, 0 ) );
            gl::drawSphere( Vec3f(0, 0, 0), 100 );
        }gl::popMatrices();
        
        mTex1.unbind();
    }
    
    mReloader.draw();
}

void LiveReloadApp::fileDrop( FileDropEvent event )
{
//    console() << "LiveReloadApp :: got drop :: " << event.getFile( 0 ) << endl;
    
    mReloader.reloadAsset( event.getFile(0) );
}


CINDER_APP_BASIC( LiveReloadApp, RendererGl )
