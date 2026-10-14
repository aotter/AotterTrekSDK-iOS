//
//  ATKNativeAdView.h
//  AotterTrekSDK
//
//  Root view of a rendered ad. Assign the loaded ATKNativeAdData to `nativeAd` after wiring the outlets; the view
//  registers itself, the media view and the call-to-action button with the ad. Use it from the main thread: every
//  setter here binds or tears down views.
//

#import <UIKit/UIKit.h>
#import "ATKNativeAdData.h"

NS_ASSUME_NONNULL_BEGIN

@interface ATKNativeAdView : UIView

/// Setting the same ad again re-registers it; setting a different ad destroys the previous one first (it cannot be
/// shown again) unless that ad is bound somewhere else by then; setting nil only unbinds, but the ad you removed is
/// destroyed later if you set a different ad on this view or call this view's destroy (again unless it is bound
/// elsewhere).
@property (nonatomic, strong, nullable) ATKNativeAdData *nativeAd;
/// ATKMediaView (recommended), a UIImageView (image ads render into it directly) or any UIView (image ads add an
/// image view, SuprAd creatives use it as their container). Changing it after `nativeAd` is set re-registers the ad.
/// Keep it inside this view: when the media loads in a media view outside it, the SDK logs a warning.
@property (nonatomic, strong, nullable) UIView *mediaView;
@property (nonatomic, strong, nullable) UIButton *callToActionButton;
/// Presenting view controller for SuprAd creatives (video, in-app browsing); falls back to the key window's top
/// controller. Image ads do not use it.
@property (nonatomic, weak, nullable) UIViewController *rootViewController;

/// Excludes a view (e.g. a close button drawn over the ad) from viewability measurement.
- (void)addFriendlyObstruction:(UIView *)view;
/// Tell a SuprAd creative that its container scrolled (parallax creatives). No-op for image ads.
- (void)notifyAdScrolled;
/// Destroys the current ad and any ad left here by setting nil, except one bound to another view by then, and clears
/// `nativeAd`.
- (void)destroy;

@end

NS_ASSUME_NONNULL_END
