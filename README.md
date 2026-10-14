# AotterTrek iOS SDK

Renders AotterTrek native, banner and SuprAd creatives (image, HTML5, WebView and VAST video) and measures them with
the IAB Open Measurement SDK. The shipped module is pure Objective-C and imports cleanly from both Objective-C and
Swift.

- **Requirements**: iOS 15 or later. `AotterTrekSDK.xcframework` is built with Xcode 27.0 (27A266a).
- **Mediation**: AdMob, Google Ad Manager and AppLovin MAX adapters live in a separate product,
  [AotterTrekMediation](https://github.com/aotter/AotterTrekMediation-iOS).

## Install

### Swift Package Manager

Add `https://github.com/aotter/AotterTrekSDK-iOS` and depend on the `AotterTrekSDK` library. It carries both binaries:
the SDK and the Open Measurement framework.

### CocoaPods

```ruby
pod 'AotterTrekSDK'
```

### Manual

Drag `AotterTrekSDK.xcframework` and `OMSDK_Aotternet.xcframework` into the app target and embed them. A **pure
Objective-C app** must also give the linker the Swift runtime, because the SDK's VAST engine is written in Swift: add
one empty `.swift` file to the app target, or append `$(TOOLCHAIN_DIR)/usr/lib/swift/$(PLATFORM_NAME)` to
`LIBRARY_SEARCH_PATHS`. The SPM and CocoaPods routes do this for you.

## Start the SDK

```swift
import AotterTrekSDK

AotterTrekSDK.sharedInstance().initSDK("<your client id>")
```

```objc
@import AotterTrekSDK;

[[AotterTrekSDK sharedInstance] initSDK:@"<your client id>"];
```

Calling `initSDK:` again with the same client id does nothing; with a different one the new id wins and takes effect
for later requests. A host that only wants to fill in what is missing (a mediation adapter, for instance) uses
`initSDKIfNeeded:error:` (`initSDKIfNeeded(clientID:)` in Swift), which fails with `ATKErrorClientIdMismatch` /
`ATKError.clientIdMismatch` (1201) when the app has already initialised the SDK with a different id. An app that also
uses MyApp initialises with the four-argument `initTrekService…` instead; see Migrating from 4.x.

Optional: `setCurrentUser:` attaches user signals, `setLogLevel:` controls logging.

## Load and show a native ad

```swift
final class AdCell: UITableViewCell, ATKAdLoaderDelegate {
    private let adView = ATKNativeAdView()
    private let mediaView = ATKMediaView()
    private let titleLabel = UILabel()
    private let bodyLabel = UILabel()
    private let callToAction = UIButton(type: .system)
    private var loader: ATKAdLoader?

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        contentView.addSubview(adView)                       // the SDK measures the ad view (a SuprAd: also its media view)
        [mediaView, titleLabel, bodyLabel, callToAction].forEach(adView.addSubview)
        adView.mediaView = mediaView                         // one media view for every creative type
        adView.callToActionButton = callToAction
        // Lay adView out to fill contentView, and constrain mediaView's height with the ad's aspect ratio
        // once an ad arrives (ATKMediaView has no intrinsic size).
    }

    required init?(coder: NSCoder) { fatalError() }

    func load(place: String, from viewController: UIViewController) {
        adView.destroy()                                     // a reload: finish the ad this cell was showing
        adView.rootViewController = viewController
        let loader = ATKAdLoader(placeID: place, listener: nil)
        loader.delegate = self
        self.loader = loader
        loader.load(ATKAdRequest(category: "", contentURL: "", contentTitle: "", meta: [:]))
    }

    func adLoader(_ adLoader: ATKAdLoader, didReceive ad: ATKNativeAdData) {
        guard adLoader === loader else { return ad.destroy() }   // a reload replaced this loader
        titleLabel.text = ad.title
        bodyLabel.text = ad.text
        callToAction.setTitle(ad.callToAction, for: .normal)
        adView.nativeAd = ad                                 // binds the ad and starts measuring it
    }

    func adLoader(_ adLoader: ATKAdLoader, didFailToReceiveAdWithError error: Error) { }
}
```

`ATKAdLoaderListener` offers the same events as closures if you prefer them over the delegate. Its signatures mirror
the Android SDK's `TrekAdListener` (the media-loaded closure has no counterpart there), so they carry less: a failure
arrives as a `String` rather than an `NSError`, and the impression and click closures take no argument. Use the
delegate when you show several ads at once and have to tell their events apart. `loadAds(_:count:)` requests one to
five ads in parallel and reports them once, in the order they were requested.

`adLoader(_:didLoadMediaFor:)` tells you when the ad's media has finished loading in the media view it is bound to: an
image ad's picture is shown, or a SuprAd creative is ready (an HTML5 creative once its page has loaded, even if some
of its resources did not). `ad.isMediaLoaded` gives the same answer at any time, and the listener has it as
`adLoaderDidAdMediaLoadedCompletion`. An image ad reports it once per bind and goes back to unloaded when it is
unbound or bound again; a SuprAd loads its media once and reports it once. Every ad goes back to unloaded when it is
destroyed. No media-loaded event comes for an image ad bound without a media view or without a picture to show, or
when the media fails to load, and a failed download or playback is not reported as an error either. A SuprAd whose
creative type the SDK does not recognise reports error 1101 when it is bound to a media view. Impressions never wait
for the media-loaded event.

