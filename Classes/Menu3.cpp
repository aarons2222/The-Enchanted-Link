//
//  Menu3.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Menu3.h"
#include "GameManager.h"

Scene* Menu3::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Menu3::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}

float Menu3::hscroll=0;


bool Menu3::init()
{
    //////////////////////////////
    // 1. super init first
    
    if ( !LayerColor::initWithColor(GameManager::getInstance()->background_color))
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto container = Sprite::create("chenar.png");
    container->setPosition(roundf(visibleSize.width/2), roundf(visibleSize.height*0.55));
    container->setVisible(false);
    addChild(container);
    
       

    //create the back button
    auto backSprite=Sprite::create("back.png");
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(Menu3::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(container->getPositionX()-container->getContentSize().width/2-container->getContentSize().width*0.02), visibleSize.height*0.95));
    
    this->addChild(menuBack);
    

    
    std::string gr;
    gr="chenar.png";
    
    

    
    
    
    grid[0]=Sprite::create(gr);
    grid[0]->setColor(Color3B(105,129,117));
    grid[0]->setFlippedY(false);

    grid[0]->setPosition(container->getPosition());
    
    grid_pos[0]=grid[0]->getPositionX();
    this->addChild(grid[0]);
    grid[0]->setVisible(false);
    for(int i=1;i<3;i++){
        grid[i]=Sprite::create(gr);
        grid[i]->setColor(Color3B(105,129,117));
        grid[i]->setPosition(grid[i-1]->getPositionX()+grid[i]->getBoundingBox().size.width*1.15, grid[0]->getPositionY());
        grid_pos[i]=grid[i]->getPositionX();
        this->addChild(grid[i]);
        grid[i]->setVisible(false);
    }
    
    
    pagination=Node::create();
    this->addChild(pagination);
    
    pagination->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35),grid[0]->getPositionY()-grid[0]->getBoundingBox().size.height/2*1.15));
    Point pag_start=Point(Point::ZERO);
    
    for(int i=0;i<3;i++){
        pag[i]=Sprite::create("pag_sel.png");
        pag[i]->setColor(GameManager::getInstance()->color2);
        pag[i]->setPosition(pag_start);
        pag_start.x+=pag[i]->getBoundingBox().size.width*2;
        pagination->addChild(pag[i]);
        pag[i]->setOpacity(255/2);
    }
    
    pag[0]->setOpacity(255);
    
    pagination->setPositionX(pagination->getPositionX()+pag[0]->getBoundingBox().size.width/2);
  
    
    
    int index=0;
    
    pack=GameManager::getInstance()->pack;
    
    
    GameManager::getInstance()->readSave(pack, current_pack);
    
    
    test=Node::create();
    test2=Node::create();
    
    this->addChild(test);
    this->addChild(test2);
    
    for(int i=0;i<3;i++){
        int x0=grid[i]->getPositionX()-grid[i]->getBoundingBox().size.width/2,y0=grid[i]->getPositionY()+grid[i]->getBoundingBox().size.height/2;
        int offset=grid[i]->getBoundingBox().size.width/5;
        x0+=offset/2;
        y0-=offset/2;
        
        
        
        for(int j=0;j<25;j++){
                
            if(current_pack[index]){
                LabelTTF *temp=LabelTTF::create(itoa(index+1), GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*33, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
                completed[index]=temp;
            }
            else{
                Sprite* temp=Sprite::create("levels_0.png");
                temp->setFlippedY(false);
                completed[index]=temp;
            }

            
            completed[index]->setPosition(x0+offset*(j%5), y0-offset*(j/5));
            completed[index]->setColor(Color3B(GameManager::getInstance()->color2));
            completed_pos[index]=completed[index]->getPositionX();
            
            
            hover[index]=Sprite::create("cel_11.png");
            hover[index]->getTexture()->setAntiAliasTexParameters() ;
            hover[index]->setPosition(completed[index]->getPosition());
            hover[index]->setColor(GameManager::getInstance()->color2);
            
            /*
            if(visibleSize.width>=768)
                hover[index]->setScale(1.2);
            */
            
            test->addChild(hover[index]);
            
            
            if(current_pack[index]>0){
                
                int width=grid[i]->getBoundingBox().size.width/5;
                
                
                Point star_pos[3];
                star_pos[0]=Point(x0+offset*(j%5)-width*0.22, y0-offset*(j/5)-width*0.3);
                star_pos[1]=Point(x0+offset*(j%5), y0-offset*(j/5)-width*0.3);
                star_pos[2]=Point(x0+offset*(j%5)+width*0.22, y0-offset*(j/5)-width*0.3);
                int k=2;
                if(current_pack[index]==1){
                    for(k=2;k<=4;k++){
                        
                        auto star=Sprite::create("levels_star3.png");
                        star->setAnchorPoint(Point(0.5, 0.5));
                        star->setFlippedY(false);
                        star->setPosition(star_pos[k-2]);
                        
                        test2->addChild(star);
                        
                        pair<Sprite*, int> p(star,star->getPositionX());
                        stele.push_back(p);
                        
                        
                    }

                }
                else{
                    for(k=2;k<=current_pack[index];k++){
                        
                        auto star=Sprite::create("levels_star4.png");
                        star->setAnchorPoint(Point(0.5, 0.5));
                        star->setFlippedY(false);
                        star->setPosition(star_pos[k-2]);
                        
                        test2->addChild(star);
                        
                        pair<Sprite*, int> p(star,star->getPositionX());
                        stele.push_back(p);
                        
                        
                    }
                    
                    for(;k<=4;k++){
                        auto star=Sprite::create("levels_star3.png");
                        star->setAnchorPoint(Point(0.5, 0.5));
                        star->setFlippedY(false);
                        star->setPosition(star_pos[k-2]);
                        
                        test2->addChild(star);
                        
                        pair<Sprite*, int> p(star,star->getPositionX());
                        stele.push_back(p);
                    }
                }
                
            }
          
            
            this->addChild(completed[index++]);
        }
    }
    
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=CC_CALLBACK_2(Menu3::onTouchBegan, this);
    touchListener->onTouchMoved=CC_CALLBACK_2(Menu3::onTouchMoved, this);
    touchListener->onTouchEnded=CC_CALLBACK_2(Menu3::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    char buffer[100];
    
    if(pack<9)
        sprintf(buffer,"Level Pack %d",pack+1);
    else
        sprintf(buffer,"Level Pack %d",pack+1);
    
    
    packs = LabelTTF::create(buffer, GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*48);
    packs->setHorizontalAlignment(TextHAlignment::LEFT);
    packs->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), visibleSize.height*0.955));
    packs->setColor(GameManager::getInstance()->color2);
    packs->setAnchorPoint(Point(0, 0.5));

    
    if(pack<10)
        sprintf(buffer, "%dx%d - Easy",pack+5,pack+5);
    else
        sprintf(buffer, "%dx%d - Easy",pack-5,pack-5);
    
    difficulty = LabelTTF::create(buffer, GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*28);
    difficulty->setHorizontalAlignment(TextHAlignment::LEFT);
    difficulty->setPosition(Point(packs->getPositionX()+3, roundf(visibleSize.height*0.9)));
    difficulty->setColor(GameManager::getInstance()->color2);
    difficulty->setAnchorPoint(Point(0, 0.5));
    //difficulty->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    
    
    // add the label as a child to this layer
    this->addChild(packs);
    this->addChild(difficulty);

    
    
    swipeTolerance=visibleSize.width/4;//how much does the user must scroll to trigger a page change
    scroll_limits.x=0;
    scroll_limits.y=grid[2]->getPositionX();
    is_scrolling=0;
    direction=3;
    
    for(int i=0;i<3;i++)
        grid[i]->setPosition(grid_pos[i]+hscroll,grid[i]->getPositionY());
    for(int i=0;i<75;i++){
        hover[i]->setPosition(completed_pos[i]+hscroll,hover[i]->getPositionY());
        completed[i]->setPosition(completed_pos[i]+hscroll, completed[i]->getPositionY());
    }
    for(int i=0;i<stele.size();i++)
        stele[i].first->setPosition(stele[i].second+hscroll,stele[i].first->getPositionY());

    
    
    
    this->scheduleUpdate();
    
    
    back->setPositionY(back->getPositionY()+visibleSize.height/2);
    packs->setPositionY(packs->getPositionY()+visibleSize.height/2);
    difficulty->setPositionY(difficulty->getPositionY()+visibleSize.height/2);
    
    
    auto move=MoveBy::create(0.6, Point(0,-visibleSize.height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    back->runAction(move->clone());
    packs->runAction(move->clone());
    difficulty->runAction(move);

    
    pagination->setPositionY(pagination->getPositionY()-visibleSize.height/2);
    move=MoveBy::create(0.6, Point(0,visibleSize.height/2));
    //easing=EaseBackInOut ::create(move);
    pagination->runAction(move);
    
    
    
    if(container->getBoundingBox().intersectsRect(grid[0]->getBoundingBox())){
        if(pack<10)
            sprintf(buffer, "%dx%d - Easy",pack+5,pack+5);
        else
            sprintf(buffer, "%dx%d - Easy",pack-5,pack-5);
        difficulty->setString(buffer);
        for(int i=0;i<3;i++){
            pag[i]->stopAllActions();
            pag[i]->setOpacity(255/2);
        }
        pag[0]->setOpacity(255);
    }
    
    if(container->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
        if(pack<10)
            sprintf(buffer, "%dx%d - Medium",pack+5,pack+5);
        else
            sprintf(buffer, "%dx%d - Medium",pack-5,pack-5);
        difficulty->setString(buffer);
        for(int i=0;i<3;i++){
            pag[i]->stopAllActions();
            pag[i]->setOpacity(255/2);
        }
        pag[1]->setOpacity(255);
    }

    
    if(container->getBoundingBox().intersectsRect(grid[2]->getBoundingBox())){
        if(pack<10)
            sprintf(buffer, "%dx%d - Hard",pack+5,pack+5);
        else
            sprintf(buffer, "%dx%d - Hard",pack-5,pack-5);
        difficulty->setString(buffer);
        for(int i=0;i<3;i++){
            pag[i]->stopAllActions();
            pag[i]->setOpacity(255/2);
        }
        pag[2]->setOpacity(255);
    }

    
    return true;
}

float easeOut(float t,float b , float c, float d) {
	float s = 1.70158f;
	return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
}
static float t=0;
void Menu3::update(float dt){
    
    
    static float duration=0.8;
    
    
    if(direction!=3){
        hscroll=easeOut(t, snap_from, snap_position,duration);
        if(t<duration)
            t+=dt;
        for(int i=0;i<3;i++)
            grid[i]->setPosition(grid_pos[i]+hscroll,grid[i]->getPositionY());
        for(int i=0;i<75;i++){
            hover[i]->setPosition(completed_pos[i]+hscroll,hover[i]->getPositionY());
            completed[i]->setPosition(completed_pos[i]+hscroll, completed[i]->getPositionY());
        }
        for(int i=0;i<stele.size();i++)
            stele[i].first->setPosition(stele[i].second+hscroll,stele[i].first->getPositionY());
        
    }
    
    if(t>duration || direction==3){
        direction=3;
        t=0;
    }
    
}

void Menu3::menuBackClick(cocos2d::Ref* pSender){
    
    auto move=MoveBy::create(0.6, Point(0,Director::getInstance()->getVisibleSize().height/2));
    //auto easing=EaseBackInOut ::create(move);
    
    back->runAction(move->clone());
    packs->runAction(move->clone());
    difficulty->runAction(move);
    
    
    move=MoveBy::create(0.6, Point(0,-Director::getInstance()->getVisibleSize().height/2));
    //easing=EaseBackInOut ::create(move);
    pagination->runAction(move);

    
    GameManager::getInstance()->setScene(5);
    GameManager::getInstance()->playSound(1);
}


bool Menu3::onTouchBegan(Touch* touch, Event* event){
    direction=3;
    
    swipeStart=touch->getLocation().x;
    
    
    for(int i=0;i<75;i++)
        if(hover[i]->getBoundingBox().containsPoint(touch->getLocation())){
            completed_state[i]=1;
            hover[i]->setTexture("cel_12.png");
            //hover[i]->setColor(GameManager::getInstance()->color1_hover);
            hover[i]->getTexture()->setAntiAliasTexParameters();
            completed[i]->setColor(Color3B(GameManager::getInstance()->background_color));
        }
        else{
            completed_state[i]=0;
            hover[i]->setTexture("cel_11.png");
            //hover[i]->setColor(GameManager::getInstance()->color1_hover);
            hover[i]->getTexture()->setAntiAliasTexParameters();
            completed[i]->setColor(Color3B(GameManager::getInstance()->color2));
        }
    
    return true;
}

void Menu3::onTouchMoved(Touch* touch, Event* event){
    is_scrolling=1;
    hscroll+=touch->getDelta().x;
    for(int i=0;i<3;i++)
        grid[i]->setPosition(grid_pos[i]+hscroll,grid[i]->getPositionY());
    
    
    for(int i=0;i<75;i++){
        hover[i]->setPosition(completed_pos[i]+hscroll,hover[i]->getPositionY());
        completed[i]->setPosition(completed_pos[i]+hscroll, completed[i]->getPositionY());
    }
    for(int i=0;i<stele.size();i++)
        stele[i].first->setPosition(stele[i].second+hscroll,stele[i].first->getPositionY());
    
}

void Menu3::onTouchEnded(Touch* touch, Event* event){
    
    
    
    snap_from=hscroll;
    
    
    float x=touch->getLocation().x;
    
    int ok;
    if(abs(x-swipeStart) >=swipeTolerance )
        ok=1;
    else
        ok=0;
    
    int direction1=0;
    
    if(x>swipeStart)
        direction1=1;
    else
        direction1=0;
    
    char buffer[100]={""};
    
    
    
    
    if(this->getBoundingBox().intersectsRect(grid[0]->getBoundingBox()) || this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
        float d1,d2;
        d1=grid_pos[0]-grid[0]->getPositionX();
        d2=grid_pos[0]-grid[1]->getPositionX();
        
        
        if(ok){
            if(direction1==1 && this->getBoundingBox().intersectsRect(grid[0]->getBoundingBox())){
                snap_position=d1;
                //pg->current=1;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Easy",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Easy",pack-5,pack-5);
                
                
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[0]->setOpacity(255);
                
                
            }
            if(direction1==0 && this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
                snap_position=d2;
                //pg->current=2;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Medium",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Medium",pack-5,pack-5);
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[1]->setOpacity(255);
                
            }
            if(this->getBoundingBox().intersectsRect(grid[0]->getBoundingBox()) && this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox()))
                goto sfarsit;
        }
        else{
            if(this->getBoundingBox().intersectsRect(grid[0]->getBoundingBox())){
                snap_position=d1;
                //pg->current=1;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Easy",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Easy",pack-5,pack-5);
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[0]->setOpacity(255);
                
            }
            if(this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
                snap_position=d2;
                //pg->current=2;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Medium",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Medium",pack-5,pack-5);
                
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[1]->setOpacity(255);
            }
        }
        
        
    }
    
    
    if(this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox()) || this->getBoundingBox().intersectsRect(grid[2]->getBoundingBox())){
        float d1,d2;
        d1=grid_pos[0]-grid[1]->getPositionX();
        d2=grid_pos[0]-grid[2]->getPositionX();
        
        if(ok){
            if(direction1==1 && this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
                snap_position=d1;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Medium",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Medium",pack-5,pack-5);
                
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[1]->setOpacity(255);
                
            }
            if(direction1==0 && this->getBoundingBox().intersectsRect(grid[2]->getBoundingBox())){
                snap_position=d2;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Hard",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Hard",pack-5,pack-5);
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[2]->setOpacity(255);
                
            }
            if(this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox()) && this->getBoundingBox().intersectsRect(grid[2]->getBoundingBox()))
                goto sfarsit;
        }
        else{
            if(this->getBoundingBox().intersectsRect(grid[1]->getBoundingBox())){
                snap_position=d1;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Medium",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Medium",pack-5,pack-5);
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[1]->setOpacity(255);
                
            }
            if(this->getBoundingBox().intersectsRect(grid[2]->getBoundingBox())){
                snap_position=d2;
                if(pack<10)
                    sprintf(buffer, "%dx%d - Hard",pack+5,pack+5);
                else
                    sprintf(buffer, "%dx%d - Hard",pack-5,pack-5);
                
                
                for(int i=0;i<3;i++){
                    pag[i]->stopAllActions();
                    pag[i]->setOpacity(255/2);
                }
                pag[2]->setOpacity(255);
                
            }
        }
    }
    

    
    if(hscroll>scroll_limits.x)
        snap_position=grid_pos[0]-grid[0]->getPositionX();
    if(hscroll<-scroll_limits.y)
        snap_position=grid_pos[0]-grid[2]->getPositionX();

    

