#import "IAPHelper.h"
#include <iostream>
using namespace std;
@interface FlowIAP : IAPHelper

@property(nonatomic,retain)  NSArray *produse;
@property(nonatomic,retain)  NSArray *identificatoriProduse;
@property(nonatomic,retain)  NSNumberFormatter *format;

+ (FlowIAP *)sharedInstance;
-(void)buy:(int) product;
-(std::string*)getPrices;
@end
