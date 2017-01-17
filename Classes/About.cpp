//
//  About.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "About.h"
#include "GameManager.h"

Scene* About::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = About::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}


bool About::init()
{
    //////////////////////////////
    // 1. super init first
    
    if ( !LayerColor::initWithColor(GameManager::getInstance()->background_color))
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    auto container = Sprite::create("chenar.png");
    container->setPosition(roundf(visibleSize.width/2), roundf(visibleSize.height*0.55));
    container->setVisible(false);
    addChild(container);
    
    // Back button //
    auto backSprite=Sprite::create("back.png");
    backSprite->setFlippedY(false);
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(About::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2-container->getContentSize().width*0.02), visibleSize.height*0.95));
    this->addChild(menuBack);
    
    // About text //
    packs_label = LabelTTF::create("About", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*48);
    packs_label->setHorizontalAlignment(TextHAlignment::LEFT);
    packs_label->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), visibleSize.height*0.955));
    packs_label->setColor(GameManager::getInstance()->color2);
    packs_label->setAnchorPoint(Point(0, 0.55));
    //packs_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    //label->getTexture()->setAliasTexParameters();
    // position the label on the center of the screen
  
    // add the label as a child to this layer
    this->addChild(packs_label);

    // About Version //
    about=Sprite::create("about_version.png");

    //about->getTexture()->setAliasTexParameters();

    about->setAnchorPoint(Point(0.01, 0.4));
    about->setPosition(Point(packs_label->getPositionX(), container->getPositionY()+container->getContentSize().height/2-about->getContentSize().height/2));
    about->setFlippedY(false);
    this->addChild(about);
    
    // About Contact -- Contact is integrated with Version
    //contact=Sprite::create("about_contact.png");

    //contact->getTexture()->setAliasTexParameters();
    //contact->setAnchorPoint(Point(0, 0.5));
    //contact->setPosition(Point(packs_label->getPositionX(), about->getPositionY()-about->getContentSize().height/2-contact->getContentSize().height/2));
    //contact->setFlippedY(false);
    //contact->setColor(GameManager::getInstance()->color2);
    //this->addChild(contact);
    
    
    
    // About How To //
    auto howtoSprite=Sprite::create("about_howto.png");
    //howtoSprite->getTexture()->setAliasTexParameters();
    howtoSprite->setFlippedY(false);
    
    auto howtoSprite2=Sprite::create("about_howto.png");
    //howtoSprite2->getTexture()->setAliasTexParameters();
    howtoSprite2->setFlippedY(false);
    howtoSprite2->setColor(GameManager::getInstance()->color2);
    
    
    back2=MenuItemSprite::create(howtoSprite,howtoSprite2,CC_CALLBACK_1(About::menuHowtoClick, this));
    back2->setAnchorPoint(Point(-0.52, 4));
    howto2=Menu::create(back2,NULL);
    howto2->setPosition(Point(packs_label->getPositionX(), roundf(container->getPositionY()+container->getContentSize().height*0.08)));
    this->addChild(howto2);
    
    
    
    // About Instructions //
    instructions=Sprite::create("about_instr.png");

    //instructions->getTexture()->setAliasTexParameters();

    instructions->setAnchorPoint(Point(0, 0));
    instructions->setPosition(Point::ZERO);
    instructions->setFlippedY(false);
    instructions->setColor(GameManager::getInstance()->color2);
    
    
    clip=ClippingNode::create();
    clip->setAnchorPoint(Point(0,1.7));
    clip->addChild(instructions);
    clip->setPosition(Point(packs_label->getPositionX()-instructions->getContentSize().width*0.03, roundf(container->getPositionY() - howtoSprite2->getContentSize().height/2)));
    clip->setContentSize(instructions->getBoundingBox().size);
    
    
    
    auto stencil = DrawNode::create();
    Point rectangle[4];
    
    rectangle[0] = Point(0, 0);
    rectangle[1] = Point(instructions->getBoundingBox().size.width, 0);
    rectangle[2] = Point(instructions->getBoundingBox().size.width, instructions->getBoundingBox().size.height);
    rectangle[3] = Point(0, instructions->getBoundingBox().size.height);
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clip->setStencil(stencil);
    //clip->setStencil(instructions);
    //clip->addChild(instructions);
    this->addChild(clip);
    
    
    

    
    auto move=MoveBy::create(0, Point(0,instructions->getBoundingBox().size.height));
    instructions->runAction(move);
    
    
    // About Feedback //
    auto feedbackSprite=Sprite::create("about_feedback.png");
    //feedbackSprite->getTexture()->setAliasTexParameters();
    feedbackSprite->setFlippedY(false);
    
    
    auto feedbackSprite2=Sprite::create("about_feedback.png");
    //feedbackSprite->getTexture()->setAliasTexParameters();
    feedbackSprite2->setFlippedY(false);
    feedbackSprite2->setColor(GameManager::getInstance()->color2);
    
    feedback=MenuItemSprite::create(feedbackSprite,feedbackSprite2,CC_CALLBACK_1(About::menuFeedbackClick, this));
    feedback->setAnchorPoint(Point(-0.52, 4));
    menuFeedback=Menu::create(feedback,NULL);
    menuFeedback->setPosition(Point(packs_label->getPositionX(), roundf(howto2->getPositionY()-howtoSprite->getContentSize().height/2 - feedbackSprite->getContentSize().height)));
    this->addChild(menuFeedback);
    
    // About Review //
    auto reviewSprite=Sprite::create("about_review.png");
    //reviewSprite->getTexture()->setAliasTexParameters();
    reviewSprite->setFlippedY(false);
    

    auto reviewSprite2=Sprite::create("about_review.png");
    //reviewSprite2->getTexture()->setAliasTexParameters();
    reviewSprite2->setFlippedY(false);
    reviewSprite2->setColor(GameManager::getInstance()->color2);
    
    review2=MenuItemSprite::create(reviewSprite,reviewSprite2,CC_CALLBACK_1(About::menuReviewClick, this));
    review2->setAnchorPoint(Point(-0.35, 7.5));
    menuReview=Menu::create(review2,NULL);
    menuReview->setPosition(Point(packs_label->getPositionX(), roundf(menuFeedback->getPositionY()-feedbackSprite->getContentSize().height/2 - reviewSprite->getContentSize().height/2)));
    this->addChild(menuReview);
    
    semn=-1;
    
    
    auto move2=MoveBy::create(0.6, Point(0,-visibleSize.height/2));
    //auto easing=EaseBackInOut ::create(move2);
    
    
    back->setPositionY(back->getPositionY()+visibleSize.height/2);
    packs_label->setPositionY(packs_label->getPositionY()+visibleSize.height/2);
    
    
    back->runAction(move2->clone());
    packs_label->runAction(move2->clone());

    
    return true;
}

void About::menuHowtoClick(cocos2d::Ref *pSender){
    auto move=MoveBy::create(1, Point(0,instructions->getBoundingBox().size.height*0.88*semn));
    auto easing=EaseQuarticActionOut::create(move);
    

    instructions->runAction(easing->clone());
    menuFeedback->runAction(easing->clone());
    menuReview->runAction(easing->clone());
    
    semn=-semn;
    GameManager::getInstance()->playSound(1);
}

void About::menuBackClick(cocos2d::Ref *pSender){
    
    auto move=MoveBy::create(0.3, Point(0,Director::getInstance()->getVisibleSize().height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    
    
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    
    GameManager::getInstance()->setScene(1);
    GameManager::getInstance()->playSound(1);
}


void About::menuFeedbackClick(cocos2d::Ref *pSender){
    GameManager::getInstance()->playSound(1);
    GameManager::getInstance()->sendFeedback();
}

void About::menuReviewClick(cocos2d::Ref *pSender){
    GameManager::getInstance()->playSound(1);
    GameManager::getInstance()->reviewApp();
}
