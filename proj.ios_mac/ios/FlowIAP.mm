#import "FlowIAP.H"
#import "GameManager.h"
#import "config.h"

@implementation FlowIAP

@synthesize produse;
@synthesize identificatoriProduse;
@synthesize format;

+ (FlowIAP *)sharedInstance {
    static dispatch_once_t once;
    static FlowIAP * sharedInstance;
    dispatch_once(&once, ^{
        NSSet * productIdentifiers = [NSSet setWithObjects:
                                      [NSString stringWithUTF8String:hints_5_id.c_str()],
                                      [NSString stringWithUTF8String:hints_20_id.c_str()],
                                      [NSString stringWithUTF8String:hints_60_id.c_str()],
                                      [NSString stringWithUTF8String:unlock_all_packs.c_str()],
                                      [NSString stringWithUTF8String:unlock_all_levels.c_str()],
                                      [NSString stringWithUTF8String:pack_6.c_str()],
                                      [NSString stringWithUTF8String:pack_7.c_str()],
                                      [NSString stringWithUTF8String:pack_8.c_str()],
                                      [NSString stringWithUTF8String:pack_9.c_str()],
                                      [NSString stringWithUTF8String:pack_10.c_str()],
                                      nil];
        sharedInstance = [[self alloc] initWithProductIdentifiers:productIdentifiers];
        sharedInstance.identificatoriProduse=[NSArray arrayWithObjects:
                                              [NSString stringWithUTF8String:hints_5_id.c_str()],
                                              [NSString stringWithUTF8String:hints_20_id.c_str()],
                                              [NSString stringWithUTF8String:hints_60_id.c_str()],
                                              [NSString stringWithUTF8String:unlock_all_packs.c_str()],
                                              [NSString stringWithUTF8String:unlock_all_levels.c_str()],
                                              [NSString stringWithUTF8String:pack_6.c_str()],
                                              [NSString stringWithUTF8String:pack_7.c_str()],
                                              [NSString stringWithUTF8String:pack_8.c_str()],
                                              [NSString stringWithUTF8String:pack_9.c_str()],
                                              [NSString stringWithUTF8String:pack_10.c_str()],
                                              nil];
        
        [sharedInstance requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
            if (success) {
                sharedInstance.produse = products;
            }
        }];
        
        sharedInstance.format = [[NSNumberFormatter alloc] init];
        [sharedInstance.format setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [sharedInstance.format setNumberStyle:NSNumberFormatterCurrencyStyle];
        
    });
    
    
    return sharedInstance;
}


-(void) buy:(int)product{
    if(produse==nil)
        return;
    
    GameManager::getInstance()->setLoading(true);
    for(SKProduct* produs in produse)
        if([produs.productIdentifier isEqualToString:[identificatoriProduse objectAtIndex:product]]){
            [[FlowIAP sharedInstance] buyProduct:produs];
            break;
        }
}

-(std::string*)getPrices{
    if(produse==nil)
        return NULL;
    std::string *ret=new std::string[15];

    for(SKProduct* produs in produse)
        for(int j=0;j<15;j++)
            if([produs.productIdentifier isEqualToString:[identificatoriProduse objectAtIndex:j]]){
                [format setLocale:produs.priceLocale];
                ret[j]= [[format stringFromNumber:produs.price] UTF8String];
                break;
            }
    
    return ret;
}

@end
