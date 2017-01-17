//
//  GameManager.cpp
//  Farm
//
//  Created by Adrian on 09/03/14.
//
//

#include "GameManager.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "AppController.h"
    #import "RootViewController.h"
    #import "ConnectIAP.h"
    #import "Keychain.h"
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#include "platform\wp8-xaml\cpp\Direct3DInterop.h"

using namespace PhoneDirect3DXamlAppComponent;


float roundf(float x)
{
   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}

int ConnectCXBrideCode=0;


namespace PhoneDirect3DXamlAppComponent{

	void ConnectHelper::goToStore(){
		ConnectCXBrideCode=1;
	}

	void ConnectHelper::unlockLevels(){
		ConnectCXBrideCode=2;
	}

}


void GameManager::update(float dt){
    if(ConnectCXBrideCode==1){
        ConnectCXBrideCode=0;
        GameManager::getInstance()->setScene(4);
	}
	if(ConnectCXBrideCode==2 && GameManager::getInstance()!=NULL && GameManager::getInstance()->save!=NULL){
            ConnectCXBrideCode=0;
            unlockLevels();
    }
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "../cocos2d/cocos/platform/android/jni/JniHelper.h"
    int ConnectNDKBridgeCode=0;

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_ConnectNDKBridgeCpp(JNIEnv* env, jobject thiz,int code)
        {
            ConnectNDKBridgeCode=code;
        }
        
        JNIEXPORT jint JNICALL Java_org_cocos2dx_cpp_AppActivity_ConnectGetGogoasa(JNIEnv* env, jobject thiz)
        {
            jint ret=-1;
            
            
            if(GameManager::getInstance()==NULL || GameManager::getInstance()->save==NULL)
                return(ret);
            
            int result=0;
            int score[150];
            for(int i=0;i<20;i++){
                GameManager::getInstance()->readSave(i, score);
                for(int j=0;j<150;j++){
                    if(i<10){
                        if(score[j]==2)
                            result+=i+1;
                        if(score[j]==3)
                            result+=2*(i+1);
                        if(score[j]==4)
                            result+=3*(i+1);
                    }
                    else{
                        if(score[j]==2)
                            result+=i-10+1;
                        if(score[j]==3)
                            result+=2*(i-10+1);
                        if(score[j]==4)
                            result+=3*(i-10+1);
                        
                    }
                }
                
            }
            
            log("%d %d",result,(jint)result);
            ret=(jint)result;
            return(ret);
            
        }
        
    }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void GameManager::update(float dt){
        if(ConnectNDKBridgeCode==1){
            ConnectNDKBridgeCode=0;
            GameManager::getInstance()->setScene(4);
        }
        if(ConnectNDKBridgeCode==2){
            ConnectNDKBridgeCode=0;
            fseek(save, 0, SEEK_SET);
            for(int j=0;j<20;j++){
                fprintf(save,"1 ");//first level
                for(int i=1;i<150;i++)
                    fprintf(save,"0 ");//rest of them
                fprintf(save,"\n");
            }

        }
        if(ConnectNDKBridgeCode==3 && GameManager::getInstance()!=NULL && GameManager::getInstance()->save!=NULL){
            ConnectNDKBridgeCode=0;
            unlockLevels();
        }
    }
#endif



#define BoundsCheck(t, start, end) \
if (t <= 0.f) return start;        \
else if (t >= 1.f) return end;

GLfloat LinearInterpolation(GLclampf t, GLfloat start, GLfloat end)
{
    BoundsCheck(t, start, end);
    return t * end + (1.f - t) * start;
}



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

static GameManager * sharedGameManager;

