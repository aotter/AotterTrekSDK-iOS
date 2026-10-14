//
//  ATKAdRequest.h
//  AotterTrekSDK
//
//  Targeting information for one ATKAdLoader load. `meta` is forwarded to the core's requestMeta; the core only
//  puts forceAdType / mediationVersion / mediationVersionCode on the wire, which the three typed properties cover.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ATKAdRequest : NSObject

@property (nonatomic, copy) NSString *category;
@property (nonatomic, copy) NSString *contentURL;
@property (nonatomic, copy) NSString *contentTitle;
/// Forwarded to the core's requestMeta, which puts only forceAdType / mediationVersion / mediationVersionCode
/// on the wire -- the three typed properties below cover those, so anything else here is ignored.
@property (nonatomic, copy) NSDictionary *meta;

/// Mediation adapters report their own version through these three; apps normally leave them nil.
@property (nonatomic, copy, nullable) NSString *mediationVersion;
@property (nonatomic, copy, nullable) NSNumber *mediationVersionCode;
/// Server-side override of the placement's ad type (mediation use only).
@property (nonatomic, copy, nullable) NSString *forceAdType;

- (instancetype)initWithCategory:(NSString *)category
                      contentURL:(NSString *)contentURL
                    contentTitle:(NSString *)contentTitle
                            meta:(NSDictionary *)meta;

@end

NS_ASSUME_NONNULL_END
