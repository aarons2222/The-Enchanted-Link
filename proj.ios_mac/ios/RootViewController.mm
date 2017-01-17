/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "Keychain.h"
#import "GADBannerView.h"
#import "GADRequest.h"
#import "platform/ios/CCEAGLView-ios.h"
#import <iAd/iAd.h>
#import "config.h"

@implementation RootViewController

@synthesize adBanner = adBanner_;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    //screen name, this is for Google Analytics
    self.screenName = @"Main screen";
    
    //variable used to check if the user purchased any pack, if he did then the game will not display ads
    int ok=1;
    
    if([[Keychain getStringForKey:[NSString stringWithUTF8String:unlock_all_packs.c_str()]] isEqualToString:@"1"])//purchased all packs
        ok=0;
    else{//or one of them
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_6.c_str()]] isEqualToString:@"1"])
            ok=0;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_7.c_str()]] isEqualToString:@"1"])
            ok=0;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_8.c_str()]] isEqualToString:@"1"])
            ok=0;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_9.c_str()]] isEqualToString:@"1"])
            ok=0;
        if([[Keychain getStringForKey:[NSString stringWithUTF8String:pack_10.c_str()]] isEqualToString:@"1"])
            ok=0;
    }
    
    if(ok){
        //set up banner coordinates to display at the bottom
        CGPoint origin = CGPointMake(0.0,self.view.frame.size.height -kGADAdSizeSmartBannerPortrait.size.height);
        self.adBanner = [[[DFPBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerPortrait origin:origin] autorelease];
        
        self.adBanner.adUnitID = [NSString stringWithUTF8String:adUnitID.c_str()];
        self.adBanner.delegate = self;
        [self.adBanner setRootViewController:self];
        [self.view addSubview:self.adBanner];
        [self.adBanner loadRequest:[self request]];
    }
    
}




- (GADRequest *)request {
    GADRequest *request = [GADRequest request];
    
    // Make the request for a test ad. Put in an identifier for the simulator as well as any devices
    // you want to receive test ads.
    
    request.testDevices = @[
                            // TODO: Add your device/simulator test identifiers here. Your device identifier is printed to
                            // the console when the app is launched.
                            GAD_SIMULATOR_ID,@"inserttestdevicehere"
                            ];
    return request;
}

#pragma mark GADBannerViewDelegate implementation

// We've received an ad successfully.
- (void)adViewDidReceiveAd:(DFPBannerView *)adView {
    //NSLog(@"Received ad successfully");
    self.adBanner.hidden=NO;
    
    CGPoint center;
    //iAd banners are usually smaller on some devices so we recalculate their position to remain at bottom
    if(adView.mediatedAdView.bounds.size.height)
        center = CGPointMake(self.view.frame.size.width/2.0,self.view.frame.size.height -
                             (adView.bounds.size.height/2.0-abs(adView.bounds.size.height-adView.mediatedAdView.bounds.size.height)));
    else
        center=CGPointMake(self.view.frame.size.width/2.0,self.view.frame.size.height - adView.bounds.size.height/2.0);
    
    adView.center=center;
    
    
}
//important, this hides the banner if no ad is received
- (void)adView:(DFPBannerView *)view didFailToReceiveAdWithError:(GADRequestError *)error {
    //NSLog(@"Failed to receive ad with error: %@", [error localizedFailureReason]);
    self.adBanner.hidden=YES;
    //[self.adBanner loadRequest:[self request]];
}






// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    //return UIInterfaceOrientationIsLandscape( interfaceOrientation );
    return YES;
}

- (void)showGameCenter
{
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    cocos2d::Director::getInstance()->stopAnimation();//pause Open GL
    gameCenterController.gameCenterDelegate = self;
    [self presentViewController: gameCenterController animated: YES completion:nil];
    [gameCenterController release];
    
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
    cocos2d::Director::getInstance()->startAnimation();//resume Open GL
}




// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
