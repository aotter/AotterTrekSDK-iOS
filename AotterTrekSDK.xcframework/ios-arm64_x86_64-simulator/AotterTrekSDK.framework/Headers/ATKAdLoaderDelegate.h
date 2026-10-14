//
//  ATKAdLoaderDelegate.h
//  AotterTrekSDK
//
//  Protocol-style callbacks for ATKAdLoader (method names follow Android's TrekAdListener / CustomAdListener;
//  adLoader:didLoadMediaForAd: has no counterpart there).
//  All methods are optional and called on the main thread, after the matching ATKAdLoaderListener closure.
//

#import <Foundation/Foundation.h>

@class ATKAdLoader, ATKNativeAdData;

NS_ASSUME_NONNULL_BEGIN

@protocol ATKAdLoaderDelegate <NSObject>
@optional

- (void)adLoader:(ATKAdLoader *)adLoader didReceiveAd:(ATKNativeAdData *)ad NS_SWIFT_NAME(adLoader(_:didReceive:));
- (void)adLoader:(ATKAdLoader *)adLoader didFailToReceiveAdWithError:(NSError *)error NS_SWIFT_NAME(adLoader(_:didFailToReceiveAdWithError:));

/// loadAdsRequest:count: — the whole batch, in request order; only fired when at least one ad loaded.
- (void)adLoader:(ATKAdLoader *)adLoader didReceiveAds:(NSArray<ATKNativeAdData *> *)ads NS_SWIFT_NAME(adLoader(_:didReceiveAds:));
/// loadAdsRequest:count: — every request failed.
- (void)adLoader:(ATKAdLoader *)adLoader didFailToReceiveAdsWithErrors:(NSArray<NSError *> *)errors NS_SWIFT_NAME(adLoader(_:didFailToReceiveAdsWithErrors:));

/// The ad's media finished loading in the media view it is bound to: an image ad's picture is shown, or a SuprAd's
/// creative reported itself ready (an HTML5 creative once its page has loaded, even if some of its resources did not).
/// At most once per bind for an image ad and once per ad for a SuprAd; never for an image ad bound without a media view
/// or without a picture to show, and nothing is reported when the media fails to load. When the listener closure,
/// which fires first, destroys the ad, or unbinds or re-binds an image ad, the delegate is not told. Impressions never
/// wait for it.
- (void)adLoader:(ATKAdLoader *)adLoader didLoadMediaForAd:(ATKNativeAdData *)ad NS_SWIFT_NAME(adLoader(_:didLoadMediaFor:));
- (void)adLoader:(ATKAdLoader *)adLoader didRecordImpressionForAd:(ATKNativeAdData *)ad NS_SWIFT_NAME(adLoader(_:didRecordImpressionFor:));
- (void)adLoader:(ATKAdLoader *)adLoader didRecordClickForAd:(ATKNativeAdData *)ad NS_SWIFT_NAME(adLoader(_:didRecordClickFor:));
/// Implement to take over the click: the SDK stops opening the landing URL itself for ads that allow custom clicks.
/// Covers a tap on the ad, and a WebView creative asking to open an external browser (that arrives with the ad's
/// landing URL, not the page's); a VAST video, an HTML5 creative and a WebView creative's pop-up still open their links
/// themselves.
/// A click taken over arrives here instead of adLoader:didRecordClickForAd:; a tap on the ad is still counted by
/// the SDK. The closure and the delegate method take over together: providing either one stops both channels from
/// hearing that click as a recorded click.
- (void)adLoader:(ATKAdLoader *)adLoader didClickCustomURL:(NSString *)url forAd:(ATKNativeAdData *)ad NS_SWIFT_NAME(adLoader(_:didClickCustomURL:for:));

@end

NS_ASSUME_NONNULL_END