GameManager::GameManager(){
    
    
    director=Director::getInstance();
    
    sharedGameManager=this;
    scene=1;
    font_scale=1;
    pack=0;
    
    std::string file=FileUtils::getInstance()->getWritablePath();
    file+="connect.dat";
    
    std::string old=FileUtils::getInstance()->getWritablePath();
    old+="save.save";
    
    FILE *oldf=fopen(old.c_str(),"r+");
    
    
    if(oldf!=NULL){
        
        int oldsave[20][150],i=0;
        
        for(i=0;i<20;i++){
            
            fseek(oldf, i*301, SEEK_SET);
            for(int j=0;j<150;j++){
                int read;
                fscanf(oldf,"%d ",&read);
                
                if(read==2)
                    read=4;
                if(read==1)
                    read=3;
                
                if(!j && !read)
                    read=1;
                oldsave[i][j]=read;
            }
        }
        
        save=fopen(file.c_str(),"w+");
        
        for(int i=0;i<20;i++){
            for(int j=0;j<150;j++)
                fprintf(save,"%d ",oldsave[i][j]);//rest of them
            fprintf(save,"\n");
        }
        fclose(save);
        
        
        fclose(oldf);
        remove(old.c_str());
        //setDefaults();
    }
    
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
        save=fopen(file.c_str(),"w+");
        for(int j=0;j<20;j++){
            fprintf(save,"1 ");//first level
            for(int i=1;i<150;i++)
                fprintf(save,"0 ");//rest of them
            fprintf(save,"\n");
			fflush(save);
        }
        setDefaults();
        
    }
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    font_name="CaviarDreams-Bold";
#else
    font_name="Caviar_Dreams_Bold.ttf";
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    
    int rezultat;
    
    NSString *oldhints=[Keychain getStringForKeyOld:@"ajutoare"];
    if(oldhints!=nil){
        rezultat=[oldhints intValue];
        rezultat+=[[Keychain getStringForKey:@"hints"] intValue];
        [Keychain setString:[NSString stringWithFormat:@"%d",rezultat] forKey:@"hints"];
    }
#endif
    
    
    menu1=Menu1::create();
    //menu2=Menu2::create();
    //menu3=Menu3::create();
    //level=Level::create();
    //about=About::create();
    //settings=Settings::create();
    //store=Store::create();
    
    
    
    updateColors();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    sounds[0]="sound1.caf";
    sounds[1]="sound2.caf";
    sounds[2]="sound3.caf";
    sounds[3]="sound4.caf";
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    sounds[0]="sound1.ogg";
    sounds[1]="sound2.ogg";
    sounds[2]="sound3.ogg";
    sounds[3]="sound4.ogg";
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
    sounds[0]="sound1.wav";
    sounds[1]="sound2.wav";
    sounds[2]="sound3.wav";
    sounds[3]="sound4.wav";
#endif

    /*

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[0].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[1].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[2].c_str() );
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( sounds[3].c_str() );
    
	*/
    
}

