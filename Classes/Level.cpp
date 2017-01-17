//
//  Level.cpp
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#include "Level.h"
#include "GameManager.h"


//#include "ZipUtils.h"


SpriteBlur::~SpriteBlur()
{
}

SpriteBlur* SpriteBlur::create()
{
    SpriteBlur* pRet = new SpriteBlur();
    if (pRet)
    {
        pRet->autorelease();
        
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool SpriteBlur::initWithTexture(Texture2D* texture, const Rect& rect)
{
    if( Sprite::initWithTexture(texture, rect) )
    {
        
#if CC_ENABLE_CACHE_TEXTURE_DATA
        auto listener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event){
            setShaderProgram(NULL);
            initProgram();
        });
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
        auto s=texture->getContentSizeInPixels();
        
        if(!shader){
            texelHeightOffset=1.0/s.height;
            texelWidthOffset=1.0/s.width;
            h_pass=0;
            v_pass=1;
            initProgram();
        }
        return true;
    }
    
    return false;
}

void SpriteBlur::initProgram()
{

                                 
    
    shader=new GLProgram();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	 shader->initWithPrecompiledByteArray(blur_shader,sizeof(blur_shader));
	//shader->initWithPrecompiledProgramByteArray((GLchar*)blur_shader,(GLchar*)blur_shader);

#else
	GLchar *fragSource=(GLchar*) String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename("blur_f.glsl").c_str())->getCString();
    GLchar *vertSource=(GLchar*) String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename("blur_v.glsl").c_str())->getCString();
    shader->initWithByteArrays(vertSource, fragSource);
#endif

    shader->retain();
    
    CHECK_GL_ERROR_DEBUG();
    
    shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    
    CHECK_GL_ERROR_DEBUG();
    
    shader->link();
    
    CHECK_GL_ERROR_DEBUG();
    
    shader->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
    
    u_texelWidthOffset = shader->getUniformLocation("texelWidthOffset");
    u_texelHeightOffset = shader->getUniformLocation("texelHeightOffset");
    
    
    CHECK_GL_ERROR_DEBUG();
    
    auto shaderState = GLProgramState::getOrCreateWithGLProgram(shader);
    setGLProgramState(shaderState);
    
}



void SpriteBlur::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{   
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SpriteBlur::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void SpriteBlur::onDraw(const Mat4 &transform, uint32_t flags)
{
    
    shader->use();
    shader->setUniformsForBuiltins(transform);
    
    
    BlendFunc blend = getBlendFunc();
    GL::blendFunc(blend.src, blend.dst);
    
    shader->setUniformLocationWith1f(u_texelHeightOffset, texelHeightOffset*v_pass);
    shader->setUniformLocationWith1f(u_texelWidthOffset, texelWidthOffset*h_pass);
    
    GL::bindTexture2D( getTexture()->getName());
    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION|cocos2d::GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    
    
    
#define kQuadSize sizeof(_quad.bl)
    size_t offset = (size_t)&_quad;
    
    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,4);

    
    h_pass=!h_pass;
    v_pass=!v_pass;
    
}




SpriteBatchNode* Level::line_textures;
Color3B Level::culori[14];

cell::cell(){
    bg=Sprite::createWithSpriteFrameName("cel_1.png");
    //bg->setPosition(Point::ZERO);
    bg->setVisible(false);
    //bg->setFlippedY(false);
    Level::line_textures->addChild(bg);
    state=0;
    color=0;
    deleted=0;
    nr=0;
    nr_tex=0;
    last_color=0;
    last_nr=0;
    last_nr_tex=-1;
    isGlobe=0;
}

void cell::updateTexture(){
    if(nr_tex==-1 || !state || !color)
        bg->setVisible(false);
    else{
        bg->setVisible(true);
        bg->setColor(Level::culori[color-1]);
    }
    switch (nr_tex) {
        case 0:
            bg->setSpriteFrame("cel_1.png");
            break;
        case 1:
            bg->setSpriteFrame("cel_2.png");
            break;
        case 2:
            bg->setSpriteFrame("cel_3.png");
            break;
        case 3:
            bg->setSpriteFrame("cel_4.png");
            break;
        case 4:
            bg->setSpriteFrame("cel_5.png");
            break;
        case 5:
            bg->setSpriteFrame("cel_6.png");
            break;
        case 6:
            bg->setSpriteFrame("cel_7.png");
            break;
        case 7:
            bg->setSpriteFrame("cel_8.png");
            break;
        case 8:
            bg->setSpriteFrame("cel_9.png");
            break;
        case 9:
            bg->setSpriteFrame("cel_10.png");
            break;
    }
}

int cell::isActive(){
    return nr_tex!=-1 && state &&color;
}

path::path(){
    length=0;
}

void path::add(int i, int j){
    paths[length].x=j;
    paths[length++].y=i;
}

Scene* Level::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    
    auto layer = Level::create();
    scene->addChild(layer);
    // return the scene
    return scene;
}


