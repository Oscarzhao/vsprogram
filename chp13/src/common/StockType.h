/**
* 证券类型判别函数库
*
* @author   huangjian
* @date     2011-12-18
*/


#ifndef __STOCK_TYPE__
#define __STOCK_TYPE__

#include "common/MarketType.h"


#define STOCK_ALL     "XXXXXX"              /* 所有股票 */
#define STOCK_SPECIAL "FFFFFF"              /* 特殊股票 */

//股票类型
#define STOCK_TYPE_STOCK    0
#define STOCK_TYPE_INDEX    1
#define STOCK_TYPE_FUND     2
#define STOCK_TYPE_FUTURE   3

/* 市场判别 */
inline bool isShanghaiMarket(const string& market) {return market.compare(MARKET_SHANGHAI) == 0;}                      // 上证所
inline bool isShenzhenMarket(const string& market) {return market.compare(MARKET_SHENZHEN) == 0;}                      // 深交所
inline bool isZhongjinFuture(const string& market) {return market.compare(MARKET_INDEX_FUTURE) == 0;}                  // 中金所
inline bool isShanghaiFuture(const string& market) {return market.compare(MARKET_SHANGHAI_FUTURE) == 0;}               // 上期所
inline bool isDalianFuture(const string& market) {return market.compare(MARKET_DALIANG_FUTURE) == 0;}                  // 大商所
inline bool isZhengzhouFuture(const string& market) {return market.compare(MARKET_ZHENGZHOU_FUTURE) == 0;}             // 郑商所
inline bool isOpenFund(const string& market) {return market.compare(MARKET_OPEN_FUND) == 0;}                           // 开放基金
inline bool isCommodityFuture(const string&market) {return (market.compare(MARKET_SHANGHAI_FUTURE) == 0 || market.compare(MARKET_DALIANG_FUTURE) == 0 || market.compare(MARKET_ZHENGZHOU_FUTURE) == 0);} //商品期货
inline bool isFutureMarket(const string &market) {return isCommodityFuture(market) || isZhongjinFuture(market);}        // 期货市场
inline bool isStockMarket(const string& market) { return market.compare(MARKET_SHANGHAI) == 0 || market.compare(MARKET_SHENZHEN) == 0; } //是股票

/* 沪市证券类别判别 */
inline bool isShanghaiStockA(const string& stock) { return (stock[0] == '6') && (stock[1] == '0'); }                        // A股 60xxxx
inline bool isShanghaiStockB(const string& stock) { return (stock[0] == '9') && (stock[1] == '0'); }                        // B股 90xxxx
inline bool isShanghaiIndex(const string& stock) { return (stock[0] == '0') && (stock[1] == '0') && (stock[2] == '0'); }    // 指数 000xxx
inline bool isShanghaiFund(const string& stock) { return (stock[0] == '5') && (stock[1] == '0');}                           // 封基 50xxxx
inline bool isShanghaiETF(const string& stock) { return (stock[0] == '5') && (stock[1] == '1') && (stock[2] == '0' || stock[2] == '1' || stock[2] == '3' || stock[2] == '8');}  // ETF
inline bool isShanghaiWarrant(const string& stock) { return (stock[0] == '5') && (stock[1] == '8');}                        // 权证 58xxxx
inline bool isShanghaiSubscribe(const string& stock) { return stock[0] == '7' && (stock[1] == '3' || stock[1] == '8') && stock[2] == '0';}          //申购 730xxx 780xxx

inline bool isShanghaiGovernmentLoanRepurchaseTrusteeShip(const string& stock) { return (stock[0] == '2') && (stock[1] == '0') && (stock[2] == '1'); }    //国债回购（席位托管方式）
inline bool isShanghaiGovernmentLoanRepurchaseEnterprise(const string& stock) { return (stock[0] == '2') && (stock[1] == '0') && (stock[2] == '2'); }    //企业债回购 
inline bool isShanghaiGovernmentLoanRepurchaseBuyout(const string& stock) { return (stock[0] == '2') && (stock[1] == '0') && (stock[2] == '3'); }    //国债买断式回购
inline bool isShanghaiGovernmentLoanRepurchaseImpawn(const string& stock) { return (stock[0] == '2') && (stock[1] == '0') && (stock[2] == '4'); }    // 新质押式国债回购
inline bool isShanghaiGovernmentLoanRepurchase(const string& stock){return isShanghaiGovernmentLoanRepurchaseImpawn(stock) ||
                                                                    isShanghaiGovernmentLoanRepurchaseBuyout(stock) ||
                                                                    isShanghaiGovernmentLoanRepurchaseEnterprise(stock) ||
                                                                    isShanghaiGovernmentLoanRepurchaseTrusteeShip(stock);}
