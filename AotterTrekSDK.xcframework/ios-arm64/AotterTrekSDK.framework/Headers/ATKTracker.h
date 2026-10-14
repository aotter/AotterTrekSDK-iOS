//
//  ATKTracker.h
//  AotterTrekSDK
//
//  Reports what the user did in the app (read a post, visited a place, ...) to the AotterTrek tracker, in the format
//  and to the endpoint the Android SDK's Tracker uses; the user carries hashEmail / hashPhone, as the SDK's ad requests
//  do. TKTracker keeps working unchanged; the two report to different endpoints in different formats.
//

#import <Foundation/Foundation.h>
#import "ATKUser.h"

NS_ASSUME_NONNULL_BEGIN

/// What the user did. Sent as the string in the comment. The tracker currently accepts READ_POST, VISIT_PLACE,
/// PLAY_GAME, LISTEN_MUSIC, WATCH_VIDEO, CALL_MERCHANT and BUY_ITEM, and drops a report that carries any other value.
typedef NS_ENUM(NSInteger, ATKTrackerActionType) {
    ATKTrackerActionTypeReadPost = 0,   // READ_POST
    ATKTrackerActionTypeCreatePost,     // CREATE_POST
    ATKTrackerActionTypeInitialUsage,   // INITIAL_USAGE
    ATKTrackerActionTypeVisitPlace,     // VISIT_PLACE
    ATKTrackerActionTypePlayGame,       // PLAY_GAME
    ATKTrackerActionTypeListenMusic,    // LISTEN_MUSIC
    ATKTrackerActionTypeWatchVideo,     // WATCH_VIDEO
    ATKTrackerActionTypeCallMerchant,   // CALL_MERCHANT
    ATKTrackerActionTypeBuyItem,        // BUY_ITEM
    ATKTrackerActionTypeUnknown,        // UNKNOWN
};

/// What the user acted on. Sent as the string in the comment. The tracker currently drops a report whose entity type
/// is UNKNOWN.
typedef NS_ENUM(NSInteger, ATKTrackerEntityType) {
    ATKTrackerEntityTypePost = 0,   // POST
    ATKTrackerEntityTypePlace,      // PLACE
    ATKTrackerEntityTypeGame,       // GAME
    ATKTrackerEntityTypeMusic,      // MUSIC
    ATKTrackerEntityTypeVideo,      // VIDEO
    ATKTrackerEntityTypeMerchant,   // MERCHANT
    ATKTrackerEntityTypeItem,       // ITEM
    ATKTrackerEntityTypeUnknown,    // UNKNOWN
};

/// The thing the user acted on: a post, a place, a video, ...
@interface ATKTrackerEntity : NSObject

@property (nonatomic, assign) ATKTrackerEntityType type;
/// Your id for the entity; sent as `id`.
@property (nonatomic, copy) NSString *entityId;
@property (nonatomic, copy) NSString *title;
/// Required, as on Android: the tracker drops a report whose entity has no url.
@property (nonatomic, copy) NSString *url;
@property (nonatomic, copy) NSArray<NSString *> *categories;
@property (nonatomic, copy) NSArray<NSString *> *tags;
/// Anything else worth reporting. It must be valid JSON: a report that cannot be serialized is not sent.
@property (nonatomic, copy) NSDictionary<NSString *, id> *meta;

- (instancetype)initWithType:(ATKTrackerEntityType)type
                    entityId:(NSString *)entityId
                       title:(NSString *)title
                         url:(NSString *)url NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

/// Where the user was. Every field is optional. The tracker does not read `location` today; the SDK sends it so the
/// report carries the same fields as Android's.
@interface ATKTrackerLocation : NSObject

/// Sent as `id`.
@property (nonatomic, copy, nullable) NSString *locationId;
@property (nonatomic, copy, nullable) NSString *title;
@property (nonatomic, copy, nullable) NSString *url;
@property (nonatomic, copy, nullable) NSString *address;
@property (nonatomic, copy) NSArray<NSString *> *categories;
/// Degrees; sent as `lat` / `lng` when set.
@property (nonatomic, strong, nullable) NSNumber *latitude;
@property (nonatomic, strong, nullable) NSNumber *longitude;
/// Must be valid JSON, like ATKTrackerEntity.meta.
@property (nonatomic, copy) NSDictionary<NSString *, id> *meta;

@end

/// One report. Create one per record, set it up by chaining, and call `send`:
///
///     let tracker = ATKTracker().entity(post).actionType(.readPost).start()
///     // ... when the user leaves the post:
///     tracker.send()
///
/// `send` does nothing but log when the SDK is not initialized, the entity, its url or the action type is missing, the
/// report is not valid JSON, or it was already sent. The result of the request is not reported back. Not thread-safe:
/// set up and send one tracker from one thread.
@interface ATKTracker : NSObject

/// Required.
- (instancetype)entity:(ATKTrackerEntity *)entity;
/// Required.
- (instancetype)actionType:(ATKTrackerActionType)actionType;
- (instancetype)location:(nullable ATKTrackerLocation *)location;
/// The user to report. Without one, `send` reports the current user (AotterTrekSDK setCurrentUser:) as it is when
/// `send` runs. The email and the phone number are hashed the way setCurrentUser: hashes them, and never sent as given.
- (instancetype)user:(nullable ATKUser *)user;
/// How long the user spent, in seconds. Overrides the time measured from `start`.
- (instancetype)timeSpan:(NSInteger)seconds;
/// Starts the clock: unless timeSpan: is set, `send` reports the whole seconds elapsed since the last `start`, rounded.
/// Without either, the time spent is 0.
- (instancetype)start;
/// Sends the report once.
- (void)send;

@end

NS_ASSUME_NONNULL_END
