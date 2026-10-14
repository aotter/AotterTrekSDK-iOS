//
//  ATKBannerAdView.h
//  AotterTrekSDK
//
//  Self-contained banner: set placeID, call loadRequest:, and the view fetches, sizes and renders the creative
//  (SuprAd web / HTML5 / VAST, or an image ad). Size yourself from adSize / intrinsicContentSize or the
//  willChangeAdSizeTo callback. Use it from the main thread: loading and reloading bind and tear down views.
//

#import <UIKit/UIKit.h>
#import "ATKAdRequest.h"

NS_ASSUME_NONNULL_BEGIN

@class ATKBannerAdView;

typedef void (^ATKBannerAdDidAdFailedToLoadCompletion)(NSString *error);
typedef void (^ATKBannerAdDidAdLoadedCompletion)(ATKBannerAdView *bannerAdView);
typedef void (^ATKBannerAdDidAdMediaLoadedCompletion)(ATKBannerAdView *bannerAdView);
typedef void (^ATKBannerAdWillChangeAdSizeTo)(CGSize adSize);
typedef void (^ATKBannerAdDidAdClickedCompletion)(void);
typedef void (^ATKBannerAdDidAdImpressionCompletion)(void);

/// Closure-style callbacks (main thread). ATKBannerAdDelegate offers the same events as a protocol; closures fire first.
@interface ATKBannerAdListener : NSObject
@property (nonatomic, copy, nullable) ATKBannerAdDidAdFailedToLoadCompletion adBannerDidAdFailedToLoadCompletion;
@property (nonatomic, copy, nullable) ATKBannerAdDidAdLoadedCompletion adBannerDidAdLoadedCompletion;
/// Same event as ATKBannerAdDelegate bannerAdViewDidLoadMedia:.
@property (nonatomic, copy, nullable) ATKBannerAdDidAdMediaLoadedCompletion adBannerDidAdMediaLoadedCompletion;
@property (nonatomic, copy, nullable) ATKBannerAdWillChangeAdSizeTo adBannerWillChangeAdSizeTo;
@property (nonatomic, copy, nullable) ATKBannerAdDidAdClickedCompletion adBannerDidAdClickedCompletion;
@property (nonatomic, copy, nullable) ATKBannerAdDidAdImpressionCompletion adBannerDidAdImpressionCompletion;
@end

@protocol ATKBannerAdDelegate <NSObject>
@optional
/// Fired once the ad has been received and bound; its media starts loading then. A video creative also needs a view
/// controller: with neither rootViewController nor a key window to fall back on, it waits until rootViewController is
/// set. bannerAdViewDidLoadMedia: reports when it has loaded.
- (void)bannerAdViewDidReceiveAd:(ATKBannerAdView *)bannerAdView NS_SWIFT_NAME(bannerAdViewDidReceiveAd(_:));
/// The shown ad's media finished loading (the creative reported itself ready, an HTML5 creative once its page has
/// loaded; or an image ad's picture is shown). Once per shown ad; nothing is reported when an image ad has no picture
/// to show or the media fails to load. Impressions never wait for it.
- (void)bannerAdViewDidLoadMedia:(ATKBannerAdView *)bannerAdView NS_SWIFT_NAME(bannerAdViewDidLoadMedia(_:));
- (void)bannerAdView:(ATKBannerAdView *)bannerAdView didFailToReceiveAdWithError:(NSError *)error NS_SWIFT_NAME(bannerAdView(_:didFailToReceiveAdWithError:));
/// Fired before the ad is shown, with the size the creative was built for.
- (void)bannerAdView:(ATKBannerAdView *)bannerAdView willChangeAdSizeTo:(CGSize)adSize NS_SWIFT_NAME(bannerAdView(_:willChangeAdSizeTo:));
- (void)bannerAdViewDidRecordImpression:(ATKBannerAdView *)bannerAdView NS_SWIFT_NAME(bannerAdViewDidRecordImpression(_:));
- (void)bannerAdViewDidRecordClick:(ATKBannerAdView *)bannerAdView NS_SWIFT_NAME(bannerAdViewDidRecordClick(_:));
@end

@interface ATKBannerAdView : UIView

@property (nonatomic, copy, nullable) IBInspectable NSString *placeID;
@property (nonatomic, strong, nullable) ATKBannerAdListener *adListener;
@property (nonatomic, weak, nullable) id<ATKBannerAdDelegate> delegate;
/// Presenting view controller for SuprAd creatives (video, in-app browsing); falls back to the key window's top
/// controller.
@property (nonatomic, weak, nullable) UIViewController *rootViewController;
/// Size of the loaded creative (CGSizeZero before the first load). Also reported as intrinsicContentSize.
@property (nonatomic, assign, readonly) CGSize adSize;
/// YES once the shown ad's media finished loading; NO before the first load, after destroy, and for a new ad until its
/// media loads.
@property (nonatomic, assign, readonly, getter=isMediaLoaded) BOOL mediaLoaded;

/// Loads a new ad. The current ad stays visible until the new one arrives; a failed load leaves it in place.
- (void)loadRequest:(nullable ATKAdRequest *)request NS_SWIFT_NAME(load(_:));
/// Excludes a view drawn over the banner from viewability measurement.
- (void)addFriendlyObstruction:(UIView *)view;
/// Tell a SuprAd creative that its container scrolled (parallax creatives).
- (void)notifyAdScrolled;
/// Destroys the current ad; a load still in flight is discarded when it arrives.
- (void)destroy;

@end

NS_ASSUME_NONNULL_END
