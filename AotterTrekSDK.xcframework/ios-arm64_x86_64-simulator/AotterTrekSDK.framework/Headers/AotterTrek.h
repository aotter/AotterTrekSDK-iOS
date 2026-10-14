



#import <Foundation/Foundation.h>
#import "TKEnumHeaders.h"

typedef enum {
    TKUserKeyEmail,
    TKUserKeyPhone,
    TKUserKeyFbId,
    TKUserKeyGender,
    TKUserKeyBirthday,
    TKUserKeyFirstPartyId
}TKUserKey;


@interface AotterTrek : NSObject
@property NSString *trekClientId;
@property NSString *trekClientSecret;
@property NSString *myAppClientId;
@property NSString *myAppClientSecret;
@property NSDictionary *currentUser;
@property TKLoggerLevel loggerLevel;

+(AotterTrek *)sharedAPI;

+(NSString *)sdkVersion;

/**
 * Initialize Trek Service with your app client id and client secret
 * Note: this clears the MyApp client id for the rest of the app session; an app that also uses MyApp calls
 * initTrekServiceWithClientId:secret:myAppClientId:myAppClientSecret: instead, before AotterTrekSDK or a mediation
 * adapter initializes the SDK.
 * @param clientId      app client id
 * @param clientSecret  app client secret
 */
-(void)initTrekServiceWithClientId:(NSString *)clientId
                            secret:(NSString *)clientSecret;


/**
 * Initialize MyApp Service with your app client id and client secret
 * WARNING: this clears the Trek client id for the rest of the app session: Trek ads stop loading, and a later
 * initTrekService call does not bring them back. Do not call it in an app that shows Trek ads.
 * @param clientId      MyApp app client id
 * @param clientSecret  MyApp app client secret
 */
-(void)initMyAppServiceWithClientId:(NSString *)clientId
                            secret:(NSString *)clientSecret;
/**
 * Initialize Trek Service & MyApp Service with your app client id and client secret
 * @param clientId      app client id
 * @param clientSecret  app client secret
 * @param myAppClientId      MyApp app client id
 * @param myAppClientSecret  MyApp app client secret
 */
-(void)initTrekServiceWithClientId:(NSString *)clientId
                            secret:(NSString *)clientSecret
                     myAppClientId:(NSString *)myAppClientId
                 myAppClientSecret:(NSString *)myAppClientSecret;

/**
 * Enable Trek log with specific level.
 * @param level   TKLoggerLevelNone, TKLoggerLevelNormal, TKLoggerLevelDetail
 */
-(void)enableLoggerWithLevel:(TKLoggerLevel)level;


/**
 * Check if Trek service is initialized successfully or not.
 * @return is successed.
 */
-(BOOL)checkTrekServiceIsInitSuccess;

/**
 * Check if MyApp service is initialized successfully or not.
 * @return is successed.
 */
-(BOOL)checkMyAppServiceIsInitSuccess;



#pragma mark - Client user
/**
 Set your app's current user data
 @param email      string
 @param phone      string
 @param fbId       string
 @param gender     string, "M" or "F"
 @param birthday   birthday for format yyyy/MM/dd
 @param additonalMeta  customized meta data
 */
-(void)setCurrentUserWithEmail:(NSString *)email
                        phone:(NSString *)phone
                         fbId:(NSString *)fbId
                       gender:(NSString *)gender
                     birthday:(NSString *)birthday
                addtionalMeta:(NSDictionary *)additonalMeta;

/**
 Set your app's current user data
 @param email         string
 @param phone         string
 @param fbId          string
 @param gender        string, "M" or "F"
 @param birthday      birthday for format yyyy/MM/dd
 @param additonalMeta customized meta data
 @param firstPartyId  string. first party usage
 */
-(void)setCurrentUserWithEmail:(NSString *)email
                        phone:(NSString *)phone
                         fbId:(NSString *)fbId
                       gender:(NSString *)gender
                     birthday:(NSString *)birthday
                addtionalMeta:(NSDictionary *)additonalMeta
                  firstPartyId:(NSString *)firstPartyId;



/**
 Update one field of the current user. The value type depends on the key:
 TKUserKeyEmail, TKUserKeyPhone, TKUserKeyFbId: NSString.
 TKUserKeyBirthday: NSDateComponents, sent as year/month/day without zero padding.
 For these keys a value of another type, or an email or phone that fails validation, is ignored.
 TKUserKeyGender: an NSNumber holding a BOOL, YES for "M" and NO for "F". It is read with boolValue and not
 type-checked, so pass @YES or @NO: the string @"M" and nil both read as NO and send "F".
 TKUserKeyFirstPartyId: not supported here; pass it to setCurrentUserWithEmail:...firstPartyId: instead.
 */
-(void)updateCurrentUserWithValue:(id)value
                  forKey:(TKUserKey)TKUserKey;


-(void)removeCurrentUser;

@end