inline bool isShanghaiGovernmentLoanInterestBearing(const string & stock) { return stock[0] == '0' && stock[1] == '1' && (stock[2] == '0' || stock[2] == '9'); }    // 附息国债 010 019
inline bool isShanghaiFinancialBonds(const string & stock) { return stock[0] == '0' && stock[1] == '1' && stock[2] == '8'; }    // 金融债 018
inline bool isShanghaiGovernmentLoanDiscount(const string & stock) { return stock[0] == '0' && stock[1] == '2' && stock[2] == '0'; }    // 贴现国债 020
inline bool isShanghaiCorporateBond(const string& stock) { return (stock[0] == '1') && (stock[1] == '2') && (stock[2] == '2' || stock[2] == '4'); }    // 公司债 122 124
inline bool isShanghaiCentralGovernmentLoan(const string& stock) { return isShanghaiGovernmentLoanInterestBearing(stock) || isShanghaiGovernmentLoanDiscount(stock); }    // 中央政府债（国债）
inline bool isShanghaiSeperationBond(const string& stock) { return (stock[0] == '1') && ( stock[1] == '2') && (stock[2] == '6'); }    // 分离债 126
inline bool isShanghaiAssetSecuritization(const string& stock) { return (stock[0] == '1') && ( stock[1] == '2') && (stock[2] == '1'); }    // 资产证券化 121
inline bool isShanghaiCreditAssetSupported(const string& stock) { return (stock[0] == '1') && ( stock[1] == '2') && (stock[2] == '8'); }    //  信贷资产支持128
inline bool isShanghaiEnterpriseBond(const string& stock) { return (stock[0] == '1') && ( stock[1] == '2') && ( (stock[2] == '0') || (stock[2] == '9') ); }    // 企业债 （席位托管方式）120 129
inline bool isShanghaiConvertibleBond(const string& stock) { return (stock[0] == '1') && ((stock[1] == '0' && stock[2] == '0') || (stock[1] == '1' && stock[2] == '0') || (stock[1] == '1' && stock[2] == '2') || (stock[1] == '1' && stock[2] == '3')) && (!(stock[0] == '1' && stock[1] == '1' && stock[2] == '2')); }    // 可转债 100,110,112,113
inline bool isShanghaiLocalGovernmentLoan(const string & stock) { return stock[0] == '1' && stock[1] == '3' && stock[2] == '0'; }   // 地方债
inline bool isShanghaiGovernmentLoan(const string & stock) { return isShanghaiCentralGovernmentLoan(stock) || isShanghaiLocalGovernmentLoan(stock); }   // 政府债（国债+地方债）
inline bool isShanghaiLoan(const string& stock) { return isShanghaiGovernmentLoan(stock) || isShanghaiConvertibleBond(stock) || isShanghaiCorporateBond(stock) ||
                                                         isShanghaiEnterpriseBond(stock) || isShanghaiAssetSecuritization(stock) || isShanghaiSeperationBond(stock) \
                                                         || isShanghaiFinancialBonds(stock) || isShanghaiCreditAssetSupported(stock); }    // 上海债券
inline bool isShanghaiStandardBond(const string& stock) { return (stock.compare("888880") == 0 || stock.compare("SHRQ88") == 0); }    // 标准券
inline bool isShanghaiClosedEndedFunds(const string & stock) { return stock[0] == '5' && stock[1] == '0' && stock[2] == '0'; }  // 封闭式基金

