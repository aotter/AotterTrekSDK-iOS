//
//  ATKUser.h
//  AotterTrekSDK
//
//  Current-user descriptor for AotterTrekSDK.setCurrentUser:. Values are hashed / filtered by the core exactly
//  as AotterTrek setCurrentUserWithEmail:phone:fbId:gender:birthday:addtionalMeta:firstPartyId: does.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ATKUser : NSObject

@property (nonatomic, copy) NSString *email;
@property (nonatomic, copy) NSString *phone;
/// yyyy/MM/dd, sent as given.
@property (nonatomic, copy) NSString *birthDay;
/// "M" or "F".
@property (nonatomic, copy) NSString *gender;
@property (nonatomic, copy) NSString *fbId;
@property (nonatomic, copy) NSString *firstPartyId;
@property (nonatomic, copy) NSDictionary *meta;

- (instancetype)initWithEmail:(NSString *)email
                        phone:(NSString *)phone
                     birthDay:(NSString *)birthDay
                       gender:(NSString *)gender
                         fbId:(NSString *)fbId
                 firstPartyId:(NSString *)firstPartyId
                         meta:(NSDictionary *)meta;

@end

NS_ASSUME_NONNULL_END
