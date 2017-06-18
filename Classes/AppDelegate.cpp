#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

bool musicSet = true;//一个用来记录音乐开关的全局变量
bool first = true;//第一次之后再次返回初始界面不需要重新放音乐
bool soundSet = true;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}


void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}


static int register_all_packages()
{
    return 0; 
}

bool AppDelegate::applicationDidFinishLaunching() {

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("OurGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("OurGame");
#endif
		glview->setFrameSize(800, 600);//改变屏幕大小
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);

    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    
    auto scene = HelloWorld::createScene();
    
    director->runWithScene(scene);

	//初始化背景音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/backgroundmusic.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/gamestartmusic.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/clip.mp3");
	//FileUtils::getInstance()->addSearchPath("fonts/");

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
