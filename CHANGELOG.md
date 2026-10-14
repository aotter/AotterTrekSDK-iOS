# Changelog

## 5.0.0 (100)

First release under the name **AotterTrekSDK**. The pod, the module, the umbrella header and the framework file all
changed name, and a new Objective-C API (the `ATK` classes) sits beside the existing `TK` one.

### Added
- `AotterTrekSDK`, the entry class: `initSDK:`, `initSDKIfNeeded:error:`, `isInitialized`, `currentClientID`,
  `setCurrentUser:`, `removeCurrentUser`, `sdkVersion`, `sdkVersionCode`, `setLogLevel:`.
- `ATKAdLoader` with `ATKAdRequest`, `ATKAdLoaderListener` (closures) and `ATKAdLoaderDelegate` (delegate), including
  `loadAdsRequest:count:` for one to five ads in parallel.
- `ATKNativeAdData`: typed getters over the response, `bindToContainerView:mediaView:callToActionView:rootViewController:`,
  `unbind` and `destroy`.
- `ATKNativeAdView` and `ATKMediaView`: one media view for every creative the server can send (image, HTML5, WebView,
  VAST video and pop-up), so the integration no longer branches on the ad type.
- `ATKBannerAdView` with `ATKBannerAdDelegate`.
- A media-loaded event for when an ad's media has finished loading in its media view (an image ad's picture is
  shown, or a SuprAd creative is ready): `adLoader:didLoadMediaForAd:`, the `adLoaderDidAdMediaLoadedCompletion`
  closure and `ATKNativeAdData.mediaLoaded`, and for banners `bannerAdViewDidLoadMedia:`,
  `adBannerDidAdMediaLoadedCompletion` and `ATKBannerAdView.mediaLoaded`. Impressions never wait for it.
- A console warning when an ad's media loads in a media view that is not inside the container it was bound with.
- `ATKTracker` with `ATKTrackerEntity` and `ATKTrackerLocation`: one report per instance, set up by chaining, in the
  format and to the endpoint of the Android SDK's `Tracker`, with all ten of its action types; the user carries
  `hashEmail` / `hashPhone`, as the SDK's ad requests do. `start` measures the time spent in seconds. `TKTracker` is
  unchanged.
- `ATKError`: the `net.aotter.trek` error domain with typed codes, including 1201 (the requested client id does not
  match the initialised one) and 1202 (the server sent an ad type this SDK does not render).

### Changed
- The pod is now `AotterTrekSDK` (was `AotterTrek-iOS-SDK`), and so are the module, the umbrella header and the
  framework file. Swift Package Manager is supported from this release.
- The log prefix is `[AotterTrekSDK]`; 4.x used `[AotterTrek-iOS-SDK]`, and `[AotterTrek-SDK]` on the Open Measurement
  start-up lines.
- The shipped module is pure Objective-C: no Swift module interface is installed, which removes the Swift version
  coupling the previous artifact had.
- Mediation adapters moved to their own pod, `AotterTrekMediation`, and cover AdMob, Google Ad Manager and AppLovin MAX.
- `updateCurrentUserWithValue:forKey:` with `TKUserKeyGender` sends "M" / "F", the values the ad server's gender
  targeting and the other platforms use; it sent "MALE" / "FEMALE", which the ad server's "M" / "F" targeting does
  not match.
- Initialising Trek again with a different, non-empty client id now also changes the client id the Trek requests
  carry; they kept sending the one they started with. MyApp requests are unchanged.
- Open Measurement fixes made after 4.2.2: destroying a SuprAd stops its video and ends its Open Measurement session at
  once instead of at deallocation; a finished session is kept alive for 1.5 seconds; the play-to-end handling reaches
  OMSDK on the main thread; `skipped` is sent only for a video that started and did not complete; the video's mute
  button is registered as a friendly obstruction; and VAST loads are reported as skippable and auto-playing.
- The bundled Open Measurement SDK is 1.6.9 (4.2.2 shipped 1.5.4).
- `TKAdSuprAd` reports 800 ("no ad fill") instead of 900 ("Trek server error") when the ad server has no ad for a
  request, as the `ATK` classes do; for both, a placement the server does not know, any other server error and a
  network failure still report 900. `TKAdNative` reports 800 whenever a fetch brings back no ad, as before.

### Compatibility
- Compared with 4.2.2, the last 4.x release: every `TK` class the integration used is still public and works as
  before apart from the changes listed above, so an existing integration keeps working after the rename. New work should
  use the `ATK` classes. Upgrading from an earlier 4.x release brings the 4.2.0–4.2.2 changes as well.
- The minimum deployment target stays iOS 15 and the privacy manifest stays in the framework, both as in 4.2.x.
