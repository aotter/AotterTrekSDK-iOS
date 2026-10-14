// swift-tools-version:5.9
// Release manifest of the AotterTrek iOS SDK (repo aotter/AotterTrekSDK-iOS).
// One product ships both binaries: a binaryTarget cannot declare dependencies, and the SDK does not autolink OMSDK.
// AotterTrekSDKSwiftSupport is an empty Swift target and is not optional: the SDK binary contains a Swift VAST engine
// whose objects autolink the Swift runtime, and a pure Objective-C app has no Swift search paths to satisfy that.
import PackageDescription

let package = Package(
    name: "AotterTrekSDK-iOS",
    platforms: [.iOS(.v15)],
    products: [
        .library(name: "AotterTrekSDK", targets: ["AotterTrekSDKSwiftSupport", "AotterTrekSDK", "OMSDK_Aotternet"]),
    ],
    targets: [
        .binaryTarget(name: "AotterTrekSDK", path: "AotterTrekSDK.xcframework"),
        .binaryTarget(name: "OMSDK_Aotternet", path: "OMSDK_Aotternet.xcframework"),
        .target(
            name: "AotterTrekSDKSwiftSupport",
            dependencies: ["AotterTrekSDK", "OMSDK_Aotternet"],
            path: "Sources/AotterTrekSDKSwiftSupport"),
    ]
)