inline bool isShanghaiPledge(const string &stock) { return (stock[0] == '0' && stock[1] == '9') || (stock[0] == '1' && stock[1] == '0' &&  (stock[2] <= '8' && stock[2] >= '3')); } //上海质押代码
/* 深市证券类别判别 */
inline bool isShenzhenMainBoard(const string & stock) { return stock[0] == '0' && stock[1] == '0' && stock[2] >= '0' && stock[2] <= '1'; }     // 主板 000xxx - 001xxx
inline bool isShenzhenSmeBoard(const string & stock) { return stock[0] == '0' && stock[1] == '0' && stock[2] >= '2' && stock[2] <= '4'; }      // 小板 002xxx - 004xxx
inline bool isShenzhenGemBoard(const string & stock) { return stock[0] == '3' && stock[1] == '0'; } // 创业板 30xxxx
inline bool isShenzhenStockA(const string& stock) { return ((stock[0] == '0') && (stock[1] == '0')) || ((stock[0] == '3') && (stock[1] == '0')); }  // A股 00xxxx 30xxxx
inline bool isShenzhenStockB(const string& stock) { return (stock[0] == '2') && (stock[1] == '0'); }    // B股 20xxxx
inline bool isShenzhenIndex(const string& stock) { return (stock[0] == '3') && (stock[1] == '9'); }     // 指数 39xxxx
inline bool isShenzhenFund(const string& stock) { return ((stock[0] == '1') && (stock[1] == '8')) || ((stock[0] == '1') && (stock[1] == '5')) || ((stock[0] == '1') && (stock[1] == '6')); }    // 18xxxx 16xxxx 15xxxx
inline bool isShenzhenETF(const string& stock) { return (stock[0] == '1') && (stock[1] == '5') && (stock[2] == '9') && (stock[3] == '9');}          // ETF 1599xx
inline bool isShenzhenWarrant(const string& stock) { return (stock[0] == '0') && (stock[1] == '3');}    // 权证 03xxxx
inline bool isShenzhenStartup(const string& stock) { return (stock[0] == '3') && (stock[1] == '0'); }   // 创业板 30xxxx

inline bool isShenzhenGovernmentLoanRepurchase(const string& stock) { return (stock[0] == '1') && (stock[1] == '3') && (stock[2] == '1') && (stock != "131990"); }    // 国债回购
inline bool isShenzhenGovernmentLoanInterestBearing(const string & stock) { return stock[0] == '1' && stock[1] == '0' && stock[2] >= '0' && stock[2] <= '7'; }  // 附息国债
inline bool isShenzhenGovernmentLoanDiscount(const string & stock) { return stock[0] == '1' && stock[1] == '0' && stock[2] == '8'; }    // 贴现国债
inline bool isShenzhenCentralGovernmentLoan(const string& stock) { return isShenzhenGovernmentLoanInterestBearing(stock) || isShenzhenGovernmentLoanDiscount(stock); }    // 中央政府债（国债）

inline bool isShenzhenCorporateBond(const string& stock) { return (stock[0] == '1') && (stock[1] == '1') && (stock[2] == '2'); }    // 公司债112xxx
inline bool isShenzhenEnterpriseBond(const string& stock) { return (stock[0] == '1') && (stock[1] == '1') && (stock[2] == '1'); }   // 企业债
inline bool isShenzhenSeperationBond(const string& stock) { return (stock[0] == '1') && (stock[1] == '1') && (stock[2] == '5'); }  // 分离债
inline bool isShenzhenMSPrivatePlacementBond(const string& stock) { return (stock[0] == '1') && (stock[1] == '1') && (stock[2] == '8'); }    //私募债-深交所特有
inline bool isShenzhenSpecialFundsManagerPlan(const string& stock) { return (stock[0] == '1') && (stock[1] == '1') && (stock[2] == '9'); }   //专项资金管理规划
inline bool isShenzhenLocalGovernmentLoan(const string& stock) { return (stock[0] == '1') && (stock[1] == '0') && (stock[2] == '9'); }   //地方政府债
inline bool isShenzhenGovernmentLoan(const string & stock) { return isShenzhenCentralGovernmentLoan(stock) || isShenzhenLocalGovernmentLoan(stock); }   // 政府债（国债+地方债）
inline bool isShenzhenDiscountGovernmentLoan(const string& stock) { return (stock[0] == '1') && (stock[1] == '0') && (stock[2] == '8'); }   //贴现式国债
inline bool isShenzhenInterestGovernmentLoan(const string& stock) { return (stock[0] == '1') && (stock[1] == '0') && (stock[2] >= '0') && (stock[2] <= '7'); }
inline bool isShenzhenConvertibleBond(const string & stock) { return (stock[0] == '1') && (stock[1] == '2') && (stock[2] >= '0') && (stock[2] <= '9'); }    // 可转债
inline bool isShenzhenLoan(const string& stock) { return isShenzhenCentralGovernmentLoan(stock) || isShenzhenCorporateBond(stock) || isShenzhenConvertibleBond(stock) || isShenzhenEnterpriseBond(stock) || isShenzhenSeperationBond(stock) ||
                                                         isShenzhenMSPrivatePlacementBond(stock) || isShenzhenSpecialFundsManagerPlan(stock) || isShenzhenLocalGovernmentLoan(stock) || isShenzhenDiscountGovernmentLoan(stock) || isShenzhenInterestGovernmentLoan(stock); }    // 深圳所有债券