void GameManager::setScene(int scene){
    switch (scene) {
        case 1:{
            auto ns=Menu1::createScene();
            auto test=ns->getChildren();
            menu1=(Menu1*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 2:{
            auto ns=About::createScene();
            auto test=ns->getChildren();
            about=(About*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 3:{
            auto ns=Settings::createScene();
            auto test=ns->getChildren();
            settings=(Settings*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 4:{
            auto ns=Store::createScene();
            auto test=ns->getChildren();
            store=(Store*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 5:{
            auto ns=Menu2::createScene();
            auto test=ns->getChildren();
            menu2=(Menu2*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 6:{
            auto ns=Menu3::createScene();
            auto test=ns->getChildren();
            menu3=(Menu3*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
        case 7:{
            auto ns=Level::createScene();
            auto test=ns->getChildren();
            level=(Level*)test.at(0);
            director->replaceScene(CCTransitionFade::create(0.2,ns,Color3B(background_color)));
        }
            break;
    }
    this->scene=scene;
}

void GameManager::playSound(int sound){
    //if(!UserDefault::getInstance()->getIntegerForKey("sound"))
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


void GameManager::setLoading(bool loading){
    if(scene==4)
        store->setLoading(loading);
    else
        settings->setLoading(loading);
}


void GameManager::setPack(int pack){
    
    this->pack=pack;
}


void GameManager::setLevel(int lvl){
    level->_level=lvl;
}


GameManager* GameManager::getInstance(){
    return sharedGameManager;
}


void GameManager::readSave(int pack,int *current_pack){
    int offset=pack *301;
    fseek(save, offset, SEEK_SET);
    for(int i=0;i<150;i++)
        fscanf(save,"%d ",&current_pack[i]);
}

void GameManager::updateSave(int pack, int *current_pack){
    int offset=pack *301;
    fseek(save, offset, SEEK_SET);
    for(int i=0;i<150;i++)
        fprintf(save,"%d ",current_pack[i]);
    
    fflush(save);
    
    int score[150];
    int64_t scor_final=0;
    float percent=0;
    for(int i=0;i<20;i++){
        readSave(i, score);
        for(int j=0;j<150;j++){
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
    percent/=150;
    percent*=100;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate submitScore:scor_final];
    [appDelegate submitAchievement:pack+1 withPercent:percent];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    jint temp=scor_final;
    jint temp2=percent;
    jint temp3=pack;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","submitScore","(I)V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,temp);
        t.env->DeleteLocalRef(t.classID);
    }
    /*
    
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","updateAchievement","(II)V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,temp3,temp2);
        t.env->DeleteLocalRef(t.classID);
    }*/
    
    
#endif
    
}

void GameManager::updateColors(){
    
    if(!UserDefault::getInstance()->getIntegerForKey("night")){
        background_color=Color4B(232, 232, 232,255);
        color2=Color3B(166, 191, 178);
        grid_color=Color3B(232, 232, 232);
    }
    else{
        background_color=Color4B(20, 20, 20,255);
        color2=Color3B(166, 191, 178);
        grid_color=Color3B(20, 20, 20);
    }
    
}

void GameManager::updatePreferences(){
    
    updateColors();
    setScene(scene);
}

void GameManager::unlockLevels(){
    int completed[150];
    for(int i=0;i<20;i++){
        readSave(i, completed);
        for(int j=0;j<150;j++)
            if(!completed[j])
                completed[j]=1;
        updateSave(i, completed);
    }
}



//multiplatform



void GameManager::reviewApp(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str=@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=796065955";
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","reviewApp","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"reviewApp",(Platform::String^)"");

#endif

}

void GameManager::sendFeedback(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"mailto:connect.support@icloud.com"]];
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","sendFeedback","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"sendFeedback",(Platform::String^)"");

#endif
}


void GameManager::showHintsAlert(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"You have 0 hints!"
                                                    message:@"Go to store to buy more."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","showHintsAlert","()V")) {
             t.env->CallStaticVoidMethod(t.classID,t.methodID);
             t.env->DeleteLocalRef(t.classID);
        }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"showHintsAlert",(Platform::String^)"");

#endif

}

void GameManager::showPackLockedAlert(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Pack locked!"
                                                    message:@"Go to store to unlock."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
#endif
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","showPackLockedAlert","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"showPackLockedAlert",(Platform::String^)"");

#endif
}

void GameManager::showLevelLockedAlert(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Level locked!"
                                                    message:@"Go to store to unlock."
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Store",nil];
    [alert show];
    [alert release];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","showLevelLockedAlert","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"showLevelLockedAlert",(Platform::String^)"");

#endif
}


void GameManager::resetProgress(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Confirmation"
                                                    message:@"Are you sure you want to reset your progress?"
                                                   delegate:[[UIApplication sharedApplication] delegate]
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Ok",nil];
    [alert show];
    [alert release];
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","resetProgress","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

	Direct3DInterop::Exec((Platform::String^)"resetProgress",(Platform::String^)"");

#endif

}


void GameManager::restorePurchase(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ConnectIAP sharedInstance] restoreCompletedTransactions];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //CCStoreController::sharedStoreController()->restoreTransactions();
#endif
    
}


void GameManager::showAchievements(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showGameCenter];
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","showAchievementsAlert","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif

}

void GameManager::getPrices(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    std::string * prices=[[ConnectIAP sharedInstance] getPrices];
    if(!prices)
        return;
    for(int i=0;i<15;i++)
        preturi[i]=prices[i];
    
    delete [] prices;
#endif
}

void GameManager::buyItem(int item){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    [[ConnectIAP sharedInstance] buy:item];
#endif
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","buy","(I)V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,item);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
   

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	Direct3DInterop::Exec((Platform::String^)"buyItem",item.ToString());
#endif

}

