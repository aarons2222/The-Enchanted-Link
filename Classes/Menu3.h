//
//  Menu3.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//


#ifndef __MENU3_H__
#define __MENU3_H__

#include "cocos2d.h"
#include <iostream>

using namespace std;
using namespace cocos2d;

class Menu3 :public cocos2d::LayerColor{
public:
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(Menu3);
    
    void menuBackClick(cocos2d::Ref* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    virtual void update(float dt);
    
    int pack;
    LabelTTF *packs,*difficulty;
    MenuItemSprite *back;
    
    float completed_pos[75],grid_pos[3];
    Sprite *hover[75];
    
    Node *completed[75];
    int current_pack[75],completed_state[75];
    Sprite *grid[3];
    
    vector < pair < Sprite*, int> > stele;
    
    Node *test2;
    Node *test;
    static float hscroll;
    float snap_position,snap_from;
    int swipeTolerance,swipeStart,direction,is_scrolling;
    cocos2d::Point scroll_limits;
    
    float start_x;
    
    void delayedLoad(float dt);
    
    Node *pagination;
    Sprite *pag[3];
};

#endif