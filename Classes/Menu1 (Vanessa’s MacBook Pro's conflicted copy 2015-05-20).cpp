//
//  Menu1.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Menu1.h"
#include "GameManager.h"


Scene* Menu1::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Menu1::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool Menu1::init()
{
    //////////////////////////////
    // 1. super init first
    
    
    if ( !LayerColor::initWithColor(GameManager::getInstance()->background_color))
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    
    //invisible container used for layout
    auto container = Sprite::create("chenar.png");
    container->setPosition(roundf(visibleSize.width/2), roundf(visibleSize.height*0.55));
    container->setVisible(false);
    //container->getSpriteFrame()->getRectInPixels().size.width
    addChild(container);
    
    //---------------------------------------------------------//
    // Edit the Lines below to customize the buttons and text. //
    //---------------------------------------------------------//
    
    // Play Button BEFORE it's pressed //
    auto playSprite=Sprite::create("play.png");
    
    
    // Play Button AFTER it's pressed //
    auto playSprite2=Sprite::create("play.png");
    playSprite2->setColor(GameManager::getInstance()->color2);
    //make it a little bigger //
    //playSprite2->setScale(1.1); -> Scaling is disabled //
    
    
    // Play Button Position & Callback //
    play=MenuItemSprite::create(playSprite,playSprite2,CC_CALLBACK_1(Menu1::menuPlayClick, this));
    play->setAnchorPoint(Point(-0.4, 0.5));
    auto menuPlay=Menu::create(play,NULL);
    menuPlay->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2+container->getContentSize().width*0.13), container->getPositionY()));
    this->addChild(menuPlay);

    
    // About [INFO] Button BEFORE it's pressed //
    auto aboutSprite=Sprite::create("about.png");
   
    // About [INFO] Button AFTER it's pressed //
    auto aboutSprite2=Sprite::create("about.png");
    aboutSprite2->setColor(GameManager::getInstance()->color2);

    // About [INFO] Button Position & Callback //
    about=MenuItemSprite::create(aboutSprite,aboutSprite2,CC_CALLBACK_1(Menu1::menuAboutClick, this));
    about->setAnchorPoint(Point(0, 0.5));
    auto menuAbout=Menu::create(about,NULL);
    menuAbout->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2), roundf(container->getPositionY()-container->getContentSize().height/2 + aboutSprite->getContentSize().height/2  ) ));
    this->addChild(menuAbout);
    
    
    // Space between buttons
    float space = container->getContentSize().width-aboutSprite->getContentSize().width*4;
    space/=3;
    
    
    // Game Center Button BEFORE it's pressed //
    auto objectivesSprite=Sprite::create("tasks.png");
    
    // Game Center Button AFTER it's pressed //
    auto objectivesSprite2=Sprite::create("tasks.png");
    objectivesSprite2->setColor(GameManager::getInstance()->color2);
    
    // Game Center Button Position & Callback //
    objectives=MenuItemSprite::create(objectivesSprite,objectivesSprite2,CC_CALLBACK_1(Menu1::menuObjectivesClick, this));
    objectives->setAnchorPoint(Point(0, 0.5));
    auto menuObjectives=Menu::create(objectives,NULL);
    menuObjectives->setPosition(Point(roundf(menuAbout->getPositionX()+space+objectivesSprite->getContentSize().width), roundf(container->getPositionY()-container->getContentSize().height/2 + objectivesSprite->getContentSize().height/2  )));
    this->addChild(menuObjectives);
    
    
    // Settings Button BEFORE it's pressed //
    auto settingsSprite=Sprite::create("settings.png");
    
    // Settings Button BEFORE it's pressed //
    auto settingsSprite2=Sprite::create("settings.png");
    settingsSprite2->setColor(GameManager::getInstance()->color2);
    
    // Settings Button Position & Callback //
    settings=MenuItemSprite::create(settingsSprite,settingsSprite2,CC_CALLBACK_1(Menu1::menuSettingsClick, this));
    settings->setAnchorPoint(Point(0, 0.5));
    auto menuSettings=Menu::create(settings,NULL);
    menuSettings->setPosition(Point(roundf(menuObjectives->getPositionX()+space+settingsSprite->getContentSize().width), roundf(container->getPositionY()-container->getContentSize().height/2 + settingsSprite->getContentSize().height/2  )));
    this->addChild(menuSettings);
    
    
    // Store Button BEFORE it's pressed //
    auto storeSprite=Sprite::create("store.png");
    
    // Store Button BEFORE it's pressed //
    auto storeSprite2=Sprite::create("store.png");
    storeSprite2->setColor(GameManager::getInstance()->color2);
    
    // Store Button Position & Callback //
    store=MenuItemSprite::create(storeSprite,storeSprite2,CC_CALLBACK_1(Menu1::menuStoreClick, this));
    store->setAnchorPoint(Point(0, 0.5));
    auto menuStore=Menu::create(store,NULL);
    menuStore->setPosition(Point(roundf(container->getPositionX()+container->getContentSize().width/2-storeSprite->getContentSize().width), roundf(container->getPositionY()-container->getContentSize().height/2 + storeSprite->getContentSize().height/2  )));
    this->addChild(menuStore);
    
    int logoSpace=10;
    auto logo = Sprite::create("logo.png");
    logo->setAnchorPoint(Vec2(0.06,-0.5));
    logo->setPosition(menuPlay->getPositionX(),menuPlay->getPositionY()+playSprite->getContentSize().height/2+logoSpace);
    addChild(logo);
    
    /*
    //scale buttons a little on ipads
    if(visibleSize.width>=768){
        about->setScale(1.2);
        objectives->setScale(1.2);
        settings->setScale(1.2);
        store->setScale(1.2);
    }*/
    
    
    //set the buttons below the screen for a slide up animation
    about->setPositionY(store->getPositionY()-visibleSize.height/2);
    objectives->setPositionY(store->getPositionY()-visibleSize.height/2);
    settings->setPositionY(store->getPositionY()-visibleSize.height/2);
    store->setPositionY(store->getPositionY()-visibleSize.height/2);
    
    //create a simple slide up animation
    auto move=MoveBy::create(0.6, Point(0,visibleSize.height/2));
    
    //make the buttons slide up
    about->runAction(move->clone());
    objectives->runAction(move->clone());
    settings->runAction(move->clone());
    store->runAction(move);
    
    //get prices from Apple
    GameManager::getInstance()->getPrices();
    
    return true;
}

