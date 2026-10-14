# Release podspec of the AotterTrek iOS SDK (repo aotter/AotterTrekSDK-iOS).
Pod::Spec.new do |s|
  s.name                  = 'AotterTrekSDK'
  s.version               = '5.0.0'
  s.summary               = 'AotterTrek iOS SDK: native, banner and SuprAd ads with OM SDK measurement.'
  s.description           = <<-DESC
    The AotterTrek iOS SDK renders AotterTrek native, banner and SuprAd creatives (image, HTML5, WebView and VAST
    video) and measures them with the IAB Open Measurement SDK. The shipped module is pure Objective-C and imports
    from both Objective-C and Swift. Mediation adapters for Google Mobile Ads and AppLovin MAX live in the separate
    AotterTrekMediation pod.
  DESC
  s.homepage              = 'https://github.com/aotter/AotterTrekSDK-iOS'
  s.license               = { :type => 'Commercial', :file => 'LICENSE' }
  s.authors               = { 'Aotter' => 'sdk@aotter.net' }
  s.source                = { :git => 'https://github.com/aotter/AotterTrekSDK-iOS.git', :tag => s.version.to_s }
  s.ios.deployment_target = '15.0'
  # Declared even though the shipped module is pure Objective-C: it makes CocoaPods treat the pod as Swift-bearing, so
  # a pure-ObjC app still links the Swift runtime the SDK's VAST engine needs.
  s.swift_version         = '5.0'
  s.vendored_frameworks   = 'AotterTrekSDK.xcframework', 'OMSDK_Aotternet.xcframework'
  s.frameworks            = 'SystemConfiguration', 'CoreMedia', 'WebKit', 'CoreTelephony', 'AdSupport', 'AVKit', 'AVFoundation', 'Foundation', 'UIKit', 'CoreMotion'
end
