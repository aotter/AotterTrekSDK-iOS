//
//  ATKMediaView.h
//  AotterTrekSDK
//
//  The one media container for every ad type: image ads render into its imageView, SuprAd / banner creatives
//  (web, HTML5, VAST) are hosted by the core inside this view. Size it yourself from
//  ATKNativeAdData.mediaContentAspectRatio; it has no intrinsic content size.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ATKMediaView;

@protocol ATKMediaViewDelegate <NSObject>
@optional
/// A VAST creative was clicked; `url` is the ad's landing URL (url_original; empty when the response has none). The
/// player itself asks the system to open the VAST ClickThrough, when the VAST has one.
- (void)mediaView:(ATKMediaView *)mediaView didClickVideoWithURL:(NSString *)url;
/// A VAST creative finished playing.
- (void)mediaViewDidEndVideo:(ATKMediaView *)mediaView;
@end

@interface ATKMediaView : UIView

@property (nonatomic, weak, nullable) id<ATKMediaViewDelegate> delegate;
/// Shows the main image of an image ad (scaleAspectFit by default); hidden while a SuprAd is bound.
@property (nonatomic, strong, readonly) UIImageView *imageView;

@end

NS_ASSUME_NONNULL_END