void GameManager::setDefaults(){
    
    UserDefault::getInstance()->setIntegerForKey("sound", 1);
    UserDefault::getInstance()->setIntegerForKey("night", 1);
    UserDefault::getInstance()->setIntegerForKey("labels", 0);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    
    [Keychain setString:@"5" forKey:@"hints"];
    [Keychain setString:@"0" forKey:@"connect_packs_all"];
    [Keychain setString:@"0" forKey:@"connect_levels_all"];
    [Keychain setString:@"0" forKey:@"connect_pack11_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack12_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack13_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack14_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack15_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack16_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack17_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack18_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack19_premium"];
    [Keychain setString:@"0" forKey:@"connect_pack20_premium"];
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","setDefaults","()V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	Direct3DInterop::Exec((Platform::String^)"setDefaults",(Platform::String^)"");
#endif
}

void GameManager::getUnlockedPacks(int *packs){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    
    for(int i=0;i<10;i++)
        packs[i]=0;
    
    if([[Keychain getStringForKey:@"connect_packs_all"] isEqualToString:@"1"])
        for(int i=0;i<10;i++)
            packs[i]=1;
    else{
        if([[Keychain getStringForKey:@"connect_pack11_premium"] isEqualToString:@"1"])
            packs[0]=1;
        if([[Keychain getStringForKey:@"connect_pack12_premium"] isEqualToString:@"1"])
            packs[1]=1;
        if([[Keychain getStringForKey:@"connect_pack13_premium"] isEqualToString:@"1"])
            packs[2]=1;
        if([[Keychain getStringForKey:@"connect_pack14_premium"] isEqualToString:@"1"])
            packs[3]=1;
        if([[Keychain getStringForKey:@"connect_pack15_premium"] isEqualToString:@"1"])
            packs[4]=1;
        if([[Keychain getStringForKey:@"connect_pack16_premium"] isEqualToString:@"1"])
            packs[5]=1;
        if([[Keychain getStringForKey:@"connect_pack17_premium"] isEqualToString:@"1"])
            packs[6]=1;
        if([[Keychain getStringForKey:@"connect_pack18_premium"] isEqualToString:@"1"])
            packs[7]=1;
        if([[Keychain getStringForKey:@"connect_pack19_premium"] isEqualToString:@"1"])
            packs[8]=1;
        if([[Keychain getStringForKey:@"connect_pack20_premium"] isEqualToString:@"1"])
            packs[9]=1;
    }
    
#endif
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    jintArray ret;
    jint *ret_c;
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","getUnlockedPacks","()[I")) {
        
        ret=(jintArray)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        ret_c = t.env->GetIntArrayElements(ret, 0);
        
        for(int i=0;i<10;i++){
            packs[i]=ret_c[i];
        }
        t.env->DeleteLocalRef(t.classID);
        t.env->ReleaseIntArrayElements(ret, ret_c, 0);
        
    }
    
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	std::wstring result( Direct3DInterop::Exec((Platform::String^)"getUnlockedPacks",(Platform::String^)"")->Data());
	std::wistringstream wis(result);
	for(int i=0;i<10;i++)
		wis>>packs[i];
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameManager::checkUnlocked(){

        
    int ok;
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","unlockedLevels","()I")) {
        ok=t.env->CallStaticIntMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        if(ok)
            ConnectNDKBridgeCode=3;
    }

}
#endif

int GameManager::getHints(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    return [[Keychain getStringForKey:@"hints"] intValue ];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int hints;
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","getHints","()I")) {
        hints=t.env->CallStaticIntMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return hints;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
	std::wstring hints( Direct3DInterop::Exec((Platform::String^)"getHints",(Platform::String^)"")->Data());
	return std::stoi(hints);
#endif

}

void GameManager::setHints(int hints){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    [Keychain setString:[NSString stringWithFormat:@"%d",hints] forKey:@"hints"];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,"org/cocos2dx/cpp/AppActivity","setHints","(I)V")) {
        t.env->CallStaticVoidMethod(t.classID,t.methodID,hints);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
	int h=hints;
	Direct3DInterop::Exec((Platform::String^)"setHints",h.ToString());
#endif

}