bool Level::init()
{
    //////////////////////////////
    // 1. super init first
    
    if ( !LayerColor::initWithColor(GameManager::getInstance()->background_color))
    {
        return false;
    }
    
    e=new EventCustom("hint");
    t=new Touch();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    width=visibleSize.width;
    height=visibleSize.height;
    
    grid=Sprite::create("chenar.png");
    grid->setPosition(width*0.5, height*0.55);
    grid->setVisible(true);
    this->addChild(grid);
    

    _pack=GameManager::getInstance()->pack;
    GameManager::getInstance()->readSave(_pack, current_pack);
    
    
    ajutoare=GameManager::getInstance()->getHints();
    
    auto backSprite=Sprite::create("back.png");
    backSprite->setFlippedY(false);
    back=MenuItemSprite::create(backSprite,backSprite,CC_CALLBACK_1(Level::menuBackClick, this));
    back->setAnchorPoint(Point(0, 0.5));
    auto menuBack=Menu::create(back,NULL);
    menuBack->setPosition(Point(roundf(grid->getPositionX()-grid->getContentSize().width/2-grid->getContentSize().width*0.02), visibleSize.height*0.95));
    this->addChild(menuBack);
    
    
    char buffer[100];
    sprintf(buffer, "Level %d",_level);
    
    level_nr = LabelTTF::create(buffer,GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*48);
    level_nr->setHorizontalAlignment(TextHAlignment::LEFT);
    level_nr->setPosition(Point(roundf(menuBack->getPositionX()+backSprite->getContentSize().width*0.35), roundf(visibleSize.height*0.955)));
    //level_nr->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    level_nr->setAnchorPoint(Point(0, 0.55));
    //label->getTexture()->setAliasTexParameters();
    // position the label on the center of the screen
    
    // add the label as a child to this layer
    this->addChild(level_nr);

    //important
    /*if(visibleSize.width>=768)
        grid=Sprite::create("grid_5_1.png");
    else*/
       
    
    
    
    line_textures=SpriteBatchNode::create("test.png");
    this->addChild(line_textures);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("test.plist");
    
    //SpriteFrameCache::getInstance()->purgeSharedSpriteFrameCache()

    
    space_label=LabelTTF::create("0", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    space_label->retain();
    
    moves_label[1]=LabelTTF::create(" 0", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    moves_label[1]->setColor(GameManager::getInstance()->color2);
    moves_label[1]->setHorizontalAlignment(TextHAlignment::CENTER);
    //moves_label[1]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    moves_label[1]->setPosition(grid->getPositionX(), grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    this->addChild(moves_label[1]);
    
    moves_label[0]=LabelTTF::create("Moves: ", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    moves_label[0]->setColor(GameManager::getInstance()->color2);
    moves_label[0]->setHorizontalAlignment(TextHAlignment::RIGHT);
   // moves_label[0]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    moves_label[0]->setAnchorPoint(Point(1, 0.5));
    moves_label[0]->setPosition(moves_label[1]->getPositionX()-moves_label[1]->getBoundingBox().size.width/2-space_label->getBoundingBox().size.width*0.3, grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    this->addChild(moves_label[0]);
    
    
    
    moves_label[2]=LabelTTF::create("/5", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    moves_label[2]->setColor(GameManager::getInstance()->color2);
    moves_label[2]->setHorizontalAlignment(TextHAlignment::LEFT);
   // moves_label[2]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    moves_label[2]->setAnchorPoint(Point(0, 0.5));
    moves_label[2]->setPosition(moves_label[1]->getPositionX()+moves_label[1]->getBoundingBox().size.width/2, grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    this->addChild(moves_label[2]);

    
    completed_label=LabelTTF::create("Completed: 0%", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    completed_label->setColor(GameManager::getInstance()->color2);
    completed_label->setHorizontalAlignment(TextHAlignment::RIGHT);
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    completed_label->setAnchorPoint(Point(1, 0.5));
    completed_label->setPosition(grid->getPositionX()+grid->getBoundingBox().size.width/2,grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    this->addChild(completed_label);
    
    
    
    time_label=LabelTTF::create("Time: 0s", GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*30);
    time_label->setColor(GameManager::getInstance()->color2);
    time_label->setHorizontalAlignment(TextHAlignment::LEFT);
    //time_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    time_label->setAnchorPoint(Point(0, 0.5));
    time_label->setPosition(grid->getPositionX()-grid->getBoundingBox().size.width/2,grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    this->addChild(time_label);
    this->schedule(schedule_selector(Level::updateTime), 1.0f);

    
    //this is where you set the colors for each dot
    //culori[0] -> 1_30.png & culori[13] -> 14_30.png
    
    //A - 1 Wizard
    culori[0]=Color3B(182, 210, 255);
    //B - 2 Wizard Staff
    culori[1]=Color3B(188, 226, 158);
    //C - 3 Potion Bottle
    culori[2]=Color3B(250, 221, 230);
    //D - 4 SpellBook
    culori[3]=Color3B(224, 213, 255);
    //E - 5 Magic Ring
    culori[4]=Color3B(255, 224, 183);
    //F - 6 Castle
    culori[5]=Color3B(98, 222, 253);
    //G - 7 Pocket Watch
    culori[6]=Color3B(255, 246, 126);
    //H - 8 Poison apple
    culori[7]=Color3B(154, 109, 255);
    //I - 9Wizard hat
    culori[8]=Color3B(243, 136, 174);
    //J - 10
    culori[9]=Color3B(79, 122, 148);
    //K - 11
    culori[10]=Color3B(220, 105, 91);
    //L - 12
    culori[11]=Color3B(113, 191, 196);
    //M - 13
    culori[12]=Color3B(162, 199, 94);
    //N - 14
    culori[13]=Color3B(79, 122, 148);
    
    
    
    last_unit=-1;
	last_unit_undo=-1;
	state_l=0;
    
    
    //grid->setScale(1.2);
    
    float scale=1.01;
    int w= grid->getBoundingBox().size.width;
    float cell_size5=w/5;
    float cell_size=w/(_pack%10+5);
    scale=cell_size/cell_size5+0.01;
    
    auto test=Sprite::createWithSpriteFrameName("cel_1.png");
    
    if(cell_size5>test->getBoundingBox().size.width)
        scale+=(cell_size5/test->getBoundingBox().size.width)-1;
    
    
    //char buffer[100];
    
    for(int i=0;i<14;i++){
        
        
        
        sprintf(buffer, "%d_%d.png",i+1,30);
        //sprintf(buffer,"1_30.png");
        
        g1[i]=Sprite::create(buffer);
        g1[i]->setVisible(false);
        g1[i]->setFlippedY(false);
        //g1[i]->setColor(culori[i]);
        g1[i]->setScale(scale-0.06);
        g1[i]->getTexture()->generateMipmap();
        g1[i]->getTexture()->setAntiAliasTexParameters();
        this->addChild(g1[i]);
        
        g2[i]=Sprite::create(buffer);
        g2[i]->setVisible(false);
        g2[i]->setFlippedY(false);
        //g2[i]->setColor(culori[i]);
        g2[i]->setScale(scale-0.06);
        g2[i]->getTexture()->generateMipmap();
        g2[i]->getTexture()->setAntiAliasTexParameters();
        this->addChild(g2[i]);
        
        
        sprintf(buffer, "%d_%d.png",i+1,28);
        
        labels1[i]=Sprite::create(buffer);
        labels1[i]->setVisible(false);
        labels1[i]->setFlippedY(false);
        labels1[i]->setScale(scale-0.06);
        labels1[i]->setColor(Color3B(GameManager::getInstance()->background_color));
        this->addChild(labels1[i]);
        
        labels2[i]=Sprite::create(buffer);
        labels2[i]->setVisible(false);
        labels2[i]->setFlippedY(false);
        labels2[i]->setScale(scale-0.06);
        labels2[i]->setColor(Color3B(GameManager::getInstance()->background_color));
        this->addChild(labels2[i]);
        
    }
    
    
    
    for(int i=0;i<14;i++)
        for(int j=0;j<14;j++){
            cels[i][j]=new cell();
            cels[i][j]->bg->setScale(scale);
        }
    
    globes_scale=g1[0]->getScale();
    
    rtt=RenderTexture::create(grid->getBoundingBox().size.width ,grid->getBoundingBox().size.height);
    this->addChild(rtt);
    rtt->setVisible(false);
    rtt->setKeepMatrix(true);
    rtt->setPosition(grid->getPosition());
    
    Size size=Director::getInstance()->getWinSize();
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    
    rtt->setVirtualViewport(Point(grid->getPositionX()-grid->getBoundingBox().size.width/2 ,grid->getPositionY()-grid->getBoundingBox().size.height/2),Rect(0,0,size.width,size.height),Rect(0,0,pixelSize.width,pixelSize.height));
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=CC_CALLBACK_2(Level::onTouchBegan, this);
    touchListener->onTouchMoved=CC_CALLBACK_2(Level::onTouchMoved, this);
    touchListener->onTouchEnded=CC_CALLBACK_2(Level::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
 
    circle=Sprite::create("circle.png");
    this->addChild(circle);
    circle->setVisible(false);
    
    
    blur1=RenderTexture::create(width, height);
    blur1->retain();
    
    blur2=RenderTexture::create(width, height);
    blur2->retain();

    blur_sprite=new SpriteBlur();
    blur_sprite->setPosition(width/2,height/2);
    
    
    
    auto undoSprite=Sprite::create("undo.png");
    //undoSprite->getTexture()->setAliasTexParameters();
    undoSprite->setColor(GameManager::getInstance()->color2);
    
    auto undoSprite2=Sprite::create("undo2.png");
    //undoSprite2->getTexture()->setAliasTexParameters();
    undoSprite2->setColor(GameManager::getInstance()->color2);
    
    
    //space between buttons
    float space = grid->getContentSize().width-undoSprite->getContentSize().width*5;
    space/=4;
    
    undo=MenuItemSprite::create(undoSprite,undoSprite2,CC_CALLBACK_1(Level::menuUndoClick, this));
    auto menuUndo=Menu::create(undo,NULL);
    menuUndo->setPosition(roundf(grid->getPositionX()-grid->getContentSize().width/2+undoSprite->getContentSize().width/2), roundf(grid->getPositionY()-grid->getBoundingBox().size.height/2-undoSprite->getBoundingBox().size.width*0.75));
    this->addChild(menuUndo);

    
    
    auto previousSprite=Sprite::create("previous.png");
    //previousSprite->getTexture()->setAliasTexParameters();
    previousSprite->setColor(GameManager::getInstance()->color2);
    
    auto previousSprite2=Sprite::create("previous2.png");
    //previousSprite2->getTexture()->setAliasTexParameters();
    previousSprite2->setColor(GameManager::getInstance()->color2);
    
    previous=MenuItemSprite::create(previousSprite,previousSprite2,CC_CALLBACK_1(Level::menuPreviousClick, this));
    auto menuPrevious=Menu::create(previous,NULL);
    menuPrevious->setPosition(roundf(menuUndo->getPositionX()+space+undoSprite->getContentSize().width), roundf(grid->getPositionY()-grid->getBoundingBox().size.height/2-undoSprite->getBoundingBox().size.width*0.75));
    this->addChild(menuPrevious);
    
    log("%f",space);
    

    
    auto resetSprite=Sprite::create("replay.png");
    //resetSprite->getTexture()->setAliasTexParameters();
    resetSprite->setColor(GameManager::getInstance()->color2);
    
    auto resetSprite2=Sprite::create("replay2.png");
    //resetSprite2->getTexture()->setAliasTexParameters();
    resetSprite2->setColor(GameManager::getInstance()->color2);
    
    reset=MenuItemSprite::create(resetSprite,resetSprite2,CC_CALLBACK_1(Level::menuResetClick, this));
    auto menuReset=Menu::create(reset,NULL);
    menuReset->setPosition(roundf(menuPrevious->getPositionX()+space+undoSprite->getContentSize().width), roundf(grid->getPositionY()-grid->getBoundingBox().size.height/2-undoSprite->getBoundingBox().size.width*0.75));
    this->addChild(menuReset);
    
    
    auto nextSprite=Sprite::create("next.png");
    //nextSprite->getTexture()->setAliasTexParameters();
    nextSprite->setColor(GameManager::getInstance()->color2);
    
    auto nextSprite2=Sprite::create("next2.png");
    //nextSprite2->getTexture()->setAliasTexParameters();
    nextSprite2->setColor(GameManager::getInstance()->color2);
    
    next=MenuItemSprite::create(nextSprite,nextSprite2,CC_CALLBACK_1(Level::menuNextClick, this));
    auto menuNext=Menu::create(next,NULL);
    menuNext->setPosition(roundf(menuReset->getPositionX()+space+undoSprite->getContentSize().width), roundf(grid->getPositionY()-grid->getBoundingBox().size.height/2-undoSprite->getBoundingBox().size.width*0.75));
    this->addChild(menuNext);

    
    auto hintsSprite=Sprite::create("hints.png");
    //hintsSprite->getTexture()->setAliasTexParameters();
    hintsSprite->setColor(GameManager::getInstance()->color2);
    
    auto hintsSprite2=Sprite::create("hints2.png");
    //hintsSprite2->getTexture()->setAliasTexParameters();
    hintsSprite2->setColor(GameManager::getInstance()->color2);
    
    
    hints=MenuItemSprite::create(hintsSprite,hintsSprite2,CC_CALLBACK_1(Level::menuHintsClick, this));
    auto menuHints=Menu::create(hints,NULL);
    menuHints->setPosition(roundf(menuNext->getPositionX()+space+undoSprite->getContentSize().width), roundf(grid->getPositionY()-grid->getBoundingBox().size.height/2-undoSprite->getBoundingBox().size.width*0.75));
    this->addChild(menuHints);
    
    
    
    hints_label=LabelTTF::create(itoa(ajutoare), GameManager::getInstance()->font_name, GameManager::getInstance()->font_scale*20);
    //hints_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    hints_label->setHorizontalAlignment(TextHAlignment::CENTER);
    hints_label->setPosition(menuHints->getPositionX()+hintsSprite->getBoundingBox().size.width*0.08,menuHints->getPositionY()+hintsSprite->getBoundingBox().size.height*0.12);
    
    hints_label->setColor(Color3B(GameManager::getInstance()->background_color));
    this->addChild(hints_label);


    
    
    
    blur_mesaj=Sprite::create();
    blur_mesaj->setPosition(width/2, height/2);
    blur_mesaj->setFlippedY(true);
    blur_mesaj->setVisible(false);
    this->addChild(blur_mesaj,999);
    
    
    
    overlay=Sprite::create("finish_overlay.png");
    //overlay->setVisible(false);
    overlay->setPosition(width/2, height/2);
    overlay->setOpacity(0);
    overlay->setTextureRect(blur1->getSprite()->getTextureRect());
    this->addChild(overlay,1000);
    
    
    
    perfect=Sprite::create("perfect.png");
    perfect->setAnchorPoint(Point(0, 0.5));
    perfect->setPosition(level_nr->getPositionX(),height*0.7);
    perfect->setVisible(false);
    perfect->setFlippedY(false);
    perfect_pos=perfect->getPosition();
    this->addChild(perfect,1001);
    
    
    not_perfect=Sprite::create("not_perfect.png");
    not_perfect->setAnchorPoint(Point(0, 0.5));
    not_perfect->setPosition(level_nr->getPositionX(),height*0.7);
    not_perfect->setVisible(false);
    not_perfect->setFlippedY(false);
    not_perfect_pos=not_perfect->getPosition();
    this->addChild(not_perfect,1001);
 
    
    
    
    finish_stars[0]=Sprite::create("star1.png");
    finish_stars[0]->setVisible(false);
    finish_stars[0]->setPosition(level_nr->getPositionX()+finish_stars[0]->getBoundingBox().size.width/2, height*0.58);
    finish_star_pos[0]=finish_stars[0]->getPosition();
    this->addChild(finish_stars[0],1001);
    
    finish_stars2[0]=Sprite::create("star2.png");
    finish_stars2[0]->setVisible(false);
    finish_stars2[0]->setPosition(level_nr->getPositionX()+finish_stars2[0]->getBoundingBox().size.width/2, height*0.58);
    finish_stars2[0]->setScale(0);
    this->addChild(finish_stars2[0],1002);
    
    for(int i=1;i<3;i++){
        finish_stars[i]=Sprite::create("star1.png");
        finish_stars[i]->setVisible(false);
        finish_stars[i]->setPosition(finish_stars[i-1]->getPositionX()+finish_stars[i]->getBoundingBox().size.width*1.2, height*0.58);
        finish_star_pos[i]=finish_stars[i]->getPosition();
        this->addChild(finish_stars[i],1001);
        
        
        finish_stars2[i]=Sprite::create("star2.png");
        finish_stars2[i]->setVisible(false);
        finish_stars2[i]->setPosition(finish_stars[i]->getPosition());
        finish_stars2[i]->setScale(0);
        this->addChild(finish_stars2[i],1002);

    }
    
    for(int i=0;i<3;i++){
        particle_stars[i]=ParticleSystemQuad::create("stars.plist");
        particle_stars[i]->setPosition(finish_star_pos[i]);
        particle_stars[i]->stopSystem();
        particle_stars[i]->setScale(GameManager::getInstance()->font_scale);
        this->addChild(particle_stars[i],1001);
    }
    
    auto next_level_sprite=Sprite::create("next_level.png");
    next_level_sprite->setFlippedY(false);
    
    auto next_level_sprite2=Sprite::create("next_level.png");
    next_level_sprite2->setFlippedY(false);
    next_level_sprite2->setColor(GameManager::getInstance()->color2);
    
    next_level=MenuItemSprite::create(next_level_sprite, next_level_sprite2,CC_CALLBACK_1(Level::menuNextLevelClick, this));
    next_level->setAnchorPoint(Point(-1.2, 1.55));
    auto menuNextLevel=Menu::create(next_level,NULL);
    menuNextLevel->setPosition(level_nr->getPositionX(),height*0.47);
    next_level->setVisible(false);
    next_level_pos=next_level->getPosition();
    this->addChild(menuNextLevel,1002);
    
    
    auto try_again_sprite=Sprite::create("retry.png");
    try_again_sprite->setFlippedY(false);
    
    
    auto try_again_sprite2=Sprite::create("retry.png");
    try_again_sprite2->setFlippedY(false);
    try_again_sprite2->setColor(GameManager::getInstance()->color2);
    
    try_again=MenuItemSprite::create(try_again_sprite, try_again_sprite2,CC_CALLBACK_1(Level::menuTryAgainClick, this));
    try_again->setAnchorPoint(Point(0, 0.5));
    auto menuTryAgain=Menu::create(try_again,NULL);
    menuTryAgain->setPosition(level_nr->getPositionX(),height*0.4);
    try_again->setVisible(false);
    try_again_pos=try_again->getPosition();
    this->addChild(menuTryAgain,1002);
    
    
    current=GameManager::getInstance()->level_nr;
    this->scheduleOnce(schedule_selector(Level::delayedLoad), 0);
    
    

    
    
    
    
    back->setPositionY(back->getPositionY()+grid->getBoundingBox().size.height);
    level_nr->setPositionY(level_nr->getPositionY()+grid->getBoundingBox().size.height);
    moves_label[0]->setPositionY(moves_label[0]->getPositionY()+grid->getBoundingBox().size.height);
    moves_label[1]->setPositionY(moves_label[1]->getPositionY()+grid->getBoundingBox().size.height);
    moves_label[2]->setPositionY(moves_label[2]->getPositionY()+grid->getBoundingBox().size.height);
    time_label->setPositionY(time_label->getPositionY()+grid->getBoundingBox().size.height);
    completed_label->setPositionY(completed_label->getPositionY()+grid->getBoundingBox().size.height);
    
    
    undo->setPositionY(undo->getPositionY()-grid->getBoundingBox().size.height);
    previous->setPositionY(previous->getPositionY()-grid->getBoundingBox().size.height);
    reset->setPositionY(reset->getPositionY()-grid->getBoundingBox().size.height);
    next->setPositionY(next->getPositionY()-grid->getBoundingBox().size.height);
    hints->setPositionY(hints->getPositionY()-grid->getBoundingBox().size.height);
    
    auto move=MoveBy::create(0.6, Point(0,-grid->getBoundingBox().size.height));
    auto move2=MoveBy::create(0.6, Point(0,grid->getBoundingBox().size.height));
    
    //auto easing=EaseBackInOut ::create(move);
    //auto easing2=EaseBackInOut ::create(move2);
    
    back->runAction(move->clone());
    level_nr->runAction(move->clone());
    moves_label[0]->runAction(move->clone());
    moves_label[1]->runAction(move->clone());
    moves_label[2]->runAction(move->clone());
    time_label->runAction(move->clone());
    completed_label->runAction(move);
    
    
    undo->runAction(move2->clone());
    previous->runAction(move2->clone());
    next->runAction(move2->clone());
    reset->runAction(move2->clone());
    hints->runAction(move2->clone());

    
    
    return true;
}


void Level::Load(int level){
    
    if(!level)
        level=1;
    
    
    //line_textures->getTexture()->setAntiAliasTexParameters();
    
    current=level;
    current_hint=0;
    last_line=0;
    last_unit=-1;
    n_g=0;
     shifts=0;
    
    char buffer[100];
    sprintf(buffer,"level %d",level);
    level_nr->setString(buffer);
    //level_nr->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    
    
    char fname[ 255 ],zipname[255];
    sprintf(fname,"%d_%d.lvl",_pack+1,level);
    sprintf(zipname, "Pack%d.zip",_pack+1);
    
    
    int file_offset=0,file_read=0;
    
    
    std::string zipfile=FileUtils::getInstance()->fullPathForFilename(zipname);
    
    long size=0;
    
    
    

    unsigned char *continut2=FileUtils::getInstance()->getFileDataFromZip(zipfile, fname,&size );
    std::string continut(continut2,continut2+size);
    
    std::istringstream s(continut);
    
    s>>n;
    
    
    
    float x0=grid->getPositionX()-grid->getBoundingBox().size.width/2.0,y0=grid->getPositionY()+grid->getBoundingBox().size.height/2.0;
    float offset=grid->getBoundingBox().size.width/n;
    x0+=offset/2;
    y0-=offset/2;

    
    
    int globes1[14]={0};
    
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            cels[i][j]->bg->setPosition(x0+j*offset, y0-i*offset);
            cels[i][j]->bg->setVisible(false);
            cels[i][j]->state=0;
            cels[i][j]->color=0;
            cels[i][j]->nr_tex=-1;
            cels[i][j]->isGlobe=0;
            cels[i][j]->updateTexture();
        }

    for(int i=0;i<14;i++){
        unit[i]=0;
        paths[i].length=0;
        g1[i]->setVisible(false);
        g2[i]->setVisible(false);
        labels1[i]->setVisible(false);
        labels2[i]->setVisible(false);
    }
    
    
    int i1=0,i2=0,j1=0,j2=0,max1=0,max2=0,gl=0;
    
    char buff[20];
    
    
    s>>max1>>buff>>i1>>j1;
    //sscanf(buffer2+file_offset,"%d%s%d%d%n",&max1,buff,&i1,&j1,&file_read);
    //file_offset+=file_read;
    gl=max1-1;
   
    
    g2[gl]->setPosition(x0+j1*offset, y0-i1*offset);
    g2[gl]->setVisible(true);
    globes_ind2[gl].x=j1;
    globes_ind2[gl].y=i1;
    
    cels[i1][j1]->isGlobe=max1;
    
    paths[gl].add(i1, j1);
    
    while(!s.eof()){
        //if(!sscanf(buffer2+file_offset,"%d%s%d%d%n",&max2,buff,&i2,&j2,&file_read))
            //break;

        s>>max2>>buff>>i2>>j2;
        
        file_offset+=file_read;
        paths[max2-1].add(i2, j2);
        if(max2>max1){
            gl=max1-1;
            
            if(globes1[gl]){
                g2[gl]->setPosition(x0+j1*offset, y0-i1*offset);
                g2[gl]->setVisible(true);
                globes_ind2[gl].x=j1;
                globes_ind2[gl].y=i1;
                cels[i1][j1]->isGlobe=max1;
            }
            else{
                g1[gl]->setPosition(x0+j1*offset, y0-i1*offset);
                g1[gl]->setVisible(true);
                globes1[gl]=1;
                globes_ind1[gl].x=j1;
                globes_ind1[gl].y=i1;
                cels[i1][j1]->isGlobe=max1;
            }
            gl=max2-1;
            
            if(globes1[gl]){
                g2[gl]->setPosition(x0+j2*offset, y0-i2*offset);
                g2[gl]->setVisible(true);
                globes_ind2[gl].x=j2;
                globes_ind2[gl].y=i2;
                cels[i2][j2]->isGlobe=max2;
            }
            else{
                g1[gl]->setPosition(x0+j2*offset, y0-i2*offset);
                g1[gl]->setVisible(true);
                globes1[gl]=1;
                globes_ind1[gl].x=j2;
                globes_ind1[gl].y=i2;
                cels[i2][j2]->isGlobe=max2;
            }
            max1++;
        }
        i1=i2;
        j1=j2;
    }
    
    gl=max2-1;
    
    if(globes1[gl]){
        g2[gl]->setPosition(x0+j2*offset, y0-i2*offset);
        g2[gl]->setVisible(true);
        globes_ind2[gl].x=j2;
        globes_ind2[gl].y=i2;
        cels[i2][j2]->isGlobe=max2;
    }
    else{
        g1[gl]->setPosition(x0+j2*offset, y0-i2*offset);
        g1[gl]->setVisible(true);
        globes1[gl]=1;
        globes_ind1[gl].x=j2;
        globes_ind1[gl].y=i2;
        cels[i2][j2]->isGlobe=max2;
    }
    
    
    if(UserDefault::getInstance()->getIntegerForKey("labels"))
        for(int i=0;i<max2;i++){
            labels1[i]->setPosition(g1[i]->getPosition());
            labels1[i]->setVisible(true);
            labels2[i]->setPosition(g2[i]->getPosition());
            labels2[i]->setVisible(true);
        }
    
    
    n_g=max2;
    nr_moves=0;
    
    
    
    sprintf(buffer, "0");
    
    moves_label[1]->setString(buffer);
    moves_label[1]->setColor(Color3B(66, 213, 94 ));
    //moves_label[1]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    sprintf(buffer, "/%d",n_g);
    moves_label[2]->setString(buffer);
   // moves_label[2]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    sprintf(buffer, "Completed: 0%%");
    completed_label->setString(buffer);
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    seconds=0;
    sprintf(buffer, "Time: 0s");
    time_label->setString(buffer);
    //time_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    
    if(!current_pack[current])
        next->setOpacity(255/2);
    else
        next->setOpacity(255);;
    
    if(current==1)
        previous->setOpacity(255/2);
    else
        previous->setOpacity(255);
    
    undo->setOpacity(255/2);
    if(!ajutoare)
        hints->setOpacity(255/2);
}


void Level::menuBackClick(cocos2d::Ref* pSender){
    if(state_l)
        return;
    GameManager::getInstance()->setScene(6);
    GameManager::getInstance()->setPack(_pack);
    GameManager::getInstance()->playSound(1);
}


void Level::updateMoves(){
    
    //sprintf((char*)moves, "%d",nr_moves);
    moves_label[1]->setString(itoa(nr_moves));
    if(nr_moves>n_g)
        moves_label[1]->setColor(Color3B(213, 66, 69 ));
    else
        moves_label[1]->setColor(Color3B(66, 213, 94 ));
    if(!moves_label[0]->getNumberOfRunningActions()){
    moves_label[2]->setPosition(moves_label[1]->getPositionX()+moves_label[1]->getBoundingBox().size.width/2, grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
     moves_label[0]->setPosition(moves_label[1]->getPositionX()-moves_label[1]->getBoundingBox().size.width/2-space_label->getBoundingBox().size.width*0.3, grid->getPositionY()+grid->getBoundingBox().size.height*1.05/2);
    }
    //moves_label[0]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    //moves_label[2]->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
}

int Level::getPosition(int i, int j, int k, int l){// 1dr 2st 3sus 4jos
    
    if(i==k){//same line
        if(l>j)
            return 1;
        else
            return 2;
    }
    if(j==l){//same col
        if(k<i)
            return 3;
        else
            return 4;
    }
    return 0;
}


bool Level::onTouchBegan(Touch* touch, Event* event){
    
	

    if(state_l)
        return true;
    
    
    if(event && event->getType()!=Event::Type::CUSTOM){
        if(hint_vector.size())
            return true;
    }
    
    
    //if(!hint)
    //y=height-y;
    
    int ok=0;
    
    
    if((grid->getBoundingBox().containsPoint(touch->getLocation()))){
        if(event && event->getType()!=Event::Type::CUSTOM){
            circle->setVisible(true);
            circle->setPosition(touch->getLocation());
        }
        //is_moving=1;
    }
    
    
    
    //sterge linie & porneste alta
    for(int i=0;i<n_g;i++)
        if(g1[i]->getBoundingBox().containsPoint(touch->getLocation())){
            
            
            //if(!cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x])
            
            for(int k=0;k<n;k++)
                for(int l=0;l<n;l++)
                    if(cels[k][l]->bg->getBoundingBox().containsPoint(touch->getLocation()) && !cels[k][l]->isGlobe)
                        goto negasit;
            
            
            last_line_undo=last_line;
            last_unit_undo=last_unit;
            for(int i=0;i<14;i++)
                unit_undo[i]=unit[i];
            
            nr_moves_undo=nr_moves;
            
            for(int i=0;i<n;i++)
                for(int j=0;j<n;j++){
                    undo_cels[i][j].state=cels[i][j]->state;
                    undo_cels[i][j].color=cels[i][j]->color;
                    undo_cels[i][j].nr=cels[i][j]->nr;
                    undo_cels[i][j].nr_tex=cels[i][j]->nr_tex;
                    undo_cels[i][j].deleted=cels[i][j]->deleted;
                    undo_cels[i][j].last_nr=cels[i][j]->last_nr;
                    undo_cels[i][j].last_nr_tex=cels[i][j]->last_nr_tex;
                    undo_cels[i][j].last_color=cels[i][j]->last_color;
                    undo_cels[i][j].isGlobe=cels[i][j]->isGlobe;
                }
            undo->setOpacity(255);
            
            auto scale=ScaleTo::create(0.1, globes_scale*1.09);
            g1[i]->runAction(scale);
            
            to_animate[0].first=1;
            to_animate[0].second=i;
            
            unit[i]=0;
            if(i+1==last_unit)
                last_unit=-1;
            else
                if(i+1!=last_line || !last_line){
                    nr_moves++;
                    updateMoves();
                }
            
            cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x]->state=1;
            last_line=i+1;
            cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x]->color=last_line;
            ok=1;
            ltj=globes_ind1[i].x;
            lti=globes_ind1[i].y;
            cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x]->nr=++shifts;
            cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x]->nr_tex=-1;
            cels[(int)globes_ind1[i].y][(int)globes_ind1[i].x]->updateTexture();
            last_dir=0;
            for(int k=0;k<n;k++)
                for(int j=0;j<n;j++)
                    if(cels[k][j]->color==last_line && (k!=(int)globes_ind1[i].y || j!=(int)globes_ind1[i].x)){
                        cels[k][j]->color=0;
                        cels[k][j]->state=0;
                        cels[k][j]->nr_tex=0;
                        cels[k][j]->updateTexture();
                    }
            break;
            
        }
        else
            if(g2[i]->getBoundingBox().containsPoint(touch->getLocation())){
                
                for(int k=0;k<n;k++)
                    for(int l=0;l<n;l++)
                        if(cels[k][l]->bg->getBoundingBox().containsPoint(touch->getLocation()) && !cels[k][l]->isGlobe)
                            goto negasit;
                
                
                last_line_undo=last_line;
                last_unit_undo=last_unit;
                for(int i=0;i<14;i++)
                    unit_undo[i]=unit[i];
                
                nr_moves_undo=nr_moves;
                
                for(int i=0;i<n;i++)
                    for(int j=0;j<n;j++){
                        undo_cels[i][j].state=cels[i][j]->state;
                        undo_cels[i][j].color=cels[i][j]->color;
                        undo_cels[i][j].nr=cels[i][j]->nr;
                        undo_cels[i][j].nr_tex=cels[i][j]->nr_tex;
                        undo_cels[i][j].deleted=cels[i][j]->deleted;
                        undo_cels[i][j].last_nr=cels[i][j]->last_nr;
                        undo_cels[i][j].last_nr_tex=cels[i][j]->last_nr_tex;
                        undo_cels[i][j].last_color=cels[i][j]->last_color;
                        undo_cels[i][j].isGlobe=cels[i][j]->isGlobe;
                    }
                
                undo->setOpacity(255);
                
                to_animate[0].first=2;
                to_animate[0].second=i;
                
                
                
                auto scale=ScaleTo::create(0.1, globes_scale*1.09);
                g2[i]->runAction(scale);
                
                
                unit[i]=0;
                if(i+1==last_unit)
                    last_unit=-1;
                else
                    if(i+1!=last_line || !last_line){
                        nr_moves++;
                        updateMoves();
                    }
                cels[(int)globes_ind2[i].y][(int)globes_ind2[i].x]->state=1;
                last_line=i+1;
                cels[(int)globes_ind2[i].y][(int)globes_ind2[i].x]->color=last_line;
                ok=1;
                ltj=globes_ind2[i].x;
                lti=globes_ind2[i].y;
                cels[(int)globes_ind2[i].y][(int)globes_ind2[i].x]->nr=++shifts;
                cels[(int)globes_ind2[i].y][(int)globes_ind2[i].x]->nr_tex=-1;
                cels[(int)globes_ind2[i].y][(int)globes_ind2[i].x]->updateTexture();
                last_dir=0;
                for(int k=0;k<n;k++)
                    for(int j=0;j<n;j++)
                        if(cels[k][j]->color==last_line && (k!=(int)globes_ind2[i].y || j!=(int)globes_ind2[i].x)){
                            cels[k][j]->color=0;
                            cels[k][j]->state=0;
                            cels[k][j]->nr_tex=0;
                            cels[k][j]->updateTexture();
                        }
                break;
            }
    
    //sterge rest linie
    if(!ok){
        
        int i=0,j=0,color;
        long nr=0;
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                if(cels[i][j]->bg->getBoundingBox().containsPoint(touch->getLocation()) && cels[i][j]->state){
                    
                    
                    if(cels[i][j]->isGlobe)
                        goto negasit;
                    
                    
                    last_line_undo=last_line;
                    last_unit_undo=last_unit;
                    for(int i=0;i<14;i++)
                        unit_undo[i]=unit[i];
                    
                    nr_moves_undo=nr_moves;
                    
                    
                    color=cels[i][j]->color;
                    nr=cels[i][j]->nr;
                    ltj=j;
                    lti=i;
                    
                    int taj,tai;
                    long nr2=0;
                    int found=0;
                    for(int i=0;i<n;i++)
                        for(int j=0;j<n;j++){
                            if(cels[i][j]->isActive() && cels[i][j]->isGlobe==color){
                                if(!found){
                                    taj=j;
                                    tai=i;
                                    nr2=cels[i][j]->nr;
                                    found=1;
                                }
                                else
                                    if(cels[i][j]->nr<nr2){
                                        taj=j;
                                        tai=i;
                                    }
                            }
                            undo_cels[i][j].state=cels[i][j]->state;
                            undo_cels[i][j].color=cels[i][j]->color;
                            undo_cels[i][j].nr=cels[i][j]->nr;
                            undo_cels[i][j].nr_tex=cels[i][j]->nr_tex;
                            undo_cels[i][j].deleted=cels[i][j]->deleted;
                            undo_cels[i][j].last_nr=cels[i][j]->last_nr;
                            undo_cels[i][j].last_nr_tex=cels[i][j]->last_nr_tex;
                            undo_cels[i][j].last_color=cels[i][j]->last_color;
                            undo_cels[i][j].isGlobe=cels[i][j]->isGlobe;
                            
                        }
                    undo->setOpacity(255);
                    
                    for(int i=0;i<n_g;i++){
                        if(globes_ind1[i].x==taj && globes_ind1[i].y==tai){
                            auto scale=ScaleTo::create(0.1, globes_scale*1.09);
                            g1[i]->runAction(scale);
                            
                            to_animate[0].first=1;
                            to_animate[0].second=i;
                        }
                        if(globes_ind2[i].x==taj && globes_ind2[i].y==tai){
                            auto scale=ScaleTo::create(0.1, globes_scale*1.09);
                            g2[i]->runAction(scale);
                            
                            to_animate[0].first=2;
                            to_animate[0].second=i;
                        }
                    }
                    
                    
                    if(color!=last_line && last_unit!=-1 && color!=last_unit){
                        nr_moves++;
                        updateMoves();
                    }
                    if(last_unit!=-1)
                        last_unit=-1;
                    
                    last_line=color;
                    ok=1;
                    unit[color-1]=0;
                    goto gasit;
                }
        goto negasit;
    gasit:
        long max=0;
        
        int k1,l1,ok2=0;
        int k=0,l=0;
        for(int k=0;k<n;k++)
            for(int l=0;l<n;l++){
                if(cels[k][l]->color==color && cels[k][l]->nr > nr){
                    cels[k][l]->state=0;
                    cels[k][l]->color=0;
                    cels[k][l]->nr=0;
                    cels[k][l]->bg->setVisible(false);
                }
            }
        
        for(k=0;k<n;k++)
            for(l=0;l<n;l++)
                if(cels[k][l]->color==color && cels[k][l]->nr<nr){
                    max=cels[k][l]->nr;
                    k1=k;
                    l1=l;
                    ok2=1;
                    l++;
                    goto gasit2;
                }
    gasit2:
        
        for(;k<n;k++)
        {
            for(;l<n;l++)
                if(cels[k][l]->color==color && cels[k][l]->nr<nr && cels[k][l]->nr>max){
                    max=cels[k][l]->nr;
                    k1=k;
                    l1=l;
                    ok2=1;
                }
            l=0;
        }
        
        if(ok2)
        {
            int poz=getPosition(k1,l1,i,j);
            
            
            if(cels[k1][l1]->nr_tex==0){
                if(poz==3)
                    cels[i][j]->nr_tex=8;
                else
                    cels[i][j]->nr_tex=6;
            }
            if(cels[k1][l1]->nr_tex==1){
                if(poz==1)
                    cels[i][j]->nr_tex=9;
                else
                    cels[i][j]->nr_tex=7;
            }
            if(cels[k1][l1]->nr_tex==2){
                if(poz==1)
                    cels[i][j]->nr_tex=9;
                else
                    cels[i][j]->nr_tex=6;
            }
            if(cels[k1][l1]->nr_tex==3){
                if(poz==2)
                    cels[i][j]->nr_tex=7;
                else
                    cels[i][j]->nr_tex=6;
            }
            if(cels[k1][l1]->nr_tex==4){
                if(poz==3)
                    cels[i][j]->nr_tex=8;
                else
                    cels[i][j]->nr_tex=7;
            }
            if(cels[k1][l1]->nr_tex==5){
                if(poz==3)
                    cels[i][j]->nr_tex=8;
                else
                    cels[i][j]->nr_tex=9;
            }
            
            if(cels[k1][l1]->nr_tex==9)
                cels[i][j]->nr_tex=7;
            if(cels[k1][l1]->nr_tex==8)
                cels[i][j]->nr_tex=6;
            if(cels[k1][l1]->nr_tex==7)
                cels[i][j]->nr_tex=9;
            if(cels[k1][l1]->nr_tex==6)
                cels[i][j]->nr_tex=8;
            
            cels[i][j]->updateTexture();
            
            if(poz)
                last_dir=poz;
        }
    }
    
negasit:
    
    if(!ok && last_unit==-1){
        last_unit=last_line;
        last_line=0;
    }
    percent=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            percent+=cels[i][j]->state;
    
    
    sprintf(completed, "Completed: %.0f%%",percent/(n*n)*100);
    completed_label->setString(completed);
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    if(last_line)
        circle->setColor(culori[last_line-1]);

    
    return true;
    
    
}

void Level::onTouchMoved(Touch* touch, Event* event){
    
    if(!last_line || state_l)
        return;
    if(event && event->getType()!=Event::Type::CUSTOM)
        circle->setPosition(touch->getLocation());
    
    float x,y;
    
    x= CLAMP(touch->getLocation().x, cels[0][0]->bg->getPositionX(), cels[0][n-1]->bg->getPositionX()) ;
    y= CLAMP(touch->getLocation().y,cels[n-1][n-1]->bg->getPositionY(), cels[0][0]->bg->getPositionY());
    
    
    
    Point touch_location(x,y);
    
    if(cels[lti][ltj]->bg->getBoundingBox().containsPoint(touch_location))
        return;
    
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            if(cels[i][j]->bg->getBoundingBox().containsPoint(touch_location) &&  (abs(i-lti)<2&&abs(j-ltj)<2)){
                
                int dir=0;
                if(j>ltj)//dr
                    dir=1;
                if(j<ltj)//st
                    dir=2;
                if(i<lti)//sus
                    dir=3;
                if(i>lti)//jos
                    dir=4;
                
                if(!last_dir)
                    last_dir=dir;
                
             
                
                if(((abs(j-ltj)!=1 && abs(i-lti)!=1)  || (abs(j-ltj)==1 && abs(i-lti)==1)) && event->getType()!=Event::Type::CUSTOM){
                    
                    
                    int ok4=1;
                    if(cels[lti][ltj]->isGlobe)
                        ok4=0;
                    
                    Touch t;
                    Event e= EventCustom("diag");
                    
                    
                    if(!ok4)
                        last_dir=0;
                    
                    
                    
                    if(dir==3){
                        if(j<ltj){
                            
                            int ok1=1,ok2=1,mutat=0;
                            

                            if(cels[i+1][j]->isGlobe)
                                ok1=0;
                            if(cels[i][j+1]->isGlobe)
                                ok2=0;
                            
                            
                            if(i<n_g && !cels[i+1][j]->state && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height- cels[i+1][j]->bg->getPositionY());
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(j<n_g && !cels[i][j+1]->state &&ok2 && !mutat){
                                t.setTouchInfo(0, cels[i][j+1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY());
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            
                            if(j<n_g && !mutat && ok2){
                                t.setTouchInfo(0,cels[i][j+1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY()  );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(i<n_g && !mutat && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i+1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            //if(!mutat)
                            //return;
                        }
                        
                        if(j>ltj){
                            
                            int ok1=1,ok2=1,mutat=0;
                            
                            if(cels[i+1][j]->isGlobe)
                                ok1=0;
                            if(cels[i][j-1]->isGlobe)
                                ok2=0;
                            
                            
                            if(i<n_g && !cels[i+1][j]->state && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i+1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(j && !cels[i][j-1]->state &&ok2 && !mutat){
                                t.setTouchInfo(0, cels[i][j-1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            
                            if(j && !mutat && ok2){
                                t.setTouchInfo(0, cels[i][j-1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                                
                            }
                            
                            if(i<n_g && !mutat && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i+1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            //if(!mutat)
                            //return;
                        }
                    }
                    else{
                        
                        if(j<ltj){
                            
                            int ok1=1,ok2=1,mutat=0;
                            
                            
                            
                            if(cels[i-1][j]->isGlobe)
                                ok1=0;
                            if(cels[i][j+1]->isGlobe)
                                ok2=0;
                            
                            
                            if(i && !cels[i-1][j]->state && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i-1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(j<n_g && !cels[i][j+1]->state &&ok2 && !mutat){
                                t.setTouchInfo(0, cels[i][j+1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            
                            if(j<n_g && !mutat && ok2){
                                t.setTouchInfo(0, cels[i][j+1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(i && !mutat && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i-1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            //if(!mutat)
                            //return;
                            
                        }
                        
                        if(j>ltj){
                            
                            int ok1=1,ok2=1,mutat=0;
                            
                        
                            
                            if(cels[i-1][j]->isGlobe)
                                ok1=0;
                            if(cels[i][j-1]->isGlobe)
                                ok2=0;
                            
                            if(i && !cels[i-1][j]->state && ok1){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i-1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(j && !cels[i][j-1]->state &&ok2 && !mutat){
                                t.setTouchInfo(0, cels[i][j-1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            
                            if(j && !mutat && ok2){
                                t.setTouchInfo(0, cels[i][j-1]->bg->getPositionX(),height-  cels[i][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            
                            if(i &&ok1 && !mutat){
                                t.setTouchInfo(0, cels[i][j]->bg->getPositionX(),height-  cels[i-1][j]->bg->getPositionY() );
                                onTouchMoved(&t,&e);
                                onTouchMoved(&t,&e);
                                mutat=1;
                            }
                            //if(!mutat)
                            //return;
                        }
                        
                    }
                    
                    goto end;
                    
                }
                
                
                
                
                
                if(cels[i][j]->isGlobe && cels[i][j]->isGlobe!=last_line && cels[lti][ltj]->nr_tex==-1){
                    last_dir=0;
                    cels[lti][ltj]->updateTexture();
                    return;
                }
                
                
                if(unit[last_line-1] && cels[i][j]->color!=last_line)
                    return;
                
                
                if(!cels[i][j]->state){
                    
                    if( cels[i][j]->isGlobe){
                        if(cels[i][j]->isGlobe!=last_line){
    
                            return;
                           
                        }
                        else{
                            
                            
                            unit[cels[i][j]->isGlobe-1]=1;
                            
                           
                            
                            cels[i][j]->state=1;
                            cels[i][j]->nr=++shifts;
                            cels[i][j]->color=last_line;
                            
                            if(dir==1)
                                cels[i][j]->nr_tex=9;
                            if(dir==2)
                                cels[i][j]->nr_tex=7;
                            if(dir==3)
                                cels[i][j]->nr_tex=8;
                            if(dir==4)
                                cels[i][j]->nr_tex=6;
                            
                            cels[i][j]->updateTexture();
                            cels[i][j]->bg->setVisible(true);
                            
                            if(dir==1 || dir==2){//dr st
                                if(last_dir!=dir){
                                    if(last_dir==3 ){
                                        if(dir==1)
                                            cels[lti][ltj]->nr_tex=2;
                                        else
                                            cels[lti][ltj]->nr_tex=3;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                    if(last_dir==4){
                                        if(dir==1)
                                            cels[lti][ltj]->nr_tex=5;
                                        else
                                            cels[lti][ltj]->nr_tex=4;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                            }
                            if(dir==3 || dir==4){//sus jos
                                if(last_dir!=dir){
                                    if(last_dir==1){
                                        if(dir==3)
                                            cels[lti][ltj]->nr_tex=4;
                                        else
                                            cels[lti][ltj]->nr_tex=3;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                    if(last_dir==2){
                                        if(dir==4)
                                            cels[lti][ltj]->nr_tex=2;
                                        else
                                            cels[lti][ltj]->nr_tex=5;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                            }
                            
                            if(cels[lti][ltj]->nr_tex==6 || cels[lti][ltj]->nr_tex==8){
                                cels[lti][ltj]->nr_tex=0;
                                cels[lti][ltj]->updateTexture();
                            }
                            if(cels[lti][ltj]->nr_tex==7 || cels[lti][ltj]->nr_tex==9){
                                cels[lti][ltj]->nr_tex=1;
                                cels[lti][ltj]->updateTexture();
                            }
                            
                            
                            last_unit=cels[i][j]->isGlobe;
                            
                            did_unit=1;
                            
                            if(globes_ind1[last_unit-1].x==j && globes_ind1[last_unit-1].y==i){
                                to_animate[1].first=1;
                                to_animate[1].second=last_unit-1;
                                auto scale=ScaleTo::create(0.1, globes_scale*1.09);
                                g1[last_unit-1]->runAction(scale);
                            }
                            else{
                                to_animate[1].first=2;
                                to_animate[1].second=last_unit-1;
                                auto scale=ScaleTo::create(0.1, globes_scale*1.09);
                                g2[last_unit-1]->runAction(scale);
                            }
                            
                        }
                
                    }
                                     
                    
                    if(!cels[lti][ltj]->state)
                        goto end;

                    
                    
                    if(cels[lti][ltj]->nr_tex==-1 && cels[lti][ltj]->isGlobe ){
                        if(dir==1)
                            cels[lti][ltj]->nr_tex=7;
                        if(dir==2)
                            cels[lti][ltj]->nr_tex=9;
                        if(dir==3)
                            cels[lti][ltj]->nr_tex=6;
                        if(dir==4)
                            cels[lti][ltj]->nr_tex=8;
                        
                        cels[lti][ltj]->updateTexture();
                    }
                    
                    
                    
                    if(dir==1 || dir==2){//dr st
                        if(last_dir!=dir){
                            if(last_dir==3 ){
                                if(dir==1){
                                    cels[i][j]->nr_tex=9;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=2;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                                else{
                                    cels[i][j]->nr_tex=7;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=3;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                    
                                }
                            }
                            if(last_dir==4){
                                if(dir==1){
                                    cels[i][j]->nr_tex=9;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=5;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                                else{
                                    cels[i][j]->nr_tex=7;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=4;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                            }
                        }
                        else{
                            if(dir==2)
                                cels[i][j]->nr_tex=7;
                            else
                                cels[i][j]->nr_tex=9;
                            cels[i][j]->updateTexture();
                        }
                    }
                    if(dir==3 || dir==4){//sus jos
                        if(last_dir!=dir){
                            if(last_dir==1){
                                if(dir==3){
                                    cels[i][j]->nr_tex=8;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=4;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                                else{
                                    cels[i][j]->nr_tex=6;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=3;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                            }
                            if(last_dir==2){
                                if(dir==4){
                                    cels[i][j]->nr_tex=6;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=2;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                }
                                else{
                                    cels[i][j]->nr_tex=8;
                                    cels[i][j]->updateTexture();
                                    if(!cels[lti][ltj]->isGlobe){
                                        cels[lti][ltj]->nr_tex=5;
                                        cels[lti][ltj]->updateTexture();
                                    }
                                    
                                }
                            }
                            
                        }
                        else{
                            if(dir==4)
                                cels[i][j]->nr_tex=6;
                            else
                                cels[i][j]->nr_tex=8;
                            cels[i][j]->updateTexture();
                        }
                    }
                    
                    
                    
                    
                    if( (cels[lti][ltj]->nr_tex==6 || cels[lti][ltj]->nr_tex==8 ) && !cels[lti][ltj]->isGlobe){
                        cels[lti][ltj]->nr_tex=0;
                        cels[lti][ltj]->updateTexture();
                    }
                    if( (cels[lti][ltj]->nr_tex==7 || cels[lti][ltj]->nr_tex==9 ) && !cels[lti][ltj]->isGlobe){
                        cels[lti][ltj]->nr_tex=1;
                        cels[lti][ltj]->updateTexture();
                    }
                    
                    

                    
                    
                    last_dir=dir;
                    cels[i][j]->state=1;
                    cels[i][j]->color=last_line;
                    cels[i][j]->nr=++shifts;
                    cels[i][j]->updateTexture();
                    cels[i][j]->bg->setVisible(true);
                    
                    
                    if(!cels[i][j]->deleted){
                        cels[i][j]->last_color=last_line;
                        cels[i][j]->last_nr=shifts;
                        cels[i][j]->last_nr_tex=cels[i][j]->last_nr_tex;
                    }
                    ltj=j;
                    lti=i;
                    
                    
                }
                else{
                    
                    
                    
                    if(cels[i][j]->isGlobe && cels[i][j]->isGlobe!=last_line)
                        return;
                    
                    int color=cels[i][j]->color;
                    int nr=cels[i][j]->nr;
                    
                    if(color==last_line ){
                        
                        if(unit[last_line-1]){
                            did_unit=0;
                            unit[last_line-1]=0;
                            
                            if(to_animate[1].first==1){
                                auto scale=ScaleTo::create(0.1, globes_scale);
                                g1[to_animate[1].second]->runAction(scale);
                            }
                            else{
                                auto scale=ScaleTo::create(0.1, globes_scale);
                                g2[to_animate[1].second]->runAction(scale);
                            }
                            
                        }
                        
                        
                        long max=0;
                        int k1=0,l1=0,ok2=0;
                        int k=0,l=0;
                        for(int k=0;k<n;k++)
                            for(int l=0;l<n;l++){
                                if(cels[k][l]->color==color && cels[k][l]->nr > nr){
                                    cels[k][l]->state=0;
                                    cels[k][l]->color=0;
                                    cels[k][l]->nr=0;
                                    cels[k][l]->nr_tex=-1;
                                    cels[k][l]->updateTexture();
                                }
                                
                            }
                        for(k=0;k<n;k++)
                            for(l=0;l<n;l++)
                                if(cels[k][l]->color==color && cels[k][l]->nr<nr ){
                                    max=cels[k][l]->nr;
                                    k1=k;
                                    l1=l;
                                    ok2=1;
                                    goto gasit2;
                                }
                    gasit2:
                        for(;k<n;k++)
                        {
                            for(;l<n;l++)
                                if(cels[k][l]->color==color && cels[k][l]->nr < nr && cels[k][l]->nr>max ){
                                    max=cels[k][l]->nr;
                                    k1=k;
                                    l1=l;
                                    ok2=1;
                                }
                            l=0;
                        }
                        
                        
                        int poz=getPosition(k1,l1,i,j);
                        
                        
                        int ok3=1;
                        
                        
                        if(cels[i][j]->isGlobe)
                            ok3=0;
                        
                        if(!ok3){
                            cels[i][j]->nr_tex=-1;
                            cels[i][j]->updateTexture();
                        }
                        
                        if(ok2 && cels[k1][l1]->state){
                            
                            if(cels[k1][l1]->nr_tex==0){
                                if(poz==3)
                                    cels[i][j]->nr_tex=8;
                                else
                                    cels[i][j]->nr_tex=6;
                            }
                            if(cels[k1][l1]->nr_tex==1){
                                if(poz==1)
                                    cels[i][j]->nr_tex=9;
                                else
                                    cels[i][j]->nr_tex=7;
                            }
                            if(cels[k1][l1]->nr_tex==2){
                                if(poz==1)
                                    cels[i][j]->nr_tex=9;
                                else
                                    cels[i][j]->nr_tex=6;
                            }
                            if(cels[k1][l1]->nr_tex==3){
                                if(poz==2)
                                    cels[i][j]->nr_tex=7;
                                else
                                    cels[i][j]->nr_tex=6;
                            }
                            if(cels[k1][l1]->nr_tex==4){
                                if(poz==3)
                                    cels[i][j]->nr_tex=8;
                                else
                                    cels[i][j]->nr_tex=7;
                            }
                            if(cels[k1][l1]->nr_tex==5){
                                if(poz==3)
                                    cels[i][j]->nr_tex=8;
                                else
                                    cels[i][j]->nr_tex=9;
                            }
                            
                            if(cels[k1][l1]->nr_tex==9)
                                cels[i][j]->nr_tex=7;
                            if(cels[k1][l1]->nr_tex==8)
                                cels[i][j]->nr_tex=6;
                            if(cels[k1][l1]->nr_tex==7)
                                cels[i][j]->nr_tex=9;
                            if(cels[k1][l1]->nr_tex==6)
                                cels[i][j]->nr_tex=8;
                            
                            cels[i][j]->updateTexture();
                            
                        }
                        
                        
                        ltj=j;
                        lti=i;
                        
                        if(cels[i][j]->nr_tex==-1)
                            last_dir=0;
                        else
                            last_dir=poz;
                        
                        
                    }
                    else{
                        
                        
                        //GameManager::getInstance()->playSound(3);
                        
                        if(!deleted[color-1]){
                            deleted[color-1]=1;
                            delete_point[color-1].x=j;
                            delete_point[color-1].y=i;
                        }
                        
                        for(int k=0;k<n;k++)
                            for(int l=0;l<n;l++){
                                
                                if(cels[k][l]->color==color && cels[k][l]->nr>=nr  && !cels[k][l]->deleted){
                                    
                                    if(cels[k][l]->nr<cels[(int)delete_point[color-1].y][(int)delete_point[color-1].x]->nr){
                                        delete_point[color-1].x=l;
                                        delete_point[color-1].y=k;
                                    }
                                    
                                    cels[k][l]->last_color=cels[k][l]->color;
                                    cels[k][l]->last_nr=cels[k][l]->nr;
                                    cels[k][l]->last_nr_tex=cels[k][l]->nr_tex;
                                    cels[k][l]->state=0;
                                    cels[k][l]->color=0;
                                    cels[k][l]->nr=0;
                                    cels[k][l]->nr_tex=-1;
                                    cels[k][l]->deleted=1;
                                    cels[k][l]->updateTexture();
                                }
                                
                            }
                        
                        //TouchedMove(x1, y1,0,1);
                        
                        return;
                        
                    }

                    
                    
                }
                
                
                
                for(int m=0;m<n_g;m++){
                    int k=0,l=0,k1=0,l1=0,ok=0;
                    long cautat=0;
                    if(deleted[m]){
                        for(k=0;k<n;k++)
                            for(l=0;l<n;l++){
                                //if(k==i && l==j)
                                //continue;
                                if(cels[k][l]->deleted && cels[k][l]->last_color==m+1 && cels[k][l]->state ){
                                    cautat=cels[k][l]->last_nr;
                                    ok=1;
                                    k1=k;
                                    l1=l++;
                                    goto cica_am_gasit;
                                }
                            }
                        
                    cica_am_gasit:
                        for(;k<n;k++){
                            for(;l<n;l++){
                                //if(k==i && l==j)
                                //continue;
                                if(cels[k][l]->deleted && cels[k][l]->last_color==m+1 && cels[k][l]->state && cels[k][l]->last_nr<cautat){
                                    cautat=cels[k][l]->last_nr;
                                    k1=k;
                                    l1=l;
                                    cautat=cels[k][l]->last_nr;
                                }
                                
                            }
                            l=0;
                        }
                        
                        
                        if(ok){
                            delete_point[m].x=l1;
                            delete_point[m].y=k1;
                        }
                        if(!ok && !cels[(int)delete_point[m].y][(int)delete_point[m].x]->state){
                            for(int k=0;k<n;k++)
                                for(int l=0;l<n;l++)
                                    if(cels[k][l]->deleted && cels[k][l]->last_color==m+1){
                                        cels[k][l]->state=1;
                                        cels[k][l]->deleted=0;
                                        cels[k][l]->color=cels[k][l]->last_color;
                                        cels[k][l]->nr_tex=cels[k][l]->last_nr_tex;
                                        cels[k][l]->nr=cels[k][l]->last_nr;
                                        cels[k][l]->updateTexture();
                                    }
                            deleted[m]=0;
                        }
                        
                    }
                }
                
                
                for(int m=0;m<n_g;m++){
                    if(deleted[m]){
                        for(int k=0;k<n;k++)
                            for(int l=0;l<n;l++){
                                if(cels[k][l]->deleted && !cels[k][l]->state && cels[k][l]->last_color==m+1 && cels[k][l]->last_nr<=cels[(int)delete_point[m].y][(int)delete_point[m].x]->last_nr)
                                {
                                    cels[k][l]->state=1;
                                    cels[k][l]->deleted=0;
                                    cels[k][l]->color=cels[k][l]->last_color;
                                    cels[k][l]->nr_tex=cels[k][l]->last_nr_tex;
                                    cels[k][l]->nr=cels[k][l]->last_nr;
                                    cels[k][l]->updateTexture();
                                }
                            }
                    }
                }
                
                
                
            }
        }
    

end:
    percent=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            percent+=cels[i][j]->state;
    

    sprintf(completed, "Completed: %.0f%%",percent/(n*n)*100);
    completed_label->setString(completed);
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    
}

void Level::onTouchEnded(Touch* touch, Event* event){
    
    if(state_l)
        return;
    
    circle->setVisible(false);
    circle->setColor(Color3B(255, 255, 255));
    
    
    
    
    if(did_unit){
        did_unit=0;
        last_line=0;
        GameManager::getInstance()->playSound(2);
    }
    
    
    if(to_animate[0].first==1){
        auto scale=ScaleTo::create(0.1, globes_scale);
        g1[to_animate[0].second]->runAction(scale);
    }
    else{
        auto scale=ScaleTo::create(0.1, globes_scale);
        g2[to_animate[0].second]->runAction(scale);
    }
    
    if(to_animate[1].first==1){
        auto scale=ScaleTo::create(0.1, globes_scale);
        g1[to_animate[1].second]->runAction(scale);
    }
    else{
        auto scale=ScaleTo::create(0.1, globes_scale);
        g2[to_animate[1].second]->runAction(scale);
    }
    
    
    for(int i=0;i<n_g;i++)
        deleted[i]=0;
    
    
    percent=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            percent+=cels[i][j]->state;
    
    sprintf(completed, "Completed: %.0f%%",percent/(n*n)*100);
    completed_label->setString(completed);
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    int connection_broke=0;
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(cels[i][j]->deleted){
                unit[cels[i][j]->last_color-1]=0;
                cels[i][j]->deleted=0;
                connection_broke=1;
            }
    
    if(connection_broke)
        GameManager::getInstance()->playSound(3);
    
    
    
    for(int i=0;i<n_g;i++)
        if(!unit[i])
            return;
    
    
    int ok=0;
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(!cels[i][j]->state){
                ok=0;
                for(int k=0;k<n_g;k++){
                    if( (globes_ind1[k].x==j && globes_ind1[k].y==i) ||  (globes_ind2[k].x==j && globes_ind2[k].y==i))
                        ok=1;
                }
                if(!ok)
                    return;
            }
    
    
    
    int result;
    
    
    if(nr_moves==n_g)
        //current_pack[current-1]=4;
        result=4;
    else{
        if(nr_moves==n_g+1)
            //current_pack[current-1]=3;
            result=3;
        if(nr_moves>n_g+1)
            //current_pack[current-1]=2;
            result=2;
    }
    
    if(result>current_pack[current-1])
        current_pack[current-1]=result;
    
    nr_stars=result-1;
    
    float percent=0;
    for(int i=0;i<75;i++)
        if(current_pack[i]==2)
            percent++;
    
    
    if(current<75 && !current_pack[current])
        current_pack[current]=1;
    GameManager::getInstance()->updateSave(_pack, current_pack);

    
    
    state_l=1;
    
    perfect->setOpacity(0);
    not_perfect->setOpacity(0);
    
    
    auto move=MoveBy::create(0.4, Point(0,grid->getBoundingBox().size.height));
    
    auto move2=MoveBy::create(0.5, Point(0,-grid->getBoundingBox().size.height));
    
    
    back->runAction(move->clone());
    level_nr->runAction(move->clone());
    moves_label[0]->runAction(move->clone());
    moves_label[1]->runAction(move->clone());
    moves_label[2]->runAction(move->clone());
    time_label->runAction(move->clone());
    completed_label->runAction(move);
    
    
    undo->runAction(move2->clone());
    previous->runAction(move2->clone());
    next->runAction(move2->clone());
    reset->runAction(move2->clone());
    hints->runAction(move2->clone());
    
    
    this->scheduleOnce(schedule_selector(Level::delayedRtt), 0.4);
    
    
    
    
}


void Level::delayedRtt(float dt){
    blur1->beginWithClear(0, 0, 0, 1);
    this->visit();
    blur1->end();
    GameManager::getInstance()->playSound(4);
    this->scheduleOnce(schedule_selector(Level::delayedBlur), 0);
}

void Level::delayedBlur(float dt){
    
    
    
    
    blur_sprite->initWithTexture(blur1->getSprite()->getTexture(), blur1->getSprite()->getTextureRect());
    //blur_sprite->setFlippedY(false);
    
    blur_sprite->getTexture()->setAntiAliasTexParameters();
    
    
    blur2->beginWithClear(0, 0, 0, 1);
    blur_sprite->draw(Director::getInstance()->getRenderer(), getNodeToParentTransform(), 0);
    blur2->end();

    
    this->scheduleOnce(schedule_selector(Level::delayedBlur2), 0);
  
    
    perfect->setPosition(perfect_pos);
    not_perfect->setPosition(not_perfect_pos);
    next_level->setPosition(next_level_pos);
    try_again->setPosition(try_again_pos);
    
    
    for(int i=0;i<3;i++){
        finish_stars[i]->stopAllActions();
        finish_stars2[i]->stopAllActions();
    }
   
    next_level->setVisible(true);
    try_again->setVisible(true);
}


void Level::delayedBlur2(float dt){
    
    
    
    
    blur_sprite->initWithTexture(blur2->getSprite()->getTexture(),blur2->getSprite()->getTextureRect() );
    blur_sprite->getTexture()->setAntiAliasTexParameters();
    
    
    
    blur1->beginWithClear(0, 0, 0, 1);
    blur_sprite->draw(Director::getInstance()->getRenderer(), getNodeToParentTransform(), 0);
    blur1->end();

    
    
    //blur_mesaj=Sprite::createWithTexture(blur1->getSprite()->getTexture());
    blur_mesaj->setTexture(blur1->getSprite()->getTexture());
    blur_mesaj->setTextureRect(blur1->getSprite()->getTextureRect());
    blur_mesaj->setPosition(width/2, height/2);
    blur_mesaj->setOpacity(0);
    blur_mesaj->setVisible(true);
    
    
    overlay->setPosition(width/2,height/2);
    overlay->setOpacity(0);
    
    auto fade=FadeIn::create(0.7);
    
    overlay->runAction(fade->clone());
    blur_mesaj->runAction(fade->clone());

    
    
    if(nr_moves==n_g){
        perfect->setVisible(true);
        perfect->setOpacity(0);
        perfect->runAction(fade);
    }
    else{
        not_perfect->setVisible(true);
        not_perfect->setOpacity(0);
        not_perfect->runAction(fade);
    }
    
    
    
    
    auto fadeout=FadeOut::create(0.7);
    
    undo->runAction(fadeout->clone());
    previous->runAction(fadeout->clone());
    reset->runAction(fadeout->clone());
    next->runAction(fadeout->clone());
    hints->runAction(fadeout->clone());

    
    for(int i=0;i<3;i++){
        finish_stars[i]->setVisible(true);
        finish_stars[i]->setOpacity(255);
        finish_stars[i]->setPosition(finish_star_pos[i]);
        finish_stars2[i]->setVisible(true);
        finish_stars2[i]->setOpacity(255);
        finish_stars2[i]->setScale(0);
        finish_stars2[i]->setPosition(finish_star_pos[i]);
    }
    
    this->scheduleOnce(schedule_selector(Level::delayedStar1), 0.2);
    
}



void Level::delayedStar1(float dt){
    auto scale=ScaleTo::create(0.5, 1);
    auto ease=EaseBackOut::create(scale);
    finish_stars2[0]->runAction(ease);
    particle_stars[0]->resetSystem();
    
    if(nr_stars>1)
        this->scheduleOnce(schedule_selector(Level::delayedStar2), 0.5);
}

void Level::delayedStar2(float dt){
    auto scale=ScaleTo::create(0.5, 1);
    auto ease=EaseBackOut::create(scale);
    finish_stars2[1]->runAction(ease);
    particle_stars[1]->resetSystem();
    if(nr_stars>2)
        this->scheduleOnce(schedule_selector(Level::delayedStar3), 0.5);
}

void Level::delayedStar3(float dt){
    auto scale=ScaleTo::create(0.5, 1);
    auto ease=EaseBackOut::create(scale);
    finish_stars2[2]->runAction(ease);
    particle_stars[2]->resetSystem();
}


void Level::delayedLoad(float dt){
    Load(current);
}


void Level::delayedPrevious(float dt){
    Load(current-1);
}

void Level::delayedNext(float dt){
    Load(current+1);
}

void Level::updateTime(float dt) {
    seconds++;
    char buffer[100];
    sprintf(buffer, "Time: %ds",seconds);
    time_label->setString(buffer);
    //time_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
}



void Level::menuNextLevelClick(cocos2d::Ref *pSender){
    
    
    
    auto move=MoveBy::create(0.6, Point(-blur_mesaj->getBoundingBox().size.width, 0));
    overlay->runAction(move->clone());
    blur_mesaj->runAction(move->clone());
    
    
    perfect->runAction(move->clone());
    not_perfect->runAction(move->clone());
    next_level->runAction(move->clone());
    try_again->runAction(move->clone());
    
    
    for(int i=0;i<3;i++){
        finish_stars[i]->runAction(move->clone());
        finish_stars2[i]->runAction(move->clone());
    }
    
    state_l=0;
    
    
    undo->stopAllActions();
    undo->setOpacity(255);
    previous->setOpacity(255);
    previous->stopAllActions();
    reset->setOpacity(255);
    reset->stopAllActions();
    next->setOpacity(255);
    next->stopAllActions();
    hints->setOpacity(255);
    hints->stopAllActions();
    
    if(current<75)
        this->scheduleOnce(schedule_selector(Level::delayedNext), 0);
    else
    {
        if(_pack<9){
           
            int unlock[5];
            GameManager::getInstance()->getUnlockedPacks(unlock);
            if(_pack+1>4 && !unlock[_pack+1-5]){
                Menu3::hscroll=0;
                Menu2::scroll=0;
                GameManager::getInstance()->setScene(5);
            }
            else{
                Menu3::hscroll=0;
                GameManager::getInstance()->setPack(_pack+1);
                GameManager::getInstance()->setScene(6);
            }
        }
        else{
            Menu3::hscroll=0;
            Menu2::scroll=0;
            GameManager::getInstance()->setScene(5);
            return;

        }
    }
    
    
    
    
    move=MoveBy::create(0.6, Point(0,-grid->getBoundingBox().size.height));
    
    auto move2=MoveBy::create(0.6, Point(0,grid->getBoundingBox().size.height));
    
    //auto easing=EaseBackInOut ::create(move);
    //auto easing2=EaseBackInOut ::create(move2);
    
    back->runAction(move->clone());
    level_nr->runAction(move->clone());
    moves_label[0]->runAction(move->clone());
    moves_label[1]->runAction(move->clone());
    moves_label[2]->runAction(move->clone());
    time_label->runAction(move->clone());
    completed_label->runAction(move);
    
    
    undo->runAction(move2->clone());
    previous->runAction(move2->clone());
    next->runAction(move2->clone());
    reset->runAction(move2->clone());
    hints->runAction(move2->clone());
    
    
    
    
    GameManager::getInstance()->playSound(1);
}

void Level::menuTryAgainClick(cocos2d::Ref *pSender){
    
    
    auto fade=FadeOut::create(0.6);
    
    overlay->runAction(fade->clone());
    blur_mesaj->runAction(fade);
    
    
    for(int i=0;i<3;i++){
        finish_stars[i]->setVisible(false);
        finish_stars2[i]->setVisible(false);
    }
    
    

    //GameManager::getInstance()->playSound(1);
    
    
    auto move=MoveBy::create(0.6, Point(0,-grid->getBoundingBox().size.height));
    
    auto move2=MoveBy::create(0.6, Point(0,grid->getBoundingBox().size.height));
    //auto easing=EaseBackInOut ::create(move);
    //auto easing2=EaseBackInOut ::create(move2);
    
    back->runAction(move->clone());
    level_nr->runAction(move->clone());
    moves_label[0]->runAction(move->clone());
    moves_label[1]->runAction(move->clone());
    moves_label[2]->runAction(move->clone());
    time_label->runAction(move->clone());
    completed_label->runAction(move);
    
    
    undo->runAction(move2->clone());
    previous->runAction(move2->clone());
    next->runAction(move2->clone());
    reset->runAction(move2->clone());
    hints->runAction(move2->clone());
    

    auto fadein=FadeIn::create(0.7);
    
    undo->runAction(fadein->clone());
    previous->runAction(fadein->clone());
    reset->runAction(fadein->clone());
    next->runAction(fadein->clone());
    hints->runAction(fadein->clone());
    
    menuResetClick(NULL);
    state_l=0;
    perfect->setVisible(false);
    not_perfect->setVisible(false);
    next_level->setVisible(false);
    try_again->setVisible(false);
    
}


void Level::menuUndoClick(cocos2d::Ref *pSender){
    if(!nr_moves || state_l)
        return;
    
    GameManager::getInstance()->playSound(1);
    
    last_line=last_line_undo;
    nr_moves=nr_moves_undo;
    updateMoves();
    
    last_unit=last_unit_undo;
    for(int i=0;i<14;i++)
        unit[i]=unit_undo[i];
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            cels[i][j]->state=undo_cels[i][j].state;
            cels[i][j]->color=undo_cels[i][j].color;
            cels[i][j]->nr=undo_cels[i][j].nr;
            cels[i][j]->nr_tex=undo_cels[i][j].nr_tex;
            cels[i][j]->deleted=undo_cels[i][j].deleted;
            cels[i][j]->last_nr=undo_cels[i][j].last_nr;
            cels[i][j]->last_nr_tex=undo_cels[i][j].last_nr_tex;
            cels[i][j]->last_color=undo_cels[i][j].last_color;
            cels[i][j]->isGlobe=undo_cels[i][j].isGlobe;
            cels[i][j]->updateTexture();
        }
    undo->setOpacity(255/2);
}

void Level::menuNextClick(cocos2d::Ref* pSender){
    
    if(current<75 && !state_l &&current_pack[current]){
        
        
        //line_textures->getTexture()->setAliasTexParameters();
        rtt->setVisible(false);
        rtt->setPosition(grid->getPosition());
        
        rtt->begin();
        this->visit();
        rtt->end();
        
        
        rtt->setVisible(true);
        
        rtt->stopAllActions();
        
        auto move=MoveBy::create(0.6, Point(-grid->getBoundingBox().size.width*1.1,0));
        rtt->runAction(move);
        this->scheduleOnce(schedule_selector(Level::delayedNext), 0);
        
        GameManager::getInstance()->playSound(1);
        
    }
}
void Level::menuPreviousClick(cocos2d::Ref* pSender){
    
    if(current>1 && !state_l){
        //line_textures->getTexture()->setAliasTexParameters();
        rtt->setVisible(false);
        rtt->setPosition(grid->getPosition());
        rtt->begin();
        this->visit();
        rtt->end();

        rtt->setVisible(true);
        
        rtt->stopAllActions();
        
        auto move=MoveBy::create(0.6, Point(grid->getBoundingBox().size.width*1.1,0));
        rtt->runAction(move);
        
        this->scheduleOnce(schedule_selector(Level::delayedPrevious), 0);
        GameManager::getInstance()->playSound(1);
    }
}
void Level::menuResetClick(cocos2d::Ref* pSender){
   
    
    
    char buffer[100];
    
    seconds=0;
    sprintf(buffer, "Time: 0s");
    time_label->setString(buffer);
    //time_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    if(!nr_moves)
        return;
    
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            cels[i][j]->color=0;
            cels[i][j]->nr=0;
            cels[i][j]->nr_tex=-1;
            cels[i][j]->state=0;
            cels[i][j]->updateTexture();
        }
    shifts=0;
    nr_moves=0;
    percent=0;
    
    for(int i=0;i<14;i++)
        unit[i]=0;
    
    current_hint=0;
    last_line=0;
    last_unit=-1;
    

    
    sprintf(buffer, "Completed: 0%%");
    completed_label->setString(buffer);
    
    //completed_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    completed_label->setPosition(roundf(completed_label->getPositionX()),roundf(completed_label->getPositionY()));
    completed_label->setFontFillColor(Color3B(255, 255, 255));
    
    
    
    updateMoves();
    
    
    GameManager::getInstance()->playSound(3);
}


void Level::delayedAnimatedHint(float dt){
    if(hint_vector.size()>1){

        onTouchMoved(&hint_vector.back(), e);
        onTouchMoved(&hint_vector.back(), e);
        onTouchMoved(&hint_vector.back(), e);
        onTouchMoved(&hint_vector.back(), e);
        hint_vector.pop_back();
    }
    
    if(hint_vector.size()==1){
        onTouchEnded(&hint_vector.back(), e);
        hint_vector.pop_back();
        this->unschedule(schedule_selector(Level::delayedAnimatedHint));
    }
}

void Level::updateHints(){
    hints_label->setString(itoa(ajutoare));
   // hints_label->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    GameManager::getInstance()->setHints(ajutoare);
    if(!ajutoare)
        hints->setOpacity(255/2);
}

void Level::menuHintsClick(cocos2d::Ref* pSender){
    
    if(ajutoare<=0){
        GameManager::getInstance()->showHintsAlert();
        return;
    }
    
    
    if(hint_vector.size())
        return;
    
    if(state_l)
        return;
    
    
    int i=0;
    for(i=0;i<n_g;i++)
        if(!unit[i])
            break;
    if(i==n_g)
        if(current_hint<n_g)
            current_hint++;
        else
            current_hint=0;
    else
        current_hint=i;
    
    t->setTouchInfo(0, cels[(int)paths[current_hint].paths[0].y][(int)paths[current_hint].paths[0].x]->bg->getPositionX(), height-cels[(int)paths[current_hint].paths[0].y][(int)paths[current_hint].paths[0].x]->bg->getPositionY());
    
    onTouchBegan(t, e);
    
    
    
    
    Touch T;
    
    T.setTouchInfo(0, cels[(int)paths[current_hint].paths[paths[current_hint].length-1].y][(int)paths[current_hint].paths[paths[current_hint].length-1].x]->bg->getPositionX(),height- cels[(int)paths[current_hint].paths[paths[current_hint].length-1].y][(int)paths[current_hint].paths[paths[current_hint].length-1].x]->bg->getPositionY());
    
    
    hint_vector.push_back(T);
    
    
    
    for(i=paths[current_hint].length-1;i>=0;i--){
        Touch T;
        T.setTouchInfo(0, cels[(int)paths[current_hint].paths[i].y][(int)paths[current_hint].paths[i].x]->bg->getPositionX(),height- cels[(int)paths[current_hint].paths[i].y][(int)paths[current_hint].paths[i].x]->bg->getPositionY());
        
        hint_vector.push_back(T);
        if(i==paths[current_hint].length-1){
            Touch t1=T,t2=T,t3=T;
            hint_vector.push_back(t1);
            hint_vector.push_back(t2);
            //hint_vector.push_back(t3);
        }
    }
    
    this->schedule(schedule_selector(Level::delayedAnimatedHint), 0.09, paths[current_hint].length+2, 0);
    
    
    current_hint++;
    ajutoare--;
    updateHints();
    
    return;

}



