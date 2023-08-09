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
    kInAppPurchaseProductDiscountTypeNone        = 1,  // û���ۿ�
    kInAppPurchaseProductDiscountTypePermanent   = 2,  // �����ۿ�
    kInAppPurchaseProductDiscountTypeTimed       = 3,  // ��ʱ�ۿ�
};

struct CubeDiscountInfo {
    int32_t off;                            // �ۿ��ʣ�[0~100)֮��:
                                            // 15 - 15%off - 8.5��
                                            // 20 - 20%off - 8��
    int32_t discountPrice;                  // �ۿۺ�ļ۸�,����offֵ�Զ�����

    InAppPurchaseProductDiscountType type;  // �ۿ�����
    uint32_t startTime;                     // ��ʱ�ۿۿ�ʼʱ�䣬ֻ����ʱ�ۿ�������Ч
    uint32_t endTime;                       // ��ʱ�ۿ۽���ʱ�䣬ֻ����ʱ�ۿ�������Ч
};

struct CubeInAppPurchaseProductExtraInfo {
    CubeString exchangeRule;    // ���ߵĺϳɹ���
    CubeString bundleRule;      // ���ߵĴ������
};

struct CubeInAppPurchaseProductInfo {
    CubeProductId_t id;                             // ����ID
    CubeString apiName;
    bool isPurchasable;                             // �����Ƿ���Թ���
    CubeString name;                                // ��������
    CubeString description;                         // ��������
    CubeString category;                            // �������
    CubeString icon;                                // ����ͼ��(Сͼ)url
    CubeString thumbnail;                           // ����С��(��ͼ)url
    CubeInAppPurchaseProductExtraInfo extraInfo;    // ���߸�����Ϣ
    int32_t originalPrice;                          // ����ԭ��, isPurchasable=trueʱ��Ч
    CubeString currencyType;                        // ��������, isPurchasable=trueʱ��Ч
    CubeDiscountInfo discount;                      // �ۿ���Ϣ, isPurchasable=trueʱ��Ч
};





struct CubeProductItem {
    CubeString apiName;
    int32_t quantity;
};

class ICubeInAppPurchase {
public:
    // ��ȡȫ���ɹ�����Ʒ��Ϣ
    virtual CubeResult AsyncRequestAllPurchasableProducts(const char* userData) = 0;

    virtual bool GetProductInfo(const char* apiName,
                                CubeInAppPurchaseProductInfo* product) = 0;


    virtual CubeResult AsyncCreateOrder(const CubeProductItem* cartItems,
                                        int32_t itemSize,
                                        const char* userData) = 0;

    //����
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