inline bool isShenzhenStandardBond(const string& stock) { return (stock.compare("131990") == 0 || stock.compare("SZRQ88") == 0); }    // 标准券
inline bool isShenzhenClosedEndedFunds(const string & stock) { return stock[0] == '1' && stock[1] == '8' && stock[2] == '4'; }  // 封闭式基金

inline bool isShenzhenLOF(const string & stock) { return stock[0] == '1' && stock[1] == '6'; }  // LOF - 深市特有
inline bool isShenzhenGradedFunds(const string & stock) { return stock[0] == '1' && stock[1] == '5' && stock[2] == '0'; }   // 分级基金 - 深市特有

/* 沪市证券类别判别 */
inline bool isShanghaiStockA(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiStockA(stock); }         // A股
inline bool isShanghaiStockB(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiStockB(stock); }         // B股
inline bool isShanghaiIndex(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiIndex(stock); }           // 指数
inline bool isShanghaiFund(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiFund(stock);}              // 封基
inline bool isShanghaiETF(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiETF(stock);}                // ETF
inline bool isShanghaiWarrant(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiWarrant(stock);}        // 权证
inline bool isShanghaiSubscribe(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiSubscribe(stock);}    //申购

inline bool isShanghaiGovernmentLoanRepurchaseTrusteeShip(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseTrusteeShip(stock); }       
inline bool isShanghaiGovernmentLoanRepurchaseEnterprise(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseEnterprise(stock); }      
inline bool isShanghaiGovernmentLoanRepurchaseBuyout(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseBuyout(stock); }         
inline bool isShanghaiGovernmentLoanRepurchaseImpawn(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseImpawn(stock);}           
inline bool isShanghaiAssetSecuritization(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiAssetSecuritization(stock);}            
inline bool isShanghaiEnterpriseBond(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiEnterpriseBond(stock);}  // 企业债

inline bool isShanghaiGovernmentLoanInterestBearing(const string& market, const string & stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanInterestBearing(stock); }    // 附息国债
inline bool isShanghaiGovernmentLoanDiscount(const string& market, const string & stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoanDiscount(stock); }    // 贴现国债
inline bool isShanghaiCentralGovernmentLoan(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiCentralGovernmentLoan(stock); }    // 中央政府债（国债）
inline bool isShanghaiCorporateBond(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiCorporateBond(stock); }    // 公司债
inline bool isShanghaiConvertibleBond(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiConvertibleBond(stock); }    // 可转债
inline bool isShanghaiLocalGovernmentLoan(const string& market, const string & stock) { return isShanghaiMarket(market) && isShanghaiLocalGovernmentLoan(stock); }  // 地方债
inline bool isShanghaiGovernmentLoan(const string &market, const string & stock) { return isShanghaiMarket(market) && isShanghaiGovernmentLoan(stock); }  // 政府债（国债+地方债）
inline bool isShanghaiSeperationBond(const string & market, const string& stock) { return isShanghaiMarket(market) && isShanghaiSeperationBond(stock); }    // 分离债
inline bool isShanghaiLoan(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiLoan(stock); }    // 上海债券
inline bool isShanghaiStandardBond(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiStandardBond(stock); }    // 上海标准券

inline bool isShanghaiClosedEndedFunds(const string & market, const string & stock) { return isShanghaiMarket(market) && isShanghaiClosedEndedFunds(stock); }   // 封闭式基金