For an image ad, setting `ATKNativeAdView.nativeAd` to the same ad again, or giving it a new `mediaView` or
`callToActionButton`, is a new bind: the picture loads and is reported again. So do not set them again from this
callback (for example by reloading a cell that sets them); resize with constraints instead.

## Show a banner

```swift
let banner = ATKBannerAdView()
banner.placeID = "<place uid>"
banner.rootViewController = self
banner.delegate = self
banner.load(nil)
```

The server decides the size. `bannerAdView(_:willChangeAdSizeTo:)` arrives before the ad is shown, and `adSize` plus
`intrinsicContentSize` follow it, so a table or stack view can lay the slot out.

`bannerAdViewDidReceiveAd(_:)` means the ad arrived and is bound, and its creative starts loading. A video creative
also needs a view controller: with neither `rootViewController` nor a key window to fall back on, it waits until you
set `rootViewController`.
`bannerAdViewDidLoadMedia(_:)` (or `adBannerDidAdMediaLoadedCompletion`, or `isMediaLoaded`) tells you when it has
loaded.

## Report what the user does

`ATKTracker` reports to the endpoint the Android SDK's `Tracker` uses, in its format; the user carries `hashEmail` /
`hashPhone`, as the SDK's ad requests do. Create one per report:

```swift
let post = ATKTrackerEntity(type: .post, entityId: "<post id>", title: "<post title>", url: "<post url>")
let tracker = ATKTracker().entity(post).actionType(.readPost).start()
// When the user leaves the post:
tracker.send()
```

The entity, its url and the action type are required. `start` lets the SDK measure the time spent, in seconds;
`timeSpan(_:)` sets it yourself. Without `user(_:)` the report carries the current user from `setCurrentUser:`. `send`
reports once and does nothing but log a line when the SDK is not initialised or the report is incomplete or not valid
JSON; the result is not reported back. Unlike Android it never throws, and the user is optional. `ATKTracker.h` lists
the action and entity types the tracker currently drops.

`TKTracker` keeps working as in 4.x and reports to its own endpoint in its own format, so report each action through
one of them.

## Rules the integration has to follow

These are the parts that bite if you guess:

- **`ATKNativeAdView.nativeAd` decides the ad's fate.** Setting the same ad again rebinds it. Setting a different ad
  destroys the previous one, which then stops measuring and can never be shown again. Setting `nil` only unbinds, but
  the ad you removed is destroyed later if you bind a different one to that view or call `destroy` on the view. An ad
  already bound to another view by then is left alone in both cases.
- **`unbind` is not a pause.** It does not stop a SuprAd that has started playing, and it does not stop an image ad's
  impression timer.
- **`ATKMediaView` has no intrinsic size.** Constrain its height with `mediaContentAspectRatio`. Note that `imgMain`
  can be an empty string while `preferredMediaViewSize` is non-zero, because the SDK falls back to another image.
