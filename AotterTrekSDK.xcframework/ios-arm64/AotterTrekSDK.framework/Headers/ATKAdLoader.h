//
//  ATKAdLoader.h
//  AotterTrekSDK
//
//  Loads ads for one placement. Results arrive through the listener closures and / or the delegate, on the main thread.
//

#import <Foundation/Foundation.h>
#import "ATKAdRequest.h"
#import "ATKAdLoaderListener.h"
#import "ATKAdLoaderDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface ATKAdLoader : NSObject

@property (nonatomic, strong, readonly) NSString *placeID;
@property (nonatomic, strong, nullable) ATKAdLoaderListener *adListener;
@property (nonatomic, weak, nullable) id<ATKAdLoaderDelegate> delegate;

- (instancetype)initWithPlaceID:(NSString *)placeID listener:(nullable ATKAdLoaderListener *)listener;

/// Loads one ad. Fails with ATKErrorNotInitialized when AotterTrekSDK was not initialized.
- (void)loadAdRequest:(nullable ATKAdRequest *)request NS_SWIFT_NAME(load(_:));
/// Loads `count` ads (clamped to 1…5) in parallel and reports once: the loaded ads in request order when at least
/// one loaded, otherwise every error.
- (void)loadAdsRequest:(nullable ATKAdRequest *)request count:(NSInteger)count NS_SWIFT_NAME(loadAds(_:count:));

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
