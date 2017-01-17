//
//  Settings.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Settings.h"
#include "GameManager.h"


Scene* Settings::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Settings::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}


bool Settings::init()
{
    //////////////////////////////
    // 1. super init first
    
    if ( !LayerColor::initWithColor(GameManager::getInstance()->background_color))
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    
    //invisible container used for layout
    auto container = Sprite::create("chenar.png");
    container->setPosition(roundf(visibleSize.width/2), roundf(visibleSize.height*0.55));
    container->setVisible(false);
    addChild(container);

    
    
    auto backSprite=Sprite::create("back.png");
    backSprite->setFlippedY(false);
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(Settings::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2-container->getContentSize().width*0.02), visibleSize.height*0.95));
    this->addChild(menuBack);
    
    
    packs_label = LabelTTF::create("Settings", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*48);
    packs_label->setHorizontalAlignment(TextHAlignment::LEFT);
    packs_label->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), visibleSize.height*0.955));
    packs_label->setColor(Color3B(105,129,117));
    packs_label->setAnchorPoint(Point(0, 0.55));
    packs_label->setColor(GameManager::getInstance()->color2);
    //packs_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    //label->getTexture()->setAliasTexParameters();
    // position the label on the center of the screen
    
    // add the label as a child to this layer
    this->addChild(packs_label);

       auto btn_on=Sprite::create("btn_off.png");
    //btn_on->getTexture()->setAliasTexParameters();
    
    auto btn_off=Sprite::create("btn_on.png");
    //btn_off->getTexture()->setAliasTexParameters();
    
    
    
    
    auto label_labels=LabelTTF::create("", GameManager::getInstance()->font_name,GameManager::getInstance()->font_scale*40);
    //label_labels->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    label_labels->setAnchorPoint(Point(0,0.5));
    label_labels->setPosition(packs_label->getPositionX(), container->getPositionY()+container->getContentSize().height/2-btn_off->getContentSize().height/2);
    label_labels->setColor(GameManager::getInstance()->color2);
    this->addChild(label_labels);
    
    
    auto label_night=LabelTTF::create("Dark Mode ", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*40);
    //label_night->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    label_night->setAnchorPoint(Point(0,0.5));
    label_night->setPosition(packs_label->getPositionX(), label_labels->getPositionY()-label_labels->getContentSize().height*1.8);
    label_night->setColor(GameManager::getInstance()->color2);
    this->addChild(label_night);

    
    auto label_sound=LabelTTF::create("Game Sounds ", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*40);
    //label_sound->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    label_sound->setAnchorPoint(Point(0,0.5));
    label_sound->setPosition(packs_label->getPositionX(), label_night->getPositionY()-label_night->getContentSize().height*1.8);
    label_sound->setColor(GameManager::getInstance()->color2);
    this->addChild(label_sound);
    
    
 
    
    
    auto btn_labels=MenuItemSprite::create(btn_on,btn_on);
    auto btn_labels2=MenuItemSprite::create(btn_off,btn_off);
    
    
    
    labels = MenuItemToggle::createWithCallback( CC_CALLBACK_1(Settings::menuLabelsClick, this), btn_labels,btn_labels2,NULL );
    labels->setAnchorPoint(Point(0, 0.5));
    auto menuLabels=Menu::create(labels,NULL);
    menuLabels->setPosition(Point(roundf(container->getPositionX()+container->getContentSize().width/2-btn_off->getContentSize().width), label_labels->getPositionY()));
    this->addChild(menuLabels);

    labels->setSelectedIndex(UserDefault::getInstance()->getIntegerForKey("labels"));
    
    auto btn_on2=Sprite::create("btn_off.png");
    //btn_on2->getTexture()->setAliasTexParameters();
    
    auto btn_off2=Sprite::create("btn_on.png");
    //btn_off2->getTexture()->setAliasTexParameters();
    
    
    auto btn_labels22=MenuItemSprite::create(btn_on2,btn_on2);
    auto btn_labels222=MenuItemSprite::create(btn_off2,btn_off2);
    
    
    night = MenuItemToggle::createWithCallback( CC_CALLBACK_1(Settings::menuNightClick, this), btn_labels22,btn_labels222,NULL );
    night->setAnchorPoint(Point(0, 0.5));
    auto menuNight=Menu::create(night,NULL);
    menuNight->setPosition(Point(roundf(container->getPositionX()+container->getContentSize().width/2-btn_off->getContentSize().width), label_night->getPositionY()));
    this->addChild(menuNight);
    night->setSelectedIndex(UserDefault::getInstance()->getIntegerForKey("night"));
    
    
    
    
    auto btn_on22=Sprite::create("btn_off.png");
    //btn_on22->getTexture()->setAliasTexParameters();
    
    auto btn_off22=Sprite::create("btn_on.png");
    //btn_off22->getTexture()->setAliasTexParameters();
    
    
    auto btn_labels2222=MenuItemSprite::create(btn_on22,btn_on22);
    auto btn_labels22222=MenuItemSprite::create(btn_off22,btn_off22);
    
    
    sound = MenuItemToggle::createWithCallback( CC_CALLBACK_1(Settings::menuSoundClick, this), btn_labels2222,btn_labels22222,NULL );
    sound->setAnchorPoint(Point(0, 0.5));
    auto menuSound=Menu::create(sound,NULL);
    menuSound->setPosition(Point(roundf(container->getPositionX()+container->getContentSize().width/2-btn_off->getContentSize().width), label_sound->getPositionY()));
    this->addChild(menuSound);
    sound->setSelectedIndex(UserDefault::getInstance()->getIntegerForKey("sound"));
    
    
    auto resetSprite=Sprite::create("reset_progress.png");
    resetSprite->setFlippedY(false);
    
    
    auto resetSprite2=Sprite::create("reset_progress.png");
    //reviewSprite2->getTexture()->setAliasTexParameters();
    resetSprite2->setFlippedY(false);
    resetSprite2->setColor(GameManager::getInstance()->color2);
    
    reset=MenuItemSprite::create(resetSprite,resetSprite2,CC_CALLBACK_1(Settings::menuResetClick, this));
    reset->setAnchorPoint(Point(-0.35, 0.35));
    auto menuReset=Menu::create(reset,NULL);
    menuReset->setPosition(Point(packs_label->getPositionX()+10 , label_sound->getPositionY()-label_sound->getContentSize().height*3));
    this->addChild(menuReset);
    

    

    
    overlay=Sprite::create("finish_overlay.png");
    overlay->setPosition(visibleSize.width/2, visibleSize.height/2);
    overlay->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    overlay->setVisible(false);
    this->addChild(overlay);

    spinner=Sprite::create("spinner.png");
    spinner->setPosition(visibleSize.width/2, visibleSize.height/2);
    spinner->setVisible(false);
    this->addChild(spinner,1001);
    
    
    
    auto move=MoveBy::create(0.6, Point(0,-visibleSize.height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    back->setPositionY(back->getPositionY()+visibleSize.height/2);
    packs_label->setPositionY(packs_label->getPositionY()+visibleSize.height/2);
    
    
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    
    
    return true;
}

void Settings::setLoading(bool loading){
    overlay->setVisible(loading);
    spinner->setVisible(loading);
    if(loading){
        auto rotate=RotateBy::create(1.2, -360);
        auto repeat=RepeatForever::create(rotate);
        spinner->runAction(repeat);
    }
    else{
        spinner->stopAllActions();
        spinner->setRotation(0);
    }
}



void Settings::menuResetClick(cocos2d::Ref *pSender){
    if(overlay->isVisible())
        return;
    GameManager::getInstance()->resetProgress();
}

void Settings::menuRestoreClick(cocos2d::Ref *pSender){
    if(overlay->isVisible())
        return;
    GameManager::getInstance()->restorePurchase();
}

void Settings::menuBackClick(cocos2d::Ref *pSender){
    if(overlay->isVisible())
        return;
    
    auto move=MoveBy::create(0.3, Point(0,Director::getInstance()->getVisibleSize().height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    
    
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    

    
    GameManager::getInstance()->setScene(1);
    GameManager::getInstance()->playSound(1);
}

void Settings::menuLabelsClick(cocos2d::Ref* pSender){
    if(overlay->isVisible())
        return;
    UserDefault::getInstance()->setIntegerForKey("labels", labels->getSelectedIndex());
    UserDefault::getInstance()->flush();
    GameManager::getInstance()->playSound(1);
}

void Settings::menuNightClick(cocos2d::Ref* pSender){
    if(overlay->isVisible())
        return;
    UserDefault::getInstance()->setIntegerForKey("night", night->getSelectedIndex());
    UserDefault::getInstance()->flush();
    GameManager::getInstance()->updatePreferences();
    GameManager::getInstance()->playSound(1);
}

void Settings::menuSoundClick(cocos2d::Ref* pSender){
    if(overlay->isVisible())
        return;
    UserDefault::getInstance()->setIntegerForKey("sound", sound->getSelectedIndex());
    UserDefault::getInstance()->flush();
    GameManager::getInstance()->playSound(1);
}

