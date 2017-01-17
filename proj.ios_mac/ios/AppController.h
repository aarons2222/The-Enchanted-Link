#import <UIKit/UIKit.h>


@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate,UIAlertViewDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property (retain,readwrite) NSString * currentPlayerID;
@property (readwrite, getter=isGameCenterAuthenticationComplete) BOOL gameCenterAuthenticationComplete;
- (void)showGameCenter;
- (void)submitScore:(int64_t)score;
- (void)submitAchievement:(int)pack withPercent:(float)percent;
@end

