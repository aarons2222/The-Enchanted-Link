//
//  Menu1.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __MENU2_H__
#define __MENU2_H__

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
#define CLAMP(X, A, B) ((X < A) ? A : ((X > B) ? B : X))

class Menu2 :public cocos2d::LayerColor{
public:
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(Menu2);
    
    void menuBackClick(cocos2d::Ref* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    //momentum scrolling
    virtual void update(float dt);
    float is_scrolling,is_autoscrolling,speed[4],velocity;
    static float scroll;
    float last_scroll,scroll_timer;
    long time[4];
    long get_milli_time();
    cocos2d::Point limite;
    
    MenuItemSprite *back;
    Sprite *packs[10],*packs_bg[10];
    float packs_pos[10],completed_pos[10];
    int tab_pressed_id,tab_state[10];
    LabelTTF *completed[10];
	Label *packs_label;
    vector < pair < Sprite*, int> > stele;
    vector < pair < Sprite*, int> > locks;
    
    ClippingNode * clip;
    int unlocked[5];
};

#endif