- **Destroy what you hold.** Call `destroy` on an `ATKNativeAdData` before leaving the page. When you reload a native
  slot, destroy the old ad first and ignore callbacks from the loader you replaced. A banner reloads itself: call
  `load(_:)` again and it keeps the ad on screen until the new one arrives, so do not destroy it first.
- **One ad, one view.** Binding the same ad into two containers is not supported; the second bind moves it, and the
  view it left stops deciding its fate. A SuprAd that has already started does not survive the move: to show it
  somewhere else, destroy it and load a new one.
- **Keep the media view inside the ad's container.** Image ads measure impressions and taps on the container, SuprAd
  impressions on the media view. When an ad's media loads in a media view that is not inside the container it was
  bound with (for `ATKNativeAdView`, the ad view itself), the SDK logs a warning, whatever the log level.
- **Call the view and ad APIs on the main thread.** Binding, unbinding and destroying touch UIKit and the ad's own
  state; callbacks come back on the main thread too.

## Errors

Errors arrive in the `net.aotter.trek` domain (`ATKErrorDomain`) with these codes:

| Code | Meaning |
|---|---|
| 800 | No ad for this request (a paused or archived placement included) |
| 801 | Request refused because caching is enabled (defined, but not reported by this release) |
| 900 | Unknown or server-side failure |
| 901 | The SDK is not initialised |
| 902 | The MyApp service is not initialised (only the MyApp `TK` classes report this; the `ATK` classes do not) |
| 1000 | The ad place is empty |
| 1001 | A media view (and, for a video creative, a view controller) is required; the `ATK` classes wait for them instead of reporting this |
| 1101 | Unknown player type |
| 1102 | Video playback failure (defined, but not reported by this release) |
| 1201 | The requested client id differs from the initialised one |
| 1202 | The server sent an ad type this SDK cannot render |

## Migrating from 4.x

The `TK` classes are unchanged from 4.2.2 and still public, apart from the fixes listed in the changelog, so the code
that calls the SDK does not change. What does change is everything around the name; the pod (or xcframework), import
and header rows have to be edited before the project compiles again:

| 4.x | 5.0 |
|---|---|
| `pod 'AotterTrek-iOS-SDK'` | `pod 'AotterTrekSDK'` |
| No Swift Package Manager support | `https://github.com/aotter/AotterTrekSDK-iOS` |
| `AotterTrek-iOS-SDK.xcframework` | `AotterTrekSDK.xcframework` |
| `@import AotterTrek_iOS_SDK;` / `import AotterTrek_iOS_SDK` | `@import AotterTrekSDK;` / `import AotterTrekSDK` |
| `#import <AotterTrek-iOS-SDK/AotterTrek-iOS-SDK.h>` | `#import <AotterTrekSDK/AotterTrekSDK.h>` |
| `pod 'TrekSDKAdMobMediationObjc'` (a separate 1.x pod) | `pod 'AotterTrekMediation'` -- remove the old one, it defines the same adapter classes |
| iOS 12 (4.1.x and earlier; 4.2.x already required 15) | iOS 15 |
| Mediation `extraAssets` keys | changed -- the mapping is in the AotterTrekMediation README |

A manual integration also replaces `OMSDK_Aotternet.xcframework` with the one in this release: the file name is the
same, but the SDK loads the Open Measurement JavaScript that matches the bundled version.

New work should use the `ATK` classes. They map to the old ones like this:

| 4.x | 5.0 |
|---|---|
| `[AotterTrek sharedAPI] initTrekService…` | `[[AotterTrekSDK sharedInstance] initSDK:]` (an app that also uses MyApp keeps the four-argument `initTrekService…` and calls it before anything else initialises the SDK, a mediation adapter included: `initSDK:` and `initSDKIfNeeded:error:` clear the MyApp client id, as their header documentation details) |
| `TKAdNative` / `TKAdSuprAd`, chosen by the caller | `ATKAdLoader`, which picks the right one from the response |
| `NSDictionary` ad data | `ATKNativeAdData` with typed properties |
| A different view per creative type | `ATKNativeAdView` plus one `ATKMediaView` |
| `TKAdSuprAd` used as a banner | `ATKBannerAdView` |
| `[TKTracker sharedAPI]` with one shared pool of items | `ATKTracker`, one per report, in Android's format |

## Support

sdk@aotter.net
