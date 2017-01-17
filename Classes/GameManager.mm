

char* itoa(int i){
    
    /* Room for INT_DIGITS digits, - and '\0' */
    static char buf[21];
    char *p = buf + 20;	/* points to terminating '\0' */
    if (i >= 0) {
        do {
            *--p = '0' + (i % 10);
            i /= 10;
        } while (i != 0);
        return p;
    }
    else {			/* i < 0 */
        do {
            *--p = '0' - (i % 10);
            i /= 10;
        } while (i != 0);
        *--p = '-';
    }
    return p;
}
/*     end copyright     */



#include "GameManager.h"
#include "config.h"

#import "AppController.h"
#import "RootViewController.h"
#import "FlowIAP.H"
#import "Keychain.h"




#define BoundsCheck(t, start, end) \
if (t <= 0.f) return start;        \
else if (t >= 1.f) return end;

GLfloat LinearInterpolation(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return t * end + (1.f - t) * start;
}


//sets background and item colors for normal and night mode
void GameManager::updateColors(){
    
    if(!UserDefault::getInstance()->getIntegerForKey("night")){
        //normal mode
        background_color=Color4B(226, 203, 156,255); //background color
        color2=Color3B(74, 38, 24); //items color
    }
    else{
        //night mode
        background_color=Color4B(74, 38, 24,255); //background color
        color2=Color3B(226, 203, 156); //items color
    }
    
}


static GameManager * sharedGameManager;

GameManager::GameManager(){
    
    //get handy director instance
    director=Director::getInstance();
    
    //set singleton instance
    sharedGameManager=this;
    
    //first scene
    scene=1;
    
    //default scale for font
    font_scale=1;
    
    //current selected pack
    pack=0;
    
    //open or create save file in documents directory
    std::string file=FileUtils::getInstance()->getWritablePath();
    file+="flow.dat";
    
    int file_exists;
    save=fopen(file.c_str(),"r+");
    if (save==NULL)
        file_exists=0;
    else {
        file_exists=1;
        fclose(save);
    }
    
    if (file_exists)
        save=fopen(file.c_str(),"r+");
    else{
        //create default save where first level is unlocked in each pack
        save=fopen(file.c_str(),"w+");
        for(int j=0;j<10;j++){
            fprintf(save,"1 ");//first level
            for(int i=1;i<75;i++)
                fprintf(save,"0 ");//rest of them
            fprintf(save,"\n");
        }
        fflush(save);
        setDefaults();
        
    }
    
    
    //set font name
    
    font_name="Chunkfive";

    //preload main menu scene
    menu1=Menu1::create();
    
    //uncomment if you want to preload all scenes(it will take longer to start the game)
    //menu2=Menu2::create();
    //menu3=Menu3::create();
    //level=Level::create();
    //about=About::create();
    //settings=Settings::create();
    //store=Store::create();
    
    updateColors();
    

    
    
    //set sounds file names
    
    sounds[0]="sound1.caf"; //button pressed
    sounds[1]="sound2.caf"; //connection made on board
    sounds[2]="sound3.caf"; //connection broke on board
    sounds[3]="sound4.caf"; //level finished
    
    //preload sounds in audio engine
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[0].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[1].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[2].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[3].c_str() );
    
}