/* 深市证券类别判别 */
inline bool isShenzhenMainBoard(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenMainBoard(stock); }  // 主板
inline bool isShenzhenSmeBoard(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenSmeBoard(stock); }    // 中小板
inline bool isShenzhenGemBoard(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenGemBoard(stock); }    // 创业板
inline bool isShenzhenStockA(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenStockA(stock); }         // A股
inline bool isShenzhenStockB(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenStockB(stock); }         // B股
inline bool isShenzhenIndex(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenIndex(stock); }           // 指数
inline bool isShenzhenFund(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenFund(stock); }             // 封基
inline bool isShenzhenETF(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenETF(stock); }               // ETF
inline bool isShenzhenWarrant(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenWarrant(stock);}        // 权证
inline bool isShenzhenStartup(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenStartup(stock); }       // 创业板

inline bool isShenzhenGovernmentLoanInterestBearing(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenGovernmentLoanInterestBearing(stock); }    // 附息国债
inline bool isShenzhenGovernmentLoanDiscount(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenGovernmentLoanDiscount(stock); }    // 贴现国债
inline bool isShenzhenCentralGovernmentLoan(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenCentralGovernmentLoan(stock); }   // 中央政府债（国债）
inline bool isShenzhenCorporateBond(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenCorporateBond(stock); }       // 公司债
inline bool isShenzhenConvertibleBond(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenConvertibleBond(stock); } 

inline bool isShenzhenMSPrivatePlacementBond(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenMSPrivatePlacementBond(stock); } 
inline bool isShenzhenEnterpriseBond(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenEnterpriseBond(stock); }     // 企业债
inline bool isShenzhenSpecialFundsManagerPlan(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenSpecialFundsManagerPlan(stock); } 
inline bool isShenzhenSeperationBond(const string & market, const string& stock) { return isShenzhenMarket(market) && isShenzhenSeperationBond(stock); }    // 分离债
inline bool isShenzhenDiscountGovernmentLoan(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenDiscountGovernmentLoan(stock); } 
inline bool isShenzhenInterestGovernmentLoan(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenInterestGovernmentLoan(stock); } 
inline bool isShenzhenLocalGovernmentLoan(const string& market, const string & stock) { return isShenzhenMarket(market) && isShenzhenLocalGovernmentLoan(stock); }  // 地方债
inline bool isShenzhenGovernmentLoan(const string &market, const string & stock) { return isShenzhenMarket(market) && isShenzhenGovernmentLoan(stock); }  // 政府债（国债+地方债）
inline bool isShenzhenLoan(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenLoan(stock); }
inline bool isShenzhenStandardBond(const string& market, const string& stock) { return isShenzhenMarket(market) && isShenzhenStandardBond(stock); }    // 深圳标准券

inline bool isShenzhenClosedEndedFunds(const string & market, const string & stock) { return isShenzhenMarket(market) && isShenzhenClosedEndedFunds(stock); }   // 封闭式基金
inline bool isShenzhenLOF(const string & market, const string & stock) { return isShenzhenMarket(market) && isShenzhenLOF(stock); } // LOF - 深市特有
inline bool isShenzhenGradedFunds(const string & market, const string & stock) { return isShenzhenMarket(market) && isShenzhenGradedFunds(stock); } // 分级基金 - 深市特有

/* 证券类别判别 */
inline bool isIndex(const string& market, const string& stock){return (isShanghaiIndex(market, stock) ) || ( isShenzhenIndex(market, stock) );}             // 指数
inline bool isStockA(const string& market, const string& stock){return (isShanghaiStockA(market, stock) ) || ( isShenzhenStockA(market, stock) );}          // A股
inline bool isStockB(const string& market, const string& stock){return (isShanghaiStockB(market, stock) ) || ( isShenzhenStockB(market, stock) );}          // B股
inline bool isStock(const string& market, const string& stock){return (isStockA(market, stock) ) || (isStockB(market, stock) );}                            // 侠义的股票（即A股和B股）
inline bool isStockEx(const string& market, const string& stock){return !isIndex(market, stock);}                                                           // 广义的股票
inline bool isFund(const string& market, const string& stock){return (isShanghaiFund(market, stock) ) || ( isShenzhenFund(market, stock) ) ;}               // 基金
inline bool isETF(const string& market, const string& stock){return (isShanghaiETF(market, stock) ) || ( isShenzhenETF(market, stock) );}                   // ETF
inline bool isClosedEndedFunds(const string& market, const string& stock){return (isShanghaiClosedEndedFunds(market, stock)) || (isShenzhenClosedEndedFunds(market, stock));}      // 封闭式基金
inline bool isWarrant(const string& market, const string& stock){return (isShanghaiWarrant(market, stock) ) || ( isShenzhenWarrant(market, stock) );}       // 权证
inline bool isLoan(const string& market, const string& stock){return (isShanghaiLoan(market, stock) ) || ( isShenzhenLoan(market, stock) );}       // 债券

