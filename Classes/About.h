//
//  Menu1.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __ABOUT_H__
#define __ABOUT_H__

#include "cocos2d.h"
using namespace cocos2d;

class About :public cocos2d::LayerColor{
public:;
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(About);
    
    void menuBackClick(cocos2d::Ref* pSender);
    void menuHowtoClick(cocos2d::Ref* pSender);
    void menuFeedbackClick(cocos2d::Ref* pSender);
    void menuReviewClick(cocos2d::Ref* pSender);
    
    Sprite *about,*contact,*instructions;
    int semn;
    ClippingNode * clip;
    
    MenuItemSprite *back,*back2,*howto,*review,*feedback,*review2;
    Menu *howto2,*menuFeedback,*menuReview;
    LabelTTF *packs_label;
};



#endif