//set scene by number
void GameManager::setScene(int sceneNr){
    switch (sceneNr) {
        case 1:{//main menu
            auto scene=Menu1::createScene();
            auto nodes=scene->getChildren();
            menu1=(Menu1*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 2:{//about screen
            auto scene=About::createScene();
            auto nodes=scene->getChildren();
            about=(About*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 3:{//settings screen
            auto scene=Settings::createScene();
            auto nodes=scene->getChildren();
            settings=(Settings*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 4:{//store screen
            auto scene=Store::createScene();
            auto nodes=scene->getChildren();
            store=(Store*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 5:{//pack select screen
            auto scene=Menu2::createScene();
            auto nodes=scene->getChildren();
            menu2=(Menu2*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 6:{//level select screen
            auto scene=Menu3::createScene();
            auto nodes=scene->getChildren();
            menu3=(Menu3*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
        case 7:{//gameplay screen
            auto scene=Level::createScene();
            auto nodes=scene->getChildren();
            level=(Level*)nodes.at(1);
            director->replaceScene(CCTransitionFade::create(0.2,scene,Color3B(background_color)));
        }
            break;
    }
    this->scene=sceneNr;
}


//plays a sound
void GameManager::playSound(int sound){
    if(!UserDefault::getInstance()->getIntegerForKey("sound"))
        return;
    switch (sound) {
        case 1:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sounds[0].c_str());
            break;
        case 2:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sounds[1].c_str());
            break;
        case 3:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sounds[2].c_str());
            break;
        case 4:
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sounds[3].c_str());
            break;
    }
}

//shows a spinner and blocks the screen in store when the user buys something
void GameManager::setLoading(bool loading){
    if(scene==4)
        store->setLoading(loading);
    else
        settings->setLoading(loading);
}

//set pack for cache

void GameManager::setPack(int pack){
    this->pack=pack;
}

//set level for cache
void GameManager::setLevel(int lvl){
    level->_level=lvl;
}

//get object singleton
GameManager* GameManager::getInstance(){
    return sharedGameManager;
}

//read a pack stats from the save file
void GameManager::readSave(int pack,int *current_pack){
    int offset=pack *151;
    fseek(save, offset, SEEK_SET);
    for(int i=0;i<75;i++)
        fscanf(save,"%d ",&current_pack[i]);
}

//update a pack stats in save file

void GameManager::updateSave(int pack, int *current_pack){
    int offset=pack *151; //151 represents the number of characters that each pack takes in the save
    fseek(save, offset, SEEK_SET);
    for(int i=0;i<75;i++)
        fprintf(save,"%d ",current_pack[i]);
    
    fflush(save);
    
    //calculate current score
    int score[75];
    int64_t scor_final=0;
    float percent=0;
    for(int i=0;i<10;i++){
        readSave(i, score);
        for(int j=0;j<75;j++){
            if(i<10){
                if(score[j]==2)
                    scor_final+=i+1;
                if(score[j]==3)
                    scor_final+=2*(i+1);
                if(score[j]==4){
                    scor_final+=3*(i+1);
                    if(i==pack)
                        percent++;
                }
            }
            else{
                if(score[j]==2)
                    scor_final+=i-10+1;
                if(score[j]==3)
                    scor_final+=2*(i-10+1);
                if(score[j]==4){
                    scor_final+=3*(i-10+1);
                    if(i==pack)
                        percent++;
                }
                
            }
        }
        
    }
    percent/=75;//75 is the number of levels in each pack
    percent*=100;
    
    //send score for gamecenter leaderboard and current pack progress for achievements
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate submitScore:scor_final];
    [appDelegate submitAchievement:pack+1 withPercent:percent];
    
}

//update changes made in settings screen
void GameManager::updatePreferences(){
    
    updateColors();
    setScene(scene);
}

//will unlock levels if the user buys the option
void GameManager::unlockLevels(){
    int completed[75];
    for(int i=0;i<20;i++){
        readSave(i, completed);
        for(int j=0;j<75;j++)
            if(!completed[j])
                completed[j]=1;
        updateSave(i, completed);
    }
}




//opens AppStore do the app
void GameManager::reviewApp(){
    
    
    NSString *str=[NSString stringWithFormat:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%s",appID.c_str()];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

//opens Mail app to write an email
void GameManager::sendFeedback(){
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"mailto:%s",contactMail.c_str()]]];
}


//various alerts in game

void GameManager::showHintsAlert(){
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"You have 0 hints!"
                                                    message:@"Go to store to buy more."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
}

void GameManager::showPackLockedAlert(){
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Pack locked!"
                                                    message:@"Go to store to unlock."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
}

void GameManager::showLevelLockedAlert(){
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Level locked!"
                                                    message:@"Go to store to unlock."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
}


void GameManager::resetProgress(){
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Confirmation"
                                                    message:@"Are you sure you want to reset your progress?"
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Ok",nil];
    [alert show];
    [alert release];
}

//will restore purchases
void GameManager::restorePurchase(){
    [[FlowIAP sharedInstance] restoreCompletedTransactions];
}

//shows the gamecenter interface in game with leaderboard and achievements
void GameManager::showAchievements(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showGameCenter];
}

//get prices for the In App Purchases items from Apple

void GameManager::getPrices(){
    std::string * prices=[[FlowIAP sharedInstance] getPrices];
    if(!prices)
        return;
    for(int i=0;i<10;i++)
        preturi[i]=prices[i];
    
    delete [] prices;
}

//will initiate a buy
void GameManager::buyItem(int item){
    [[FlowIAP sharedInstance] buy:item];
}

//sets default settings first time the app is started

void GameManager::setDefaults(){
    
    UserDefault::getInstance()->setIntegerForKey("sound", 1); //sound is on, put 0 to set it off by default
    UserDefault::getInstance()->setIntegerForKey("night", 0); //night mode is on, put 0 to set it by for default
    UserDefault::getInstance()->setIntegerForKey("labels", 0); //labels are off, put 1 to set it by for default
    
    //sets default in app purchases stats(assumes the user did not buy anything)
    
    [Keychain setString:@"10" forKey:@"hints"]; //give the user 10 hints when he installs the app
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:unlock_all_packs.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:unlock_all_levels.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:pack_6.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:pack_7.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:pack_8.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:pack_9.c_str()]];
    [Keychain setString:@"0" forKey:[NSString stringWithUTF8String:pack_10.c_str()]];
    
}

//returns an array of unlocked packs (6 to 10)

void GameManager::getUnlockedPacks(int *packs){

    for(int i=0;i<5;i++)
        packs[i]=0;
    
    if([[Keychain getStringForKey:[NSString stringWithUTF8String:unlock_all_packs.c_str()]] isEqualToString:@"1"])
        for(int i=0;i<5;i++)
            packs[i]=1;
    else{
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_6.c_str()]] isEqualToString:@"1"])
            packs[0]=1;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_7.c_str()]] isEqualToString:@"1"])
            packs[1]=1;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_8.c_str()]] isEqualToString:@"1"])
            packs[2]=1;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_9.c_str()]] isEqualToString:@"1"])
            packs[3]=1;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_10.c_str()]] isEqualToString:@"1"])
            packs[4]=1;
    }

}

//get the curent number of hints

int GameManager::getHints(){
    return [[Keychain getStringForKey:@"hints"] intValue ];
}

//update the number of hints if the user has consumed any

void GameManager::setHints(int hints){
    [Keychain setString:[NSString stringWithFormat:@"%d",hints] forKey:@"hints"];
}

