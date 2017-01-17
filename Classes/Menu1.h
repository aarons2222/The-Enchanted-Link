//
//  Menu1.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __MENU1_H__
#define __MENU1_H__

#include "cocos2d.h"
using namespace cocos2d;

class Menu1 :public cocos2d::LayerColor{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Menu1);
    
    void menuPlayClick(cocos2d::Ref* pSender);
    void menuAboutClick(cocos2d::Ref* pSender);
    void menuObjectivesClick(cocos2d::Ref* pSender);
    void menuSettingsClick(cocos2d::Ref* pSender);
    void menuStoreClick(cocos2d::Ref* pSender);
    
    Layer* layer;
    Sprite *logo;
    MenuItemSprite *play,*about,*objectives,*settings,*store;
    
};

#endif
