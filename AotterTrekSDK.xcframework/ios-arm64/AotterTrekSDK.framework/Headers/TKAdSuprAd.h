//
//  ATSuprAd.h
//  AotterService
//
//  Copyright © 2019 Aotter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "TKAdError.h"

@class TKAdSuprAd;
@class TKMyAppAdSuprAd;
@protocol TKAdSuprAdDelegate <NSObject>



/**
 * While the SuprAd finishes fetching the ad, it will return prefered ad size.
 * Developer could set adContainer and presentingViewController in this callback.
 *
 * @param  size    preferred media view size for this ad.
 * @param  adData  ad data
 */
@optional
-(void)TKAdSuprAd:(TKAdSuprAd *)suprAd didReceivedAdWithAdData:(NSDictionary *)adData preferedMediaViewSize:(CGSize)size;

/**
 * While the SuprAd finishes fetching the ad, it will return prefered ad size.
 * Developer could set adContainer and presentingViewController in this callback.
 *
 * @param  size        preferred media view size for this ad.
 * @param  adData      ad data
 * @param  isVideoAd   determine this suprAd is video type or not
 */
@optional
-(void)TKAdSuprAd:(TKAdSuprAd *)suprAd didReceivedAdWithAdData:(NSDictionary *)adData preferedMediaViewSize:(CGSize)size isVideoAd:(BOOL)isVideoAd;

/**
 * Tell when the SuprAd finishes loading.
 */
@optional
-(void)TKAdSuprAdLoaded:(TKAdSuprAd *)suprAd;

/**
 * Tell when the SuprAd finishes loading and the ad is ready to display on screen.
 */
@optional
-(void)TKAdSuprAdCompleted:(TKAdSuprAd *)suprAd;



/**
 * Tell when the SuprAd get error when fetching/loading.
 * check error.message for more details.
 */
@optional
-(void)TKAdSuprAd:(TKAdSuprAd *)suprAd adError:(TKAdError *)error;


/**
 only avaiable for MyApp ads.
 implement this function will prevent default click event(open url)
 */
@optional
-(void)TKMyAppAdSuprAdOnClicked:(TKMyAppAdSuprAd *)ad;


/**
 * Called for video ads as well (on the VAST impression).
 */
@optional
-(void)TKAdSuprAdWillLogImpression:(TKAdSuprAd *)ad;

/**
 * Called for clicks on the ad, video ads included, though not every recorded click arrives here. Not called for a
 * click taken over by TKAdSuprAdCustomClickHandler:urlString:; a click the video player handles arrives as
 * TKAdSuprAdVideoClicked:urlString: instead. Clicks inside a WebView creative's pop-up are not reported here.
 */
@optional
-(void)TKAdSuprAdWillLogClick:(TKAdSuprAd *)ad;


/**
 * callback when the SuprAd video finishes playing
 */
@optional
-(void)TKAdSuprAdVideoCompleted:(TKAdSuprAd *)suprAd;

/**
 * callback when the SuprAd video clicked
 */
@optional
-(void)TKAdSuprAdVideoClicked:(TKAdSuprAd *)suprAd urlString:(NSString *)urlString;

/**
 * For ads that allow custom clicks, takes over a tap on the ad view and a WebView creative asking to open an external
 * browser (urlString is then the ad's landing URL): the SDK does not open it itself, and the click is not reported
 * through TKAdSuprAdWillLogClick:. An HTML5 creative's links, the video player's clicks and WebView pop-ups are not
 * taken over.
 */
@optional
-(void)TKAdSuprAdCustomClickHandler:(TKAdSuprAd *)suprAd urlString:(NSString *)urlString;

@end


@interface TKAdSuprAd : NSObject
@property (weak) id<TKAdSuprAdDelegate> delegate;
@property (readonly) NSDictionary *adData;
@property (readonly) BOOL adLoaded;
@property (readonly) CGSize preferedContainerSize;
@property (nonatomic,strong) NSDictionary *requestMeta;

/**
 * Initialize the SuperAd with ad place.
 * @param place ad place.
 */
-(instancetype)initWithPlace:(NSString *)place;

/**
 * Initialize the SuperAd with ad place and ad category
 @param place    ad place.
 @param category ad cateogry
 */
-(instancetype)initWithPlace:(NSString *)place category:(NSString *)category;

/**
 * optional param for your content page's title
 */
-(void)setAdContentTitle:(NSString *)contentTitle;

/**
 * optional param for your content page's display url
 */
-(void)setAdContentUrl:(NSString *)contentUrl;


/**
 * Set the ad category manually.
 * @param category ad category
 */
-(void)setAdCategory:(NSString *)category;


/**
 * Register the ad view: the tap target (for WebView and HTML5 creatives also the view Open Measurement measures).
 * Trek impressions are measured on the TKMediaView.
 * adView and TKMediaView could be the same.
 */
-(void)registerAdView:(UIView *)adView;



/**
 * Register ad media view for SuprAd view displaying.
 * adView and TKMediaView could be the same.
 */
-(void)registerTKMediaView:(UIView *)tkMediaView;


/**
 * Register the viewcontroller for view showing.
 * 
 * WARNING: required for video ads: without it, loadAd fails with error 1001, reported through TKAdSuprAd:adError:.
 * Other creatives fall back to the key window's top view controller when they need one.
 * @param  viewController  the viewControlelr for presenting additonal views
 */
-(void)registerPresentingViewController:(UIViewController *)viewController;


/**
 * Fetch suprAd.
 * The ad place is required.
 * TKAdSuprAd:didReceivedAdWithAdData:preferedMediaViewSize:isVideoAd: (or the variant without isVideoAd) or
 * TKAdSuprAd:adError: will get called after fetching the ad; TKAdSuprAd:adError: can still follow a received ad when
 * loading it then fails.
 */
-(void)fetchAd;


/*
  no doc.
 */
-(void)loadAd;



/**
 * Fetch suprAd.
 * The ad place is required.
 * @param callback called with adData (ad data), preferedAdSize (preferred media view size for this ad), adError (check
 *        error.message for more details), isVideoAd, and loadAd: call loadAd once your ad settings are done; if you do
 *        not, the ad will not load.
 */
-(void)fetchAdWithCallback:(void(^)(NSDictionary *adData, CGSize preferedAdSize, TKAdError *adError, BOOL isVideoAd, void(^loadAd)(void)))callback;


/**
 * Add a friendly obstruction view for OMSDK
 * The view is required.
 * call this method after fetchAd
 */
- (void)addFriendlyObstructionWithView:(UIView *)view;


//check the ad is still valid
-(BOOL)isExpired;

//check this supr ad is video type
-(BOOL)isVideoAd;


/**
 * Release the ad and its related views, container, and players.
 */
-(void)destroy;


/**
    if you render the ad in your scrollView/TableView/CollectionView or anything may have vertical scroll behavior, please call this fuction when the scrollView scrolled.
    such as -(void)scrollViewDidScroll:(UIScrollView *)scrollView;
 */
-(void)notifyAdScrolled;

@end


@interface TKMyAppAdSuprAd : TKAdSuprAd

@end
