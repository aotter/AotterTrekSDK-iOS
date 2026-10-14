//
//  AotterTrekSDK.h
//  AotterTrekSDK
//
//  Umbrella header of the AotterTrekSDK module and declaration of its entry class (same name as the module).
//  Quoted imports: they resolve next to this file both in the framework's Headers/ and when the demo builds from source.
//

#import <UIKit/UIKit.h>

// TK core (the 4.2.x API, kept for compatibility)
#import "AotterTrek.h"
#import "TKTracker.h"
#import "TKAdNative.h"
#import "TKAdManager.h"
#import "TKEnumHeaders.h"
#import "TKAdSuprAd.h"
#import "TKAdError.h"
#import "TKNativeAdConstant.h"

// ATK API (5.0)
#import "ATKError.h"
#import "ATKUser.h"
#import "ATKAdRequest.h"
#import "ATKNativeAdData.h"
#import "ATKAdLoaderListener.h"
#import "ATKAdLoaderDelegate.h"
#import "ATKAdLoader.h"
#import "ATKMediaView.h"
#import "ATKNativeAdView.h"
#import "ATKBannerAdView.h"
#import "ATKTracker.h"

//! Project version number for AotterTrekSDK.
FOUNDATION_EXPORT double AotterTrekSDKVersionNumber;

//! Project version string for AotterTrekSDK.
FOUNDATION_EXPORT const unsigned char AotterTrekSDKVersionString[];

NS_ASSUME_NONNULL_BEGIN

/// How much the SDK logs. Off (None) in every build unless the host turns it on. Some output prints whatever the
/// level, for example the start-up banner and integration warnings.
typedef NS_ENUM(NSInteger, ATKLogLevel) {
    ATKLogLevelNone = 0,
    /// Warnings and errors.
    ATKLogLevelNormal,
    /// Everything, including per-request detail.
    ATKLogLevelDetail,
};


/// SDK entry point. Initialize once with the clientID from the Trek console; mediation adapters call
/// initSDKIfNeeded:error: and only fill in a missing initialization.
@interface AotterTrekSDK : NSObject

+ (AotterTrekSDK *)sharedInstance;

/// Initializes the SDK. Calling it with an empty clientID, or again with the same one, is a no-op; a different clientID
/// replaces the current one. Unless the call is a no-op, it clears the MyApp client id for the rest of the app session
/// (initSDKIfNeeded:error: too, when the SDK is not initialized yet and a non-empty clientID is given). An app that
/// also uses MyApp calls AotterTrek initTrekServiceWithClientId:secret:myAppClientId:myAppClientSecret: instead, before
/// anything else initializes the SDK (a mediation adapter does when the ad network starts or loads its first ad).
- (void)initSDK:(NSString *)clientID;
/// Initializes only when the SDK is not initialized yet. Returns NO with ATKErrorClientIdMismatch when the SDK is
/// already initialized with another clientID, or ATKErrorNotInitialized when nothing is initialized and clientID is
/// empty or the core does not accept it. When it initializes, it clears the MyApp client id like initSDK:.
- (BOOL)initSDKIfNeeded:(nullable NSString *)clientID error:(NSError * _Nullable * _Nullable)error NS_SWIFT_NAME(initSDKIfNeeded(clientID:));

@property (nonatomic, readonly, getter=isInitialized) BOOL initialized;
@property (nonatomic, readonly, nullable) NSString *currentClientID;

- (void)setCurrentUser:(ATKUser *)user;
- (void)removeCurrentUser;

+ (NSString *)sdkVersion;
+ (NSString *)sdkVersionCode;
+ (void)setLogLevel:(ATKLogLevel)level;

@end

NS_ASSUME_NONNULL_END
