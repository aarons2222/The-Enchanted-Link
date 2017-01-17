//
//  Store.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Store.h"
#include "GameManager.h"

Scene* Store::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Store::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}


bool Store::init()
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
    
    auto backSprite=Sprite::create("back.png");
    backSprite->setFlippedY(false);
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(Store::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2-container->getContentSize().width*0.02), visibleSize.height*0.95));
    this->addChild(menuBack);
    
    
    packs_label = LabelTTF::create("Store", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*48);
    packs_label->setHorizontalAlignment(TextHAlignment::LEFT);
    packs_label->setPosition(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), roundf(visibleSize.height*0.955));
    packs_label->setAnchorPoint(Point(0, 0.55));
    packs_label->setColor(GameManager::getInstance()->color2);
    //packs_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    //label->getTexture()->setAliasTexParameters();
    // position the label on the center of the screen
    
    // add the label as a child to this layer
    this->addChild(packs_label);
    
    
    difficulty = LabelTTF::create("Buy any level pack to remove the ads", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*28);
    difficulty->setHorizontalAlignment(TextHAlignment::LEFT);
    difficulty->setPosition(Point(packs_label->getPositionX()+3, roundf(visibleSize.height*0.9)) );
    difficulty->setColor(GameManager::getInstance()->color2);
    difficulty->setAnchorPoint(Point(0, 0.5));
    //difficulty->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);

    
    this->addChild(difficulty);
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=CC_CALLBACK_2(Store::onTouchBegan, this);
    touchListener->onTouchMoved=CC_CALLBACK_2(Store::onTouchMoved, this);
    touchListener->onTouchEnded=CC_CALLBACK_2(Store::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    
    clip=ClippingNode::create();
    clip->setAnchorPoint(Point(0.5,0.5));
    
    
    /*if(visibleSize.width>=768)
        clip->setPosition(0, visibleSize.height*.51);
    else*/
        clip->setPosition(Vec2(container->getPositionX(),container->getPositionY()));

    
    float start;
    
    
    
    
    /*if(visibleSize.width>=768)
        packs[0]=Sprite::create("store_1_1.png");
    else*/
        packs[0]=Sprite::create("store_1.png");
    

    //packs[0]->getTexture()->setAliasTexParameters();
    packs[0]->setFlippedY(false);
    
    
    /*if(visibleSize.width>=768)
        packs_bg[0]=Sprite::create("packs_bg_1.png");
    else*/
        packs_bg[0]=Sprite::create("packs_bg.png");
    //packs_bg[0]->getTexture()->setAliasTexParameters();
    
    
    
    Size s;
    s.width=packs_bg[0]->getBoundingBox().size.width;
    s.height=(packs_bg[0]->getBoundingBox().size.height+1)*5;
    clip->setContentSize(s);
    
    
    
    start=(packs_bg[0]->getBoundingBox().size.height)*4+3;
    packs[0]->setPosition(0, start);
    packs[0]->setAnchorPoint(Point(0, 0));
    
    
    packs_bg[0]->setPosition(packs[0]->getPosition());
    packs_bg[0]->setAnchorPoint(Point(0, 0));
    
    tab_pressed_id=0;
    clip->addChild(packs_bg[0]);
    clip->addChild(packs[0]);
    
    
    packs_pos[0]=start;
    
    

    
    prices[0] = LabelTTF::create(GameManager::getInstance()->preturi[0],GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*23);
    prices[0]->setAnchorPoint(Point(1, 0.5));
    prices[0]->setHorizontalAlignment(TextHAlignment::RIGHT);
    prices[0]->setPosition(packs[0]->getPositionX()+s.width*0.93, packs[0]->getPositionY()+packs[0]->getBoundingBox().size.height*0.34);
    prices[0]->setColor(Color3B(GameManager::getInstance()->color2));
    //prices[0]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    clip->addChild(prices[0]);
    prices_pos[0]=prices[0]->getPositionY();

    
    
    
    for(int i=1;i<10;i++){
        
        
        start-=packs_bg[0]->getBoundingBox().size.height+1;
        
        char buffer[256];
        
        //i+1
        /*if(visibleSize.width>=768)
            sprintf(buffer,"store_%d_1.png",i+1);
        else*/
            sprintf(buffer,"store_%d.png",i+1);
        
        
        
        /*if(visibleSize.width>=768)
            packs_bg[i]=Sprite::create("packs_bg_1.png");
        else*/
            packs_bg[i]=Sprite::create("packs_bg.png");
        

        
        
        //packs_bg[i]->getTexture()->setAliasTexParameters();
        packs_bg[i]->setPosition(0, start);
        packs_bg[i]->setAnchorPoint(Point(0, 0));
        
        
        
        packs[i]=Sprite::create(buffer);
        //packs[i]->getTexture()->setAliasTexParameters();
        packs[i]->setFlippedY(false);
        packs[i]->setPosition(0, start);
        packs[i]->setAnchorPoint(Point(0, 0));
        clip->addChild(packs_bg[i]);
        clip->addChild(packs[i]);
        packs_pos[i]=start;
        
        
        
        prices[i] = LabelTTF::create(GameManager::getInstance()->preturi[i],GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*23);
        prices[i]->setAnchorPoint(Point(1, 0.5));
        prices[i]->setHorizontalAlignment(TextHAlignment::RIGHT);
        prices[i]->setPosition(packs[0]->getPositionX()+s.width*0.93, packs[i]->getPositionY()+packs[i]->getBoundingBox().size.height*0.34);
        prices[i]->setAnchorPoint(Point(1, 0.5));
        //prices[i]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
        clip->addChild(prices[i]);
        prices_pos[i]=prices[i]->getPositionY();

        
        
    }
    
    
    
    auto stencil = DrawNode::create();
    Point rectangle[4];
    
    rectangle[0] = Point(0, 0);
    rectangle[1] = Point(s.width, 0);
    rectangle[2] = Point(s.width, s.height);
    rectangle[3] = Point(0, s.height);
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clip->setStencil(stencil);
    this->addChild(clip);
    
    
    
    
    //momentum scrolling
    this->scheduleUpdate();
    is_scrolling=0;
    is_autoscrolling=0;
    velocity=0;
    scroll=0;
    scroll_timer=0;
    //limite=Point(clip->getBoundingBox().size.height-packs[0]->getBoundingBox().size.height,0);
    
    limite=Point(0,packs[9]->getPositionY()-packs[0]->getPositionY());
    
    
    limite.y=(packs[0]->getPositionY()-packs[9]->getPositionY())-(packs[0]->getPositionY()-packs[4]->getPositionY())-(packs[1]->getPositionY()-packs[0]->getPositionY())-packs[0]->getBoundingBox().size.height;
    
    
    
    
    
    overlay=Sprite::create("finish_overlay.png");
    overlay->setPosition(visibleSize.width/2, visibleSize.height/2);
    overlay->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    overlay->setVisible(false);
    this->addChild(overlay,1000);
    
    spinner=Sprite::create("spinner.png");
    spinner->setPosition(visibleSize.width/2, visibleSize.height/2);
    spinner->setVisible(false);
    this->addChild(spinner,1001);
    
    
    
    auto move=MoveBy::create(0.6, Point(0,-visibleSize.height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    back->setPositionY(back->getPositionY()+visibleSize.height/2);
    packs_label->setPositionY(packs_label->getPositionY()+visibleSize.height/2);
    difficulty->setPositionY(difficulty->getPositionY()+visibleSize.height/2);
    
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    difficulty->runAction(move->clone());
    
    
    return true;
}


void Store::setLoading(bool loading){
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



long Store::get_milli_time(){
    struct timeval tv;
    
	gettimeofday( &tv, NULL );
    
    return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}



void Store::update(float dt){
    
    if(!is_scrolling){
        velocity*=0.96;
        scroll+=velocity;
        last_scroll+=velocity;
    }
    
    
    
    if(!is_scrolling && scroll<limite.x){
        //velocity=0;
        scroll=LinearInterpolation(scroll_timer,last_scroll,limite.x)+velocity/10;
        scroll_timer+=0.07993;
    }
    
    if(!is_scrolling && scroll>limite.y){
        //velocity=0;
        scroll=LinearInterpolation(1-scroll_timer,limite.y,last_scroll)+velocity/10;
        scroll_timer+=0.07993;
    }
    
    
    
    if(!is_scrolling){
        for(int i=0;i<10;i++){
            packs[i]->setPosition(packs[i]->getPositionX(), packs_pos[i]+scroll);
            prices[i]->setPosition(prices[i]->getPositionX(), prices_pos[i]+scroll);
            packs_bg[i]->setPosition(packs[i]->getPositionX(), packs_pos[i]+scroll);
        }
    }
    

}


bool Store::onTouchBegan(Touch *touch, Event *unused_event){
    
    
    if(overlay->isVisible())
        return true;
    
    speed[0]=speed[1]=speed[2]=speed[3]=touch->getLocation().y;
    time[0]=time[1]=time[2]=time[3]=get_milli_time();
    velocity=0;
    
    
    
    Point location=clip->convertTouchToNodeSpace(touch);
    
    for(int i=0;i<10;i++)
        if(packs[i]->getBoundingBox().containsPoint(location)){
            tab_pressed_id=i;
            /*if(Director::getInstance()->getVisibleSize().width>=768)
                packs_bg[i]->setTexture("packs_bg2_1.png");
            else*/
                packs_bg[i]->setTexture("packs_bg2.png");
            tab_state[i]=1;
            packs[i]->setColor(Color3B(GameManager::getInstance()->background_color));
            prices[i]->setColor(Color3B(GameManager::getInstance()->background_color));
            break;
        }

    
    
    return true;
}

void Store::onTouchMoved(Touch *touch, Event *unused_event){
    
    if(overlay->isVisible())
        return ;
    scroll+=touch->getDelta().y;
    
    for(int i=0;i<10;i++){
        packs[i]->setPosition(packs[i]->getPositionX(), packs_pos[i]+scroll);
        prices[i]->setPosition(prices[i]->getPositionX(), prices_pos[i]+scroll);
        packs_bg[i]->setPosition(packs[i]->getPositionX(), packs_pos[i]+scroll);
    }
    //tab_pressed->setPosition(Point(tab_pressed->getPositionX(), packs_pos[tab_pressed_id]+scroll));
    
    is_scrolling=1;
    is_autoscrolling=0;
    time[0]=time[1];
    time[1]=time[2];
    time[2]=time[3];
    
    time[3]=get_milli_time();
    
    speed[0]=speed[1];
    speed[1]=speed[2];
    speed[2]=speed[3];
    
    speed[3]=touch->getLocation().y;

    
    
}

void Store::onTouchEnded(Touch *touch, Event *unused_event){
    
    if(overlay->isVisible())
        return ;
    
    long timec=get_milli_time();
    
    if(timec-time[3]>230)
        velocity=0;
    else
        if(is_scrolling && speed[0]!=speed[3] )
        {
            float diff=touch->getLocation().y-speed[0];
            float timen=(timec-time[0]);
            velocity=diff/timen*10;
            velocity=CLAMP(velocity, -65,65);
        }
    
    
    
    
    
    //tab_pressed->setVisible(false);
    
    /*if(Director::getInstance()->getVisibleSize().width>=768)
        packs_bg[tab_pressed_id]->setTexture("packs_bg_1.png");
    else*/
        packs_bg[tab_pressed_id]->setTexture("packs_bg.png");
    
    packs[tab_pressed_id]->setColor(Color3B(GameManager::getInstance()->color2));
    prices[tab_pressed_id]->setColor(Color3B(GameManager::getInstance()->color2));
    
    Point location=clip->convertTouchToNodeSpace(touch);
    
    for(int i=0;i<10;i++)
        if(packs[i]->getBoundingBox().containsPoint(location)){
            if(tab_state[i] && fabs(velocity)<1){
                GameManager::getInstance()->buyItem(i);
                /*Menu3::hscroll=0;
                GameManager::getInstance()->setPack(i);
                GameManager::getInstance()->setScene(6);
                GameManager::getInstance()->playSound(1);*/
            }
            tab_state[i]=0;
        }

    
    last_scroll=scroll;
    is_scrolling=0;
    
    if(scroll_timer>1)
        scroll_timer=0;
}


void Store::menuBackClick(cocos2d::Ref *pSender){
    if(overlay->isVisible())
        return;
    
    
    
    auto move=MoveBy::create(0.3, Point(0,Director::getInstance()->getVisibleSize().height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    
    
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    difficulty->runAction(move->clone());
    
    GameManager::getInstance()->setScene(1);
    GameManager::getInstance()->playSound(1);
}