inline bool isGovernmentLoanRepurchase(const string& stock) { return (isShanghaiGovernmentLoanRepurchase(stock)) || (isShenzhenGovernmentLoanRepurchase(stock)); }    // 债券回购
inline bool isStandardBond(const string& market, const string& stock) { return (isShanghaiStandardBond(market, stock)) || (isShenzhenStandardBond(market, stock)); }    // 标准券

inline bool isLoanRepurchaseBuyout(const string& market, const string& stock) { return (isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseBuyout(stock)) ;} //是否买断式回购 深圳还没看到买断式回购
inline bool isLoanRepurchaseImpawn(const string& market, const string& stock) { return (isShanghaiMarket(market) && isShanghaiGovernmentLoanRepurchaseImpawn(stock)) || ( isShenzhenMarket(market) && isShenzhenGovernmentLoanRepurchase(stock));}//是否质押式回购

inline bool isNameSTStock(const string & name) { return name[0] == 'S' && name[1] == 'T'; } // ST股
inline bool isName7STStock(const string & name) { return name[0] == '*' && name[1] == 'S' && name[2] == 'T'; }  // *ST股
inline bool isNameSStock(const string & name) { return name[0] == 'S' && name[1] != '*' && name[1] != 'S' && name[1] != 'T'; }  // S股
inline bool isNameSSTStock(const string & name) { return name[0] == 'S' && name[1] == 'S' && name[2] == 'T'; }  // SST股
inline bool isNameS7STStock(const string & name) { return name[0] == 'S' && name[1] == '*' && name[2] == 'S' && name[3] == 'T'; }   // S*ST股

inline bool isHedgeSP(const string& market, const string& code) {return MARKET_DETAIL_DALIANG_FUTURE == market && code[0] == 'S' && code[1] == 'P' && code[2] == ' ' && code.npos != code.find('&');}   //以SP打头的大连同品种跨期组合合约
inline bool isHedgeSPC(const string& market, const string& code) {return MARKET_DETAIL_DALIANG_FUTURE == market && code[0] == 'S' && code[1] == 'P' && code[2] == 'C' && code.npos != code.find('&');}  //以SPC打头的大连同期跨品种组合合约
inline bool isHedgeSPD(const string& market, const string& code) {return MARKET_DETAIL_ZHENGZHOU_FUTURE == market && code[0] == 'S' && code[1] == 'P' && code[2] == 'D' && code.npos != code.find('&');}  //以SPD打头的郑州同品种跨期组合合约

/* 期货股票类别判别 */
inline bool isNewStock(const string& stock) {return (stock[0] == 'N');}
inline bool isSTStock(const string& stock) {return (string::npos != stock.find("ST"));}
inline bool isSOrSTStock(const string& stock) {return (string::npos != stock.find("ST") || stock.find("S") == 0);}


//黄金ETF 先写死，回头再考虑怎么动态加载
inline bool isGoldETF(const string& market, const string& stock) { return (isShanghaiMarket(market) && (stock.compare("518880") == 0 || stock.compare("518800") == 0)) || (isShenzhenMarket(market) && stock.compare("159934") == 0); }

inline bool isShanghaiFixedIncomeETF(const string& stock) { return (stock[0] == '5') && (stock[1] == '1') && (stock[2] == '1');  } //511号段，上海的固定收益类

//固定收益:跟踪债券指数的交易型开放式指数基金、交易型货币市场基金
inline bool isFixedIncomeETF(const string& market, const string& stock) { return isShanghaiMarket(market) && isShanghaiFixedIncomeETF(stock);}

//实时申赎货币基金，这是T+1的
inline bool isRealTimeMonetaryFund(const string& market, const string& stock) {return (isShanghaiMarket(market) && (stock.substr(0, 4) == "5198")) || (isShenzhenMarket(market) && stock.substr(0, 4) == "1590");}

