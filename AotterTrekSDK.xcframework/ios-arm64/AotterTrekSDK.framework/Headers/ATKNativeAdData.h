//
//  ATKNativeAdData.h
//  AotterTrekSDK
//
//  One loaded ad. Wraps the core ad object (TKAdNative or TKAdSuprAd, chosen by the response's adType) and is the
//  single registration entry for views and mediation adapters (bindToContainerView:…).
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ATKImageResource;

@interface ATKNativeAdData : NSObject

@property (nonatomic, strong, readonly) NSString *uuid;
@property (nonatomic, strong, readonly) NSString *title;
@property (nonatomic, strong, readonly) NSString *text;
@property (nonatomic, strong, readonly) NSString *sponsor;
@property (nonatomic, strong, readonly) NSString *advertiserName;
@property (nonatomic, strong, readonly) NSArray<ATKImageResource *> *imgs;
@property (nonatomic, strong, readonly) NSString *callToAction;
@property (nonatomic, assign, readonly) double billableOffset;
/// Image ads: the main picture's size (img_main, or the fallback picture when there is none); SuprAd / banner: the
/// size the server asked for. CGSizeZero when unknown.
@property (nonatomic, assign, readonly) CGSize preferredMediaViewSize;

/// Image URLs (empty string when the response has none).
@property (nonatomic, strong, readonly) NSString *imgMain;
@property (nonatomic, strong, readonly) NSString *imgIcon;
@property (nonatomic, strong, readonly) NSString *imgIconHd;
@property (nonatomic, strong, readonly) NSString *category;
/// width / height of preferredMediaViewSize, 0 when unknown.
@property (nonatomic, assign, readonly) CGFloat mediaContentAspectRatio;
@property (nonatomic, assign, readonly, getter=isExpired) BOOL expired;
/// YES for VAST-driven SuprAd creatives; always NO for image ads.
@property (nonatomic, assign, readonly, getter=isVideoAd) BOOL videoAd;
/// YES once the media in the bound media view finished loading (ATKAdLoaderDelegate adLoader:didLoadMediaForAd:). An
/// image ad goes back to NO when it is unbound or bound again, until its picture is shown again; a SuprAd's media loads
/// once and stays loaded. Every ad goes back to NO when it is destroyed.
@property (nonatomic, assign, readonly, getter=isMediaLoaded) BOOL mediaLoaded;

/// Presenting view controller for SuprAd creatives (video, in-app browsing); image ads do not use it. Falls back to
/// the key window's top view controller.
@property (nonatomic, weak, nullable) UIViewController *rootViewController;

/// Registers the views with the core. Call on the main thread. Binding again moves the ad to the new views;
/// a SuprAd starts loading its media once a mediaView is bound (video creatives also need a view controller).
/// Put mediaView inside containerView (or pass the same view for both): when the media loads in a media view outside
/// the container, the SDK logs a warning.
- (void)bindToContainerView:(UIView *)containerView
                  mediaView:(nullable UIView *)mediaView
           callToActionView:(nullable UIView *)callToActionView
         rootViewController:(nullable UIViewController *)rootViewController;
/// Removes the gestures, targets and media subviews this ad installed on the bound views (main thread). The core keeps
/// its own (weak) references to those views until the next bind or destroy: an image ad still measures impressions
/// against the old container, and a SuprAd whose media already started keeps playing. To show a *different* ad in the
/// same views, destroy this one first (ATKNativeAdView does that for you); to show this ad again, bind it again.
- (void)unbind;
/// Stops timers, measurement and playback and releases the media (main thread, like bind and unbind: it tears down
/// views). The ad cannot be shown afterwards.
- (void)destroy;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

@interface ATKImageResource : NSObject

@property (nonatomic, strong, readonly) NSString *label;
@property (nonatomic, strong, readonly) NSString *src;
@property (nonatomic, assign, readonly) int width;
@property (nonatomic, assign, readonly) int height;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
