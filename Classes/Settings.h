//
//  Settings.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"
using namespace cocos2d;

class Settings :public cocos2d::LayerColor{
public:;
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(Settings);
    
    void menuBackClick(cocos2d::Ref* pSender);
    void menuLabelsClick(cocos2d::Ref* pSender);
    void menuNightClick(cocos2d::Ref* pSender);
    void menuSoundClick(cocos2d::Ref* pSender);
    void menuResetClick(cocos2d::Ref* pSender);
    void menuRestoreClick(cocos2d::Ref* pSender);
    
    
    MenuItemSprite *back,*reset,*restore;
    
    MenuItemToggle *labels,*night,*sound;
    
    Sprite *overlay,*spinner;
    void setLoading(bool);
    
    LabelTTF *packs_label;
};



#endif