//交易型货币基金 T+0
inline bool isTransactionMonetaryFund(const string& market, const string& stock) {return (isShanghaiMarket(market) && (stock == "511880" || stock == "511990"));}

//货币基金
inline bool isMonetaryFund(const string& market, const string& stock) {return isRealTimeMonetaryFund(market, stock) || isTransactionMonetaryFund(market, stock);}

//是否是上海申购代码
inline bool isShanghaiIPO(const string& market, const string& stock) {return (isShanghaiMarket(market) && ( stock.substr(0, 3) == "730" || stock.substr(0, 3) == "732" ||stock.substr(0, 3) == "780" || stock.substr(0, 3) == "733" || stock.substr(0, 3) == "735")); }

//edited by zld 20130304 增加上海申购代码，深圳的申购代码和上市后代码一致，不会不能交易
//可交易的
inline bool isTranableStock(const string& market, const string& stock) { 
    return isStockMarket(market) && \
            (\
                isStock(market, stock) || isFund(market, stock) || isETF(market, stock) || isWarrant(market, stock)\
                || isShanghaiSubscribe(market, stock) || isShenzhenStartup(market, stock) || isGovernmentLoanRepurchase(stock) \
                || isLoan(market, stock) || isFixedIncomeETF(market, stock) || isGoldETF(market, stock) || isMonetaryFund(market, stock)\
            || isShanghaiCentralGovernmentLoan(market, stock) || isShenzhenCentralGovernmentLoan(market, stock) \
            || isShanghaiLocalGovernmentLoan(market, stock) || isShenzhenLocalGovernmentLoan(market, stock)\
            || isShanghaiIPO(market, stock)
            ); 
} 

//判断是否是回转交易
//债券、跟踪债券指数的交易型开放式指数基金、交易型货币市场基金、黄金交易型开放式证券投资基金和权证实行当日回转交易，B股实行次交易日起回转交易 除了债券以外的品种，后续需要补充判断
inline bool isIntraDay(const string& market, const string& stock) { return isLoan(market, stock) || isGoldETF(market, stock) || isFixedIncomeETF(market, stock) || isWarrant(market, stock); }

//连续合约, 连续合约实例, 如IF00, IFL0, 
inline bool isContinueEx(const string& stock)  { return stock.size() > 2 &&  \
    ( '0' <= stock[stock.size() - 1] && '9' >= stock[stock.size() - 1] ) &&
    ( '0' > stock[stock.size() - 3] || '9' < stock[stock.size() - 3]);\
};

inline double getPricePrecision(const string &market, const string &code)
{
    if (market.compare(MARKET_SHANGHAI) == 0)
    {
        if ( isShanghaiStockB(code) || isShanghaiETF(code) || isShanghaiFund(code))//按照交易规则，这些精度为0.001
        {
            return 0.001;
        }
        else if (isShanghaiGovernmentLoanRepurchaseImpawn(code))//按照交易规则，这些精度为0.005
        {
            return 0.005;
        }
        else
        {
            return 0.01;
        }
    }
    else if (market.compare(MARKET_SHENZHEN) == 0)
    {
        if( isShenzhenFund(code) || isShenzhenLoan(code) || isShenzhenGovernmentLoanRepurchase(code) || isMonetaryFund(market, code) || isGoldETF(market, code))
            return 0.001;
        else
            return 0.01;
    }
    else if (market.compare(MARKET_INDEX_FUTURE) == 0)
    {
        if (!strncmp(code.c_str(), "TF", 2))
            return 0.002;
        else
            return 0.2;
    }
    else if (market.compare(MARKET_SHANGHAI_FUTURE) == 0)
    {
        if (!strncmp(code.c_str(), "au", 2))
            return 0.05;
        else if (!strncmp(code.c_str(), "ru", 2) ||
            !strncmp(code.c_str(), "al", 2) ||
            !strncmp(code.c_str(), "pb", 2) ||
            !strncmp(code.c_str(), "zn", 2))
            return 5;
        else if (!strncmp(code.c_str(), "cu", 2))
            return 10;
        else
            return 1.0;
    }
    else if (market.compare(MARKET_DALIANG_FUTURE) == 0)
    {
        return 1.0;
    }
    else if (market.compare(MARKET_ZHENGZHOU_FUTURE) == 0)
    {
        return 1.0;
    }

    return 0.01;
}

