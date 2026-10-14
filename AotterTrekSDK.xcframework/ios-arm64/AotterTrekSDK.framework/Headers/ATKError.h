//
//  ATKError.h
//  AotterTrekSDK
//
//  Error domain shared by the ATK API. Codes 800…1102 keep the TKAdError numbers;
//  12xx are produced by the ATK shell itself.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSErrorDomain const ATKErrorDomain;   // net.aotter.trek

typedef NS_ERROR_ENUM(ATKErrorDomain, ATKErrorCode) {
    ATKErrorNoAd = 800,
    /// Defined, but not reported by this release.
    ATKErrorFailedByCacheEnabled = 801,
    ATKErrorUnknown = 900,
    ATKErrorNotInitialized = 901,
    /// Reported by the MyApp TK classes only; the ATK classes do not report it.
    ATKErrorMyAppNotInitialized = 902,
    ATKErrorAdPlaceRequired = 1000,
    /// The ATK classes wait for the media view (and a video's view controller) instead of reporting this.
    ATKErrorMediaViewRequired = 1001,
    ATKErrorUnknownPlayerType = 1101,
    /// Defined, but not reported by this release.
    ATKErrorVideoPlayError = 1102,
    /// The SDK is already initialized with another clientID and the request asked for a different one (initSDKIfNeeded:error:).
    ATKErrorClientIdMismatch = 1201,
    /// The ad response carried a missing or unknown adType, so no ad class could be chosen.
    ATKErrorUnsupportedAdType = 1202,
};

/// userInfo: the clientID the SDK is initialized with (1201).
FOUNDATION_EXPORT NSErrorUserInfoKey const ATKErrorInitializedClientIdKey;
/// userInfo: the clientID the caller asked for (1201).
FOUNDATION_EXPORT NSErrorUserInfoKey const ATKErrorRequestedClientIdKey;
/// userInfo: the adType value found in the response, as a string (1202).
FOUNDATION_EXPORT NSErrorUserInfoKey const ATKErrorAdTypeKey;
/// userInfo: the original TKAdError when the error was produced by the TK core.
FOUNDATION_EXPORT NSErrorUserInfoKey const ATKErrorUnderlyingTKAdErrorKey;

NS_ASSUME_NONNULL_END
