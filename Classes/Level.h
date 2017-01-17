//
//  Level.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "cocos2d.h"
using namespace cocos2d;


class SpriteBlur : public Sprite
{
public:
    ~SpriteBlur();
    bool initWithTexture(Texture2D* texture, const cocos2d::Rect&  rect);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    void initProgram();
    
    static SpriteBlur* create();
    
    GLProgram *shader;
    
    
    
protected:
    
    void onDraw(const Mat4 &transform, uint32_t flags);
    GLuint u_texelWidthOffset;
    GLuint u_texelHeightOffset;
    
    float texelWidthOffset;
    float texelHeightOffset;
    
    int h_pass;
    int v_pass;
    
    
    CustomCommand _customCommand;
};


struct cell{
    Sprite *bg;
    int state;
    cell();
    int color;
    long nr;
    int nr_tex;
    int deleted;
    long last_nr;
    int last_nr_tex;
    int last_color;
    void updateTexture();
    int isActive();
    int isGlobe;
};

struct cell_undo{
    int state;
    int color;
    long nr;
    int nr_tex;
    int deleted;
    long last_nr;
    int last_nr_tex;
    int last_color;
    int isGlobe;
};


struct path{
    int length;
    Vec2 paths[200];
    path();
    void add(int,int);
};


class Level :public cocos2d::LayerColor{
public:
    static cocos2d::Scene* createScene();
    Scene* scene;
    Layer* layer;
    virtual bool init();
    CREATE_FUNC(Level);
    
    void menuBackClick(cocos2d::Ref* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

    void Load(int);
    
    
    MenuItemSprite *back;
    
    int _pack,_level;
    LabelTTF *level_nr;
    //from old version
    
    int n,n_g;//nr of lines-cols, nr of points to join
    int **level;
    int current;
    int nr_moves;
    int state_l;
    int ajutoare;
    int width,height;
    int skin;
    int labels;

    
    struct path paths[14];
    int current_hint;
    char moves[50];
    char time[50];
    char completed[50];
    float percent;
    
    int current_pack[75];
    /*
    int pack_nr=1;
    int pack_nr2=1;
    int pack_dificulty=1;*/

    Sprite *grid,*g1[14],*g2[14],*circle,*labels1[14],*labels2[14];
    cell *cels[14][14];
    cell_undo undo_cels[14][14];
    int nr_moves_undo;
    kmVec2 globes_ind1[14],globes_ind2[14];
    
    
    static SpriteBatchNode *line_textures;
    static Color3B culori[14];
    

    
    int intersectGlobes(float,float);
    
    
    
    
    void delayedStar1(float dt);
    void delayedStar2(float dt);
    void delayedStar3(float dt);
    void delayedAnimatedHint(float dt);
    void delayedRtt(float dt);
    void delayedBlur(float dt);
    void delayedBlur2(float dt);
    void delayedLoad(float dt);
    void delayedPrevious(float dt);
    void delayedNext(float dt);
    void delayedHint(float dt);
    void updateTime(float dt);
    
    RenderTexture *rtt,*blur1,*blur2;
    SpriteBlur *blur_sprite;
    Sprite *animation_sprite,*blur_mesaj;
    
    
    LabelTTF *time_label,*moves_label[3],*completed_label,*space_label;
    int seconds;
    
    MenuItemSprite *next,*previous,*reset,*hints,*undo;
    void menuNextClick(cocos2d::Ref* pSender);
    void menuPreviousClick(cocos2d::Ref* pSender);
    void menuResetClick(cocos2d::Ref* pSender);
    void menuHintsClick(cocos2d::Ref* pSender);
    void menuUndoClick(cocos2d::Ref* pSender);
    
    MenuItemSprite *next_level,*try_again;
    
    void menuNextLevelClick(cocos2d::Ref* pSender);
    void menuTryAgainClick(cocos2d::Ref* pSender);
    
    
    Sprite *perfect,*not_perfect;
    Sprite *overlay,*finish_stars[3],*finish_stars2[3];
    
    cocos2d::Point perfect_pos,not_perfect_pos,next_level_pos,try_again_pos,finish_star_pos[3];
    
    ParticleSystemQuad *particle_stars[3];
    
    
    int nr_stars;
    

    int last_line,last_line_undo;
    long shifts;
    int last_dir;
    int lti;
    int ltj;
    int deleted[14];
    kmVec2 delete_point[14];
    int unit[14],unit_undo[14];
    int last_unit,last_unit_undo;
    int did_unit;
    

    
    Touch *t;
    EventCustom *e;
    void updateMoves();
    int getPosition(int,int,int,int);
    
    std::vector <Touch> hint_vector;
    
    std::pair <int,int> to_animate[2];
    float globes_scale;
    
    LabelTTF *hints_label;
    void updateHints();
    
};




#endif
