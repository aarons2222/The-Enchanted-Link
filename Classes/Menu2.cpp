//
//  Menu1.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Menu2.h"
#include "GameManager.h"



Scene* Menu2::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Menu2::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}

float Menu2::scroll=0;


bool Menu2::init()
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
    addChild(container);
    
    
    //create the back button
    auto backSprite=Sprite::create("back.png");
    backSprite->setFlippedY(false);
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(Menu2::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2-container->getContentSize().width*0.02), visibleSize.height*0.95));
    this->addChild(menuBack);
    
    
    //create the label with the menu name
	packs_label=Label::createWithSystemFont("Choose a Level Pack",GameManager::getInstance()->font_name,GameManager::getInstance()->font_scale*42);
	packs_label->setHorizontalAlignment(TextHAlignment::LEFT);
    packs_label->setColor(GameManager::getInstance()->color2);
	packs_label->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), visibleSize.height*0.955));
    packs_label->setAnchorPoint(Point(0, 0.60));
    this->addChild(packs_label, 1);

    
    
    
    //we need a custon touch listener for the scrolling algorithm
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=CC_CALLBACK_2(Menu2::onTouchBegan, this);
    touchListener->onTouchMoved=CC_CALLBACK_2(Menu2::onTouchMoved, this);
    touchListener->onTouchEnded=CC_CALLBACK_2(Menu2::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    //create a clipping node for scrolling of the packs
    //any content oudside this will be hidden
    clip=ClippingNode::create();
    clip->setAnchorPoint(Point(0.5,0.5));
    
    //set the clipping node position
   
    /*if(visibleSize.width>=768)
        clip->setPosition(0, visibleSize.height*.51);
    else*/
        clip->setPosition(Vec2(container->getPositionX(),container->getPositionY()));
    
    //the y coordinate where each pack will be added to the clipping node
    float start;
    
    
    
    //initialize the first pack
    
   /* if(visibleSize.width>=768)
        packs[0]=Sprite::create("pack_1_1.png");
    else*/
        packs[0]=Sprite::create("pack_1.png");
   
    
    //initialize the first pack border
    /*
    if(visibleSize.width>=768)
        packs_bg[0]=Sprite::create("packs_bg_1.png");
    else*/
        packs_bg[0]=Sprite::create("packs_bg.png");

    
    
    //set the size of the clipping nod to have place exactly for 5 packs
    Size s;
    s.width=packs_bg[0]->getBoundingBox().size.width;
    s.height=(packs_bg[0]->getBoundingBox().size.height+1)*5;
    clip->setContentSize(s);
    
    
    //set the start variable so that the second pack comes below the first
    start=(packs_bg[0]->getBoundingBox().size.height)*4+3;
    packs[0]->setPosition(0, start);
    packs[0]->setAnchorPoint(Point(0, 0));
    
    //set the pack border too
    packs_bg[0]->setPosition(packs[0]->getPosition());
    packs_bg[0]->setAnchorPoint(Point(0, 0));

    //initialize the current pressed tab
    tab_pressed_id=0;
    
    //add the pack and the pack border to the clipping node
    clip->addChild(packs_bg[0]);
    clip->addChild(packs[0]);
    
    //set the current position of the first pack
    packs_pos[0]=start;
    
    
    //will hold each pack progress and the number of completed levels
    int current_pack[75],current_pack_completed=0;
    
    //get the progress from the save for first pack
    GameManager::getInstance()->readSave(0, current_pack);
    
    //assume its perfect
    int perfect=1;
    
    //check if perfect
    for(int i=0;i<75;i++){
        if(current_pack[i]>1)
            current_pack_completed++;
        if(current_pack[i]<4)
            perfect=0;
    }
    
    //will hold the current progress for each pack (0/75)
    char completed_buffer[100];
    
    //set the progress in the string variable
    sprintf(completed_buffer, "%d/75",current_pack_completed);
    
    
    //create the completed levels label
    completed[0] = LabelTTF::create(completed_buffer,GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*23);
    completed[0]->setAnchorPoint(Point(1, 0.5));
    completed[0]->setHorizontalAlignment(TextHAlignment::RIGHT);
    completed[0]->setPosition(Point(packs[0]->getPositionX()+s.width*0.93, packs[0]->getPositionY()+packs[0]->getBoundingBox().size.height*0.34));
    completed[0]->setAnchorPoint(Point(1, 0.5));
    
    //add it to the clipping node
    clip->addChild(completed[0]);
    
    //retain default position
    completed_pos[0]=completed[0]->getPositionY();
    
    
    //if the pack was completed perfectly display a star
    if(perfect){
        auto star=Sprite::create("levels_star.png");
        star->setFlippedY(false);
        star->setAnchorPoint(Point(1, 0.5));
        star->setPosition(packs[0]->getPositionX()+s.width*0.95, packs[0]->getPositionY()+packs[0]->getBoundingBox().size.height*0.7);
        clip->addChild(star);
        pair<Sprite*, int> p(star,star->getPositionY());
        stele.push_back(p);
    }
    perfect=1;
    
    //get the bought packs
    GameManager::getInstance()->getUnlockedPacks(unlocked);
    
    //set up the rest of 9 packs relativly to the first one
    //this is simillar to the first pack but the positions are calculated automaticaly
    
    for(int i=1;i<10;i++){
        
        start-=packs_bg[0]->getBoundingBox().size.height+1;
        
        char buffer[256];
        
       /* if(visibleSize.width>=768)
            sprintf(buffer,"pack_%d_1.png",i+1);
        else*/
            sprintf(buffer,"pack_%d.png",i+1);


        
        /*if(visibleSize.width>=768)
            packs_bg[i]=Sprite::create("packs_bg_1.png");
        else*/
            packs_bg[i]=Sprite::create("packs_bg.png");
        
        
        packs_bg[i]->setPosition(0, start);
        packs_bg[i]->setAnchorPoint(Point(0, 0));
        
        
        packs[i]=Sprite::create(buffer);
        packs[i]->setFlippedY(false);
        packs[i]->setPosition(0, start);
        packs[i]->setAnchorPoint(Point(0, 0));
        
        clip->addChild(packs_bg[i]);
        clip->addChild(packs[i]);
        
        packs_pos[i]=start;
        
        
        
        GameManager::getInstance()->readSave(i, current_pack);
        current_pack_completed=0;
        for(int k=0;k<75;k++){
            if(current_pack[k]>1)
                current_pack_completed++;
            if(current_pack[k]<4)
                perfect=0;
        }
        
        if(perfect){
            auto star=Sprite::create("levels_star.png");
            star->setFlippedY(false);
            star->setAnchorPoint(Point(1, 0.5));
            star->setPosition(packs[i]->getPositionX()+s.width*0.935, packs[i]->getPositionY()+packs[i]->getBoundingBox().size.height*0.7);
            clip->addChild(star);
            pair<Sprite*, int> p(star,star->getPositionY());
            stele.push_back(p);
        }
        perfect=1;
        
        //check if the user has purchased the pack, if not show a lock on it
        if(i>4 && !unlocked[i-5]){
            auto lock=Sprite::create("levels_0.png");
            lock->setFlippedY(false);
            lock->setAnchorPoint(Point(1, 0.5));
            lock->setPosition(packs[i]->getPositionX()+s.width, packs[i]->getPositionY()+packs[i]->getBoundingBox().size.height*0.66);
            clip->addChild(lock);
            pair<Sprite*, int> p(lock,lock->getPositionY());
            locks.push_back(p);

        }
        
        sprintf(completed_buffer, "%d/75",current_pack_completed);
        
        
        completed[i] = LabelTTF::create(completed_buffer, GameManager::getInstance()->font_name,GameManager::getInstance()->font_scale* 23);
        completed[i]->setAnchorPoint(Point(1, 0.5));
        completed[i]->setHorizontalAlignment(TextHAlignment::RIGHT);
        completed[i]->setPosition(Point(packs[i]->getPositionX()+s.width*0.93, packs[i]->getPositionY()+packs[0]->getBoundingBox().size.height*0.34));
        
        clip->addChild(completed[i]);
        
        completed_pos[i]=completed[i]->getPositionY();
        
    }
    
    
    //create a stencil buffer for the clipping node
    auto stencil = DrawNode::create();
    //4 points to hold the stencil rectangle coordinates
    Point rectangle[4];
    
    //set the coordinates to the clipping node size
    rectangle[0] = Point(0, 0);
    rectangle[1] = Point(s.width, 0);
    rectangle[2] = Point(s.width, s.height);
    rectangle[3] = Point(0, s.height);
    
    //configure the stencil
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    
    //add the stencil to the clip
    clip->setStencil(stencil);
    
    //add the clip to the main scene
    this->addChild(clip);
    
    
    

    //momentum scrolling settings
    
    //schedule for a callback on each frame
    
    this->scheduleUpdate();
    
    //set default variables for still scroll
    is_scrolling=0;
    is_autoscrolling=0;
    velocity=0;
    scroll_timer=0;
    
    //set the limits for the scroll
    limite=Point(0,packs[9]->getPositionY()-packs[0]->getPositionY());
    
    limite.y=(packs[0]->getPositionY()-packs[9]->getPositionY())-(packs[0]->getPositionY()-packs[4]->getPositionY())-(packs[1]->getPositionY()-packs[0]->getPositionY())-packs[0]->getBoundingBox().size.height;
    
    
    //create a slide down animation
    auto move=MoveBy::create(0.6, Point(0,-visibleSize.height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    
    //set the position of the back button and label over the screen
    back->setPositionY(back->getPositionY()+visibleSize.height/2);
    packs_label->setPositionY(packs_label->getPositionY()+visibleSize.height/2);
    
    //make them slide down
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    
    
    
    return true;
}


//callback for each frame
//use it to update the position of the packs if the user scrolls

void Menu2::update(float dt){
    

    //if the user is not scrolling, create a slowdown in the movement, don't just stop
    
    if(!is_scrolling){
        velocity*=0.96;
        scroll+=velocity;
        last_scroll+=velocity;
    }
    
    
    //make the packs go back to the top position if the user exceded it
    if(!is_scrolling && scroll<limite.x){
        scroll=LinearInterpolation(scroll_timer,last_scroll,limite.x)+velocity/10;
        scroll_timer+=0.07993;
    }
    
    //make the packs go back to the bottom position if the user excedded it
    if(!is_scrolling && scroll>limite.y){
        scroll=LinearInterpolation(1-scroll_timer,limite.y,last_scroll)+velocity/10;
        scroll_timer+=0.07993;
    }
    
    
    //update the packs, completed level and stars/locks position
    if(!is_scrolling){
        for(int i=0;i<10;i++){
            packs[i]->setPosition(Point(packs[i]->getPositionX(), packs_pos[i]+scroll));
            completed[i]->setPosition(Point(completed[i]->getPositionX(), completed_pos[i]+scroll));
            packs_bg[i]->setPosition(Point(packs[i]->getPositionX(), packs_pos[i]+scroll));
        }
        for(int i=0;i<stele.size();i++)
            stele[i].first->setPosition(stele[i].first->getPositionX(),stele[i].second+scroll);
        for(int i=0;i<locks.size();i++)
            locks[i].first->setPosition(locks[i].first->getPositionX(),locks[i].second+scroll);
    }

    
}

long Menu2::get_milli_time(){
    struct timeval tv;
    
	gettimeofday( &tv, NULL );
    
    return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}

//user touched the back buttton
void Menu2::menuBackClick(cocos2d::Ref *pSender){
    
    //create a slide up animation
    auto move=MoveBy::create(0.3, Point(0,Director::getInstance()->getVisibleSize().height/2));
    
    //slide the back button and label up
    back->runAction(move->clone());
    packs_label->runAction(move->clone());
    
    //set the scene to main menu
    GameManager::getInstance()->setScene(1);
    GameManager::getInstance()->playSound(1);
}


//user has started scrolling
bool Menu2::onTouchBegan(Touch *touch, Event *unused_event){
    
    //reset time, speed and velocity variables
    speed[0]=speed[1]=speed[2]=speed[3]=touch->getLocation().y;
    time[0]=time[1]=time[2]=time[3]=get_milli_time();
    velocity=0;
    
    
    //get the touch location
    Point location=clip->convertTouchToNodeSpace(touch);
    
    //if has touched any pack, make it pressed by changing border texture and
    for(int i=0;i<10;i++)
        if(packs[i]->getBoundingBox().containsPoint(location)){
            tab_pressed_id=i;
            
            /*if(Director::getInstance()->getVisibleSize().width>=768)
                packs_bg[i]->setTexture("packs_bg2_1.png");
            else*/
                packs_bg[i]->setTexture("packs_bg2.png");
            tab_state[i]=1;
            packs[i]->setColor(Color3B(GameManager::getInstance()->background_color));
            completed[i]->setColor(Color3B(GameManager::getInstance()->background_color));
            break;
        }
    
    return true;
}

//the user is scrolling
void Menu2::onTouchMoved(Touch *touch, Event *unused_event){
    
    //mark that the user is scrolling
    is_scrolling=1;
    
    //update the scroll variable with the new position
    scroll+=touch->getDelta().y;
    
    //scroll the elements in the clipping node
    for(int i=0;i<stele.size();i++)
        stele[i].first->setPosition(stele[i].first->getPositionX(),stele[i].second+scroll);
    for(int i=0;i<locks.size();i++)
        locks[i].first->setPosition(locks[i].first->getPositionX(),locks[i].second+scroll);
    
    for(int i=0;i<10;i++){
        packs[i]->setPosition(Point(packs[i]->getPositionX(), packs_pos[i]+scroll));
        completed[i]->setPosition(Point(completed[i]->getPositionX(), completed_pos[i]+scroll));
        packs_bg[i]->setPosition(Point(packs[i]->getPositionX(), packs_pos[i]+scroll));
    }
    
    
    is_scrolling=1;
    is_autoscrolling=0;
    
    //update the scroll time
    time[0]=time[1];
    time[1]=time[2];
    time[2]=time[3];
    time[3]=get_milli_time();
    
    //update the scroll speed
    speed[0]=speed[1];
    speed[1]=speed[2];
    speed[2]=speed[3];
    speed[3]=touch->getLocation().y;

   
    
}

//user has stopped scrolling
void Menu2::onTouchEnded(Touch *touch, Event *unused_event){

    //get the time
    long timec=get_milli_time();
    
    //stop scrolling if the time is too small compared to the last one
    if(timec-time[3]>230)
        velocity=0;
    else
        if(is_scrolling && speed[0]!=speed[3] )
        {//prepare to start the momentum scrolling
            float diff=touch->getLocation().y-speed[0];
            float timen=(timec-time[0]);
            velocity=diff/timen*10;
            velocity=CLAMP(velocity, -65,65);
        }

   
    //release the pressed pack if any
   /* if(Director::getInstance()->getVisibleSize().width>=768)
        packs_bg[tab_pressed_id]->setTexture("packs_bg_1.png");
    else*/
        packs_bg[tab_pressed_id]->setTexture("packs_bg.png");
    
    //update the released pack colors if any
    //packs[tab_pressed_id]->setColor(Color3B(GameManager::getInstance()->color2));
    //completed[tab_pressed_id]->setColor(Color3B(GameManager::getInstance()->color2));
    
    //get the touch location
    Point location=clip->convertTouchToNodeSpace(touch);
    
    for(int i=0;i<10;i++)
        if(packs[i]->getBoundingBox().containsPoint(location)){
            if(tab_state[i] && fabs(velocity)<1){
                //if the user has selected a locked pack show an alert to buy it
                if( i>4 && !unlocked[i-5]){
                    tab_state[i]=0;
                    GameManager::getInstance()->showPackLockedAlert();
                    return;
                }
                
                //the pack is free or user has bought it
                
                //create slide up animation
                auto move=MoveBy::create(0.3, Point(0,Director::getInstance()->getVisibleSize().height/2));
                
                back->runAction(move->clone());
                packs_label->runAction(move->clone());

                //go to the next screen
                Menu3::hscroll=0;
                GameManager::getInstance()->setPack(i);
                GameManager::getInstance()->setScene(6);
                GameManager::getInstance()->playSound(1);
            }
            tab_state[i]=0;
        }
    
    //update scroll variables
    last_scroll=scroll;
    is_scrolling=0;
    
    if(scroll_timer>1)
        scroll_timer=0;
    
}
