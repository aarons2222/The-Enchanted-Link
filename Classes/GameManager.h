//
//  GameManager.h
//  The Enchanted Links
//
//  Aaron Strickland
//
//

#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Menu1.h"
#include "Menu2.h"
#include "Menu3.h"
#include "About.h"
#include "Settings.h"
#include "Store.h"
#include "Level.h"

using namespace cocos2d;


GLfloat LinearInterpolation(GLclampf t, GLfloat start, GLfloat end);

char* itoa(int i);

class GameManager{
public:
    GameManager();
    void setScene(int);
    void setPack(int);
    void setLevel(int);
    Menu1* menu1;
    Menu2* menu2;
    Menu3* menu3;
    Level* level;
    About* about;
    Store* store;
    Settings *settings;
    int scene;
    int pack;
    int level_nr;
    static GameManager* getInstance();
    Scene* scene1,*scene2,*scene5;
    
    FILE *save;
    void readSave(int pack,int *current_pack);
    void updateSave(int pack,int *current_pack);
    void updatePreferences();
    
    
    std::string font_name;
    Color4B background_color;
    Color3B color1,color2,grid_color;
    void updateColors();
    
    void playSound(int sound);
    
    
    
    float font_scale;
    void showAchievements();
    void buyItem(int item);
    void getPrices();
    
    std::string preturi[10];
    
    void setDefaults();
    void getUnlockedPacks(int packs[]);
    
    int getHints();
    void setHints(int hints);
    
    void unlockLevels();
    
    void resetProgress();
    void restorePurchase();
    
    void showLevelLockedAlert();
    void showPackLockedAlert();
    void showHintsAlert();
    void sendFeedback();
    void reviewApp();
    
    void setLoading(bool);
    
    std::string sounds[4];
    
    int width,height;
    
private:
    Director* director;
};



#endif