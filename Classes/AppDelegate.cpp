#include "AppDelegate.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview) {
        glview = GLViewImpl::create("Flow");
        //glview->setDesignResolutionSize(640, 1136, ResolutionPolicy::NO_BORDER);
        director->setOpenGLView(glview);
    }

    
    //initialize our game manager class
    
    gamemanager=new GameManager();
    
    
    //set up content scaling for resolutions different from the design resolution
    
    
    if(director->getVisibleSize().width<640 || (director->getVisibleSize().width>640 && director->getVisibleSize().width<768)){
        float scale=1;
        scale=640.0/director->getVisibleSize().width;
        director->setContentScaleFactor(scale);
        GameManager::getInstance()->font_scale=1.0/scale;
    }
    
    
    if(director->getVisibleSize().width>768){
        director->setContentScaleFactor(768.0/director->getVisibleSize().width);
        GameManager::getInstance()->font_scale=1.0/(768.0/director->getVisibleSize().width);
    }


    director->setProjection(Director::Projection::_2D);
    
    // turn off display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    director->runWithScene(GameManager::getInstance()->menu1->createScene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
