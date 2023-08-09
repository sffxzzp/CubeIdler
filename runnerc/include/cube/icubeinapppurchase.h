#ifndef icubinapppurchase_h__
#define icubinapppurchase_h__



#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"
#include "./base/cubestring.hpp"
namespace Cube {
#pragma pack(push, 1)


enum InAppPurchaseProductDiscountType {
    kInAppPurchaseProductDiscountTypeInvalid     = 0,
    kInAppPurchaseProductDiscountTypeNone        = 1,  // 没有折扣
    kInAppPurchaseProductDiscountTypePermanent   = 2,  // 永久折扣
    kInAppPurchaseProductDiscountTypeTimed       = 3,  // 限时折扣
};

struct CubeDiscountInfo {
    int32_t off;                            // 折扣率，[0~100)之间:
                                            // 15 - 15%off - 8.5折
                                            // 20 - 20%off - 8折
    int32_t discountPrice;                  // 折扣后的价格,根据off值自动计算

    InAppPurchaseProductDiscountType type;  // 折扣类型
    uint32_t startTime;                     // 限时折扣开始时间，只对限时折扣类型有效
    uint32_t endTime;                       // 限时折扣结束时间，只对限时折扣类型有效
};

struct CubeInAppPurchaseProductExtraInfo {
    CubeString exchangeRule;    // 道具的合成规则
    CubeString bundleRule;      // 道具的打包规则
};

struct CubeInAppPurchaseProductInfo {
    CubeProductId_t id;                             // 道具ID
    CubeString apiName;
    bool isPurchasable;                             // 道具是否可以购买
    CubeString name;                                // 道具名称
    CubeString description;                         // 道具描述
    CubeString category;                            // 道具类别
    CubeString icon;                                // 道具图标(小图)url
    CubeString thumbnail;                           // 道具小样(大图)url
    CubeInAppPurchaseProductExtraInfo extraInfo;    // 道具附加信息
    int32_t originalPrice;                          // 道具原价, isPurchasable=true时有效
    CubeString currencyType;                        // 货币种类, isPurchasable=true时有效
    CubeDiscountInfo discount;                      // 折扣信息, isPurchasable=true时有效
};





struct CubeProductItem {
    CubeString apiName;
    int32_t quantity;
};

class ICubeInAppPurchase {
public:
    // 获取全部可购买商品信息
    virtual CubeResult AsyncRequestAllPurchasableProducts(const char* userData) = 0;

    virtual bool GetProductInfo(const char* apiName,
                                CubeInAppPurchaseProductInfo* product) = 0;


    virtual CubeResult AsyncCreateOrder(const CubeProductItem* cartItems,
                                        int32_t itemSize,
                                        const char* userData) = 0;

    //购买
//     virtual CubeResult AsyncPurchaseProducts(const CubeProductItem* cartItems,
//                                              int32_t itemSize,
//                                              const char* userData) = 0;

    virtual CubeResult AsyncFinishOrder(const char* orderId,
                                        const char* userData) = 0;

	virtual CubeResult AsyncGetOrderInfo(const char* orderId, const char* userData) = 0;
};

#pragma pack(pop)

}
#endif // icubinapppurchase_h__
