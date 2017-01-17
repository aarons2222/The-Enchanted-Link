//
//  Store.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __STORE_H__
#define __STORE_H__

#include "cocos2d.h"
using namespace cocos2d;

class Store :public cocos2d::LayerColor{
public:
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(Store);
    
    void menuBackClick(cocos2d::Ref* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    
    MenuItemSprite *back;
    
    //momentum scrolling
    virtual void update(float dt);
    float is_scrolling,is_autoscrolling,speed[4],velocity;
    float scroll,last_scroll,scroll_timer;
    long time[4];
    long get_milli_time();
    cocos2d::Point limite;
    
    
    Sprite *packs[15],*packs_bg[10];
    float packs_pos[15],prices_pos[10];
    int tab_pressed_id,tab_state[10];
    LabelTTF *prices[10],*packs_label,*difficulty;
    
    ClippingNode * clip;
    
    Sprite *overlay,*spinner;
    void setLoading(bool);
    
};



#endif