sfarsit:
    
    
    
    direction=direction1;
    if(strlen(buffer)){
        difficulty->setString(buffer);
        //difficulty->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    }
    for(int i=0;i<75;i++){
        if(hover[i]->getBoundingBox().containsPoint(touch->getLocation()) && completed_state[i] && abs(x-swipeStart)<20 ){
            if(!current_pack[i]){
                completed_state[i]=0;
                completed[i]->setColor(Color3B(GameManager::getInstance()->color2));
                hover[i]->setTexture("cel_11.png");
                hover[i]->setColor(GameManager::getInstance()->color2);
                hover[i]->getTexture()->setAntiAliasTexParameters();
                GameManager::getInstance()->showLevelLockedAlert();
                break;
            }
            
            
            
            
            auto move=MoveBy::create(0.6, Point(0,Director::getInstance()->getVisibleSize().height/2));
            //auto easing=EaseBackInOut ::create(move);
            
            back->runAction(move->clone());
            packs->runAction(move->clone());
            difficulty->runAction(move);
            
            
            move=MoveBy::create(0.6, Point(0,-Director::getInstance()->getVisibleSize().height/2));
            //easing=EaseBackInOut ::create(move);
            pagination->runAction(move);
            
            
            completed_state[i]=0;
            completed[i]->setColor(Color3B(GameManager::getInstance()->color2));
            hover[i]->setTexture("cel_11.png");
            hover[i]->setColor(GameManager::getInstance()->color2);
            hover[i]->getTexture()->setAntiAliasTexParameters();
            GameManager::getInstance()->level_nr=i+1;
            GameManager::getInstance()->setScene(7);
            GameManager::getInstance()->playSound(1);
        }
        else{
            completed_state[i]=0;
            hover[i]->setTexture("cel_11.png");
            hover[i]->setColor(GameManager::getInstance()->color2);
            hover[i]->getTexture()->setAntiAliasTexParameters();
            completed[i]->setColor(Color3B(GameManager::getInstance()->color2));
        }
    }
    is_scrolling=0;
    
}

void Menu3::delayedLoad(float dt){
    GameManager::getInstance()->setScene(7);
}
