/**
* 证券类型判别函数库
*
* @author   huangjian
* @date     2011-12-18
*/

 
#ifndef __MARKET_TYPE__
#define __MARKET_TYPE__


/**
* 证卷交易市场定义
**/
#define MARKET_SHANGHAI         "SH"            /* 上交所 */
#define MARKET_SHENZHEN         "SZ"            /* 深交所 */
#define MARKET_INDEX_FUTURE     "IF"            /* 股指期货 */
#define MARKET_SHANGHAI_FUTURE  "SF"            /* 上海商品期货 */
#define MARKET_DALIANG_FUTURE   "DF"            /* 大连商品期货 */
#define MARKET_ZHENGZHOU_FUTURE "ZF"            /* 郑州商品期货 */

#define MARKET_FUTURES          "FU"            /* 代指所有期货 */

#define MARKET_OPEN_FUND        "OF"            /* 开放式基金 */
#define MARKET_ALL              "XX"            /* 所有市场 */

#define MARKET_DETAIL_INDEX_FUTURE     "CFFEX"            /* 股指期货 */
#define MARKET_DETAIL_SHANGHAI_FUTURE  "SHFE"            /* 上海商品期货 */
#define MARKET_DETAIL_DALIANG_FUTURE   "DCE"            /* 大连商品期货 */
#define MARKET_DETAIL_ZHENGZHOU_FUTURE "CZCE"            /* 郑州商品期货 */

/**
* 市场状态
*/
#define MARKET_STATUS_UNKNOW        0           /* 未知 */
#define MARKET_STATUS_OPEN          1           /* 日间开盘 */
#define MARKET_STATUS_CLOSE         2           /* 闭市 */
#define MARKET_STATUS_NIGHT_OPEN    3           /* 夜间开盘 */


/**
* 时间戳定义
*/
#define TIMETAG_START    0x92F1DFC800   /* 起始时间, 1990-1-1 */    //实在不知道起始时间可以用这个,已经是毫秒了
#define TIMETAG_END      0x1F3FFFFC180  /* 结束时间, 2038-1-19 */   //实在不知道结束时间可以用这个,已经是毫秒了
#define TIMETAG_LATEST   0x1F3FFFFF830  /* 最新时间*/               //通过这个时间可以获得最新的数据,已经是毫秒了
#define TIMETAG_SPECAIL  0x1F3FFFFFC18  /* 特殊时间*/               //特殊时间，留待他用,已经是毫秒了
#define COUNT_MAX        INT32_MAX      /* 最大量*/

/**
* 序列号定义
*/
#define SEQ_START  1           /* 起始序列号 */

/************************************************************************/
/* 交易所名称转换                                                */
/************************************************************************/
inline string getMarketNameByTraderName(string market)
{
    if (market.compare("SHFE") == 0)
    {
        return "SF";
    }
    else if (market.compare("DCE") == 0)
    {
        return "DF";
    }
    else if (market.compare("CZCE") == 0)
    {
        return "ZF";
    }
    else if (market.compare("CFFEX") == 0)
    {
        return "IF";
    }
    return market;
};

inline string getTraderNameByMarketName(string market)
{
    if (market.compare("SF") == 0)
    {
        return "SHFE";
    }
    else if (market.compare("DF") == 0)
    {
        return "DCE";
    }
    else if (market.compare("ZF") == 0)
    {
        return "CZCE";
    }
    else if (market.compare("IF") == 0)
    {
        return "CFFEX";
    }
    return market;
};

#endif