inline double getPricePrecisionByRealMarket(const string &market, const string &code)
{
    if (market == MARKET_DETAIL_INDEX_FUTURE)
    {
        return getPricePrecision(MARKET_INDEX_FUTURE, code);
    }
    else if (market == MARKET_DETAIL_SHANGHAI_FUTURE)
    {
        return getPricePrecision(MARKET_SHANGHAI_FUTURE, code);
    }
    else if (market == MARKET_DETAIL_DALIANG_FUTURE)
    {
        return getPricePrecision(MARKET_DALIANG_FUTURE, code);
    }
    else if (market == MARKET_DETAIL_ZHENGZHOU_FUTURE)
    {
        return getPricePrecision(MARKET_ZHENGZHOU_FUTURE, code);
    }
    return getPricePrecision(market, code);
}

inline string getMarketIndexCode(string market)
{
    string ret;
    static std::pair<string, string> market2index[] = {pair<string, string>(MARKET_SHANGHAI, "000001"), pair<string, string>(MARKET_SHENZHEN, "399001")};
    for (unsigned int i = 0; i < sizeof(market2index)/sizeof(market2index[0]); i++)
    {
        if (market2index[i].first == market)
        {
            return market2index[i].second;
        }
    }

    return ret;
}

inline bool isValidStockCode(const string& market, const string& stock)
{
    return isIndex(market , stock)
        || isStock(market , stock)
        || isETF(market , stock)
        || isWarrant(market , stock)
        || isFund(market , stock) 
        || isLoan(market, stock);
}

inline string getMarketByStockCode(string code)
{
    if ( isShanghaiStockA(code) || isShanghaiStockB(code) || isShanghaiFund(code) || isShanghaiETF(code) || isShanghaiWarrant(code) || isShanghaiLoan(code) || isShanghaiGovernmentLoanRepurchase(code))
    {
        return MARKET_SHANGHAI;
    }
    if ( isShenzhenStockA(code) || isShenzhenStockB(code) || isShenzhenFund(code) || isShenzhenETF(code) || isShenzhenWarrant(code) || isShenzhenStartup(code) || isShenzhenLoan(code) || isShenzhenGovernmentLoanRepurchase(code))
    {
        return MARKET_SHENZHEN;
    }
    return "";
}

inline int getTradeMinUnit(string market, string strCode)
{
    if (isShanghaiGovernmentLoanRepurchaseBuyout(market, strCode))//债券买断式回购 1000手，1手10张
    {
        return 10000;
    }
    else if (isShanghaiGovernmentLoanRepurchase(strCode))//上海债券其他回购 100手，1手10张
    {
        return 1000;
    }
    else if (isShenzhenGovernmentLoanRepurchase(strCode))//深圳质押回购，10张
    {
        return 10;
    }
    else if (isLoan(market, strCode))//债券10张
    {
        return 10;
    }
    else if (isStockMarket(market))
    {
        return 100;
    }
    return 1;
}

inline int getUiMinUnit(string market, string strCode, bool bIsBuy)
{
    if (isShanghaiGovernmentLoanRepurchaseBuyout(market, strCode))//债券买断式回购 1000手，1手10张
    {
        return 10000;
    }
    else if (isShanghaiGovernmentLoanRepurchase(strCode))//上海债券其他回购 100手，1手10张
    {
        return 1000;
    }
    else if (isShenzhenGovernmentLoanRepurchase(strCode))//深圳质押回购，10张
    {
        return 10;
    }
    else if (isLoan(market, strCode) && bIsBuy)//债券10张
    {
        return 10;
    }
    else if (isStockMarket(market) && bIsBuy)
    {
        return 100;
    }
    return 1;
}

// 逆回购：m张 -> n个下单单位
inline int convertZhangToNum(int nZhang, string strCode)
{
    return isShanghaiGovernmentLoanRepurchase(strCode) ? (nZhang * 10) : nZhang;
}

// 逆回购：n个下单单位 -> m张
inline int convertNumToZhang(int nNum, string strCode)
{
    return isShanghaiGovernmentLoanRepurchase(strCode) ? (nNum / 10) : nNum;
}

#endif
