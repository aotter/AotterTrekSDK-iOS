//
//  ATKAdLoaderListener.h
//  AotterTrekSDK
//
//  Closure-style callbacks for ATKAdLoader. Every closure runs on the main thread. ATKAdLoaderDelegate offers the
//  same events as a protocol; when both are set the closure fires first.
//

#import <Foundation/Foundation.h>
#import "ATKNativeAdData.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (^ATKAdLoaderDidAdFailedToLoadCompletion)(NSString *error);
typedef void (^ATKAdLoaderDidAdLoadedCompletion)(ATKNativeAdData *adData);
typedef void (^ATKAdLoaderDidAdMediaLoadedCompletion)(ATKNativeAdData *adData);
typedef void (^ATKAdLoaderDidAdClickedCompletion)(void);
typedef void (^ATKAdLoaderDidAdImpressionCompletion)(void);
typedef void (^ATKAdLoaderDidAdsLoadedCompletion)(NSArray<ATKNativeAdData *> *ads);
typedef void (^ATKAdLoaderDidAdsFailedToLoadCompletion)(NSArray<NSString *> *errors);
typedef void (^ATKAdLoaderDidCustomClickCompletion)(NSString *url);

@interface ATKAdLoaderListener : NSObject

@property (nonatomic, copy, nullable) ATKAdLoaderDidAdFailedToLoadCompletion adLoaderDidAdFailedToLoadCompletion;
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdLoadedCompletion adLoaderDidAdLoadedCompletion;
/// Same event as ATKAdLoaderDelegate adLoader:didLoadMediaForAd:.
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdMediaLoadedCompletion adLoaderDidAdMediaLoadedCompletion;
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdClickedCompletion adLoaderDidAdClickedCompletion;
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdImpressionCompletion adLoaderDidAdImpressionCompletion;

/// loadAdsRequest:count: results (whole batch, in request order).
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdsLoadedCompletion adLoaderDidAdsLoadedCompletion;
@property (nonatomic, copy, nullable) ATKAdLoaderDidAdsFailedToLoadCompletion adLoaderDidAdsFailedToLoadCompletion;
/// Set this to take over the click: the SDK stops opening the landing URL itself for ads that allow custom clicks.
/// Covers a tap on the ad, and a WebView creative asking to open an external browser (that arrives with the ad's
/// landing URL, not the page's); a VAST video, an HTML5 creative and a WebView creative's pop-up still open their links
/// themselves.
/// A click taken over arrives here instead of adLoaderDidAdClickedCompletion; a tap on the ad is still counted by
/// the SDK. The closure and the delegate method take over together: providing either one stops both channels from
/// hearing that click as a recorded click.
@property (nonatomic, copy, nullable) ATKAdLoaderDidCustomClickCompletion adLoaderDidCustomClickCompletion;

@end

NS_ASSUME_NONNULL_END