//play button was pressed
void Menu1::menuPlayClick(cocos2d::Ref* pSender){
    //create a slide down animation
    auto move=MoveBy::create(0.3, Point(0,-Director::getInstance()->getVisibleSize().height/2));
    
    //make the buttons slide down
    about->runAction(move->clone());
    objectives->runAction(move->clone());
    settings->runAction(move->clone());
    store->runAction(move);
    
    //set move to the pack select screen
    GameManager::getInstance()->setScene(5);
    
    //play the button pressed sound
    GameManager::getInstance()->playSound(1);
    
    //set the scroll offset to 0 on pack select screen
    Menu2::scroll=0;
}

//about button was pressed
void Menu1::menuAboutClick(cocos2d::Ref* pSender){
    GameManager::getInstance()->setScene(2);
    GameManager::getInstance()->playSound(1);
}

//gamecenter button was pressed
void Menu1::menuObjectivesClick(cocos2d::Ref* pSender){
    GameManager::getInstance()->showAchievements();
    GameManager::getInstance()->playSound(1);
}

//settings button was pressed
void Menu1::menuSettingsClick(cocos2d::Ref* pSender){
    GameManager::getInstance()->setScene(3);
    GameManager::getInstance()->playSound(1);
}

//store button was pressed
void Menu1::menuStoreClick(cocos2d::Ref* pSender){
    GameManager::getInstance()->setScene(4);
    GameManager::getInstance()->playSound(1);
}
