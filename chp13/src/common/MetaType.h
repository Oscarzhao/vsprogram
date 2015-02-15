/**
* meta id 的定义
*
* @author   huangjian
* @date     2011-12-18
*/


#ifndef __META_TYPE__
#define __META_TYPE__

#ifdef __cplusplus
extern "C" {
#endif

// 主结构字段
    #define META_DATA_KEY_REQUEST                   "R"        // 请求信息
    #define META_DATA_KEY_META_INFO                 "F"        // 数据信息
    #define META_DATA_KEY_CONTENT                   "C"        // 数据内容
    #define META_DATA_KEY_CONTENT_COMPRESS          "D"        // 压缩的数据项
    #define META_DATA_KEY_CONTENT_COMPRESS_ZLIB     "Z"        // zlib压缩
    #define META_DATA_KEY_CONTENT_COMPRESS_ZLIB2    "Z2"       // zlib压缩2, 用于改造后的压缩算法
    #define META_DATA_KEY_CONTENT_COMPRESS_LZMA     "LZ"       // lzma压缩

    #define META_DATA_KEY_CONTENT_COMPRESS_BIN      "B"       // 压缩后的bindata
    #define META_DATA_KEY_CONTENT_COMPRESS_TYPE     "T"       // 压缩的算法类型

// 请求公共字段
    #define META_FIELD_REQUEST_MARKET               "market"            //市场
    #define META_FIELD_REQUEST_STOCK                "stock"             //股票
    #define META_FIELD_REQUEST_META_ID              "metaId"              //数据类型
    #define META_FIELD_REQUEST_PERIOD               "period"            //周期
    #define META_FIELD_REQUEST_FIELDS               "fields"            //fields
    #define META_FIELD_REQUEST_START_TIME           "startTime"         //起始时间
    #define META_FIELD_REQUEST_START_SEQ            "startSeq"          //起始序号
    #define META_FIELD_REQUEST_END_TIME             "endTime"           //结束时间
    #define META_FIELD_REQUEST_END_SEQ              "endSeq"            //结束序号
    #define META_FIELD_REQUEST_TIMETAG              "timeTag"            //结束时间 by count
    #define META_FIELD_REQUEST_SEQ                  "seq"              //结束序号 by count
    #define META_FIELD_REQUEST_COUNT                "count"            //总量
    #define META_FIELD_REQUEST_COMPRESS             "compress"         //是否用zlib压缩


// 数据公共字段
    #define META_FIELD_TIMETAG                      "G"         //时间戳
    #define META_FIELD_SEQ                          "Q"         //序号

// 数据信息
#define META_ID_META_INFO               1
    #define META_FIELD_META_INFO_MARKET             "M"         //市场
    #define META_FIELD_META_INFO_STOCK              "S"         //股票
    #define META_FIELD_META_INFO_META_ID            "I"         //数据类型
    #define META_FIELD_META_INFO_PERIOD             "P"         //周期
    #define META_FIELD_META_INFO_TYPE               "T"         //数据存储方式

// 市场交易日信息
#define META_ID_MARKET_TRADE_DATE       2
    #define META_FIELD_MARKET_TRADE_DATE_TIMETAG    "0"         //市场交易日信息，访问时code使用STOCK_SPECIAL

// 市场状态
#define META_ID_MARKET_STATUS           3
    #define META_FIELD_MARKET_STATUS_TRADE_DATE     "0"     // 当前交易日
    #define META_FIELD_MARKET_STATUS_TRADE_STATUS   "1"     // 交易状态

// 实时行情
#define META_ID_QUOTE                   1000
    #define META_FIELD_QUOTE_PRICE                  "0"         //当前价
    #define META_FIELD_QUOTE_VOLUEM                 "1"         //成交总量
    #define META_FIELD_QUOTE_AMOUNT                 "2"         //成交总额
    #define META_FIELD_QUOTE_OPEN_INTEREST          "3"         //持仓量
    #define META_FIELD_QUOTE_BID_PRICE              "4"         //委买均价
    #define META_FIELD_QUOTE_BID_VOLUME             "5"         //委买总量
    #define META_FIELD_QUOTE_ASK_PRICE              "6"         //委卖均价
    #define META_FIELD_QUOTE_ASK_VOLUME             "7"         //委卖总量
    #define META_FIELD_QUOTE_OPEN_PRICE             "8"         //开盘价
    #define META_FIELD_QUOTE_HIGH_PRICE             "9"         //最高价
    #define META_FIELD_QUOTE_LOW_PRICE              "a"         //最低价
    #define META_FIELD_QUOTE_SETTLEMENT_PRICE       "b"         //今结算
    
    // 若非特别说明，一般不使用
    #define META_FIELD_QUOTE_LAST_CLOSE_PRICE       "c"         // 前收盘

// K线
#define META_ID_KLINE                   1001
    #define META_FIELD_KLINE_OPEN_PRICE             "0"         //开盘价
    #define META_FIELD_KLINE_HIGH_PRICE             "1"         //最高价
    #define META_FIELD_KLINE_LOW_PRICE              "2"         //最低价
    #define META_FIELD_KLINE_CLOSE_PRICE            "3"         //收盘价
    #define META_FIELD_KLINE_LAST_CLOSE_PRICE       "4"         //前收盘价
    #define META_FIELD_KLINE_VOLUME                 "5"         //成交量
    #define META_FIELD_KLINE_AMOUNT                 "6"         //成交额
    #define META_FIELD_KLINE_OPEN_INETERST          "7"         //持仓量
    #define META_FIELD_KLINE_FLOAT_VOLUME           "8"         //流通盘
    #define META_FIELD_KLINE_TOTAL_VOLUME           "9"         //总股本
    #define META_FIELD_KLINE_DR                     "a"         //当日除权系数
    #define META_FIELD_KLINE_TOTAL_DR               "b"         //总的除权系数
    #define META_FIELD_KLINE_PAUSE_COUNT            "c"         //停牌周期数
    #define META_FIELD_KLINE_SETTLEMENT_PRICE       "d"         //今结算

// 逐笔成交
#define META_ID_TRANSACTION             1002
    #define META_FIELD_TRANSACTION_PRICE            "0"         //价格
    #define META_FIELD_TRANSACTION_VOLUME           "1"         //成交量
    #define META_FIELD_TRANSACTION_AMOUNT           "2"         //成交额

// 逐笔委托
#define META_ID_STEP_ORDER              1003
    #define META_FIELD_STEP_ORDER_PRICE             "0"         // 委托价
    #define META_FIELD_STEP_ORDER_VOLUME            "1"         // 委托量
    #define META_FIELD_STEP_ORDER_AMOUNT            "2"         // 委托额
    #define META_FIELD_STEP_ORDER_TYPE              "3"         // 委托类型

// 委买分档
#define META_ID_BID_QUEUE               1004
    #define META_FIELD_BID_QUEUE_PRICE              "0"         //多档委托价
    #define META_FIELD_BID_QUEUE_VOLUME             "1"         //多档委托量

// 委卖分档
#define META_ID_ASK_QUEUE               1005
    #define META_FIELD_ASK_QUEUE_PRICE              "0"         //多档委托价
    #define META_FIELD_ASK_QUEUE_VOLUME             "1"         //多档委托量

// 最优委买队列
#define META_ID_BID_ORDER_QUEUE         1006
    #define META_FIELD_BID_ORDER_QUEUE_PRICE        "0"         //多档委托价 目前只有一档，是double，不是vdouble
    #define META_FIELD_BID_ORDER_QUEUE_VOLUME       "1"         //多档委托量，vd，买1对应的

// 最优委卖队列
#define META_ID_ASK_ORDER_QUEUE         1007
    #define META_FIELD_ASK_ORDER_QUEUE_PRICE        "0"         //多档委托价 目前只有一档，是double，不是vdouble
    #define META_FIELD_ASK_ORDER_QUEUE_VOLUME       "1"         //多档委托量，vd，卖1对应的

// 主买单
#define META_ID_ACTIVE_BID_DEAL         1008
    #define META_FIELD_ACTIVE_BID_DEAL_PRICE        "0"     // 成交价（最后成交）
    #define META_FIELD_ACTIVE_BID_DEAL_VOLUME       "1"     // 成交量
    #define META_FIELD_ACTIVE_BID_DEAL_AMOUNT       "2"     // 成交额

// 主卖单
#define META_ID_ACTIVE_ASK_DEAL         1009
    #define META_FIELD_ACTIVE_ASK_DEAL_PRICE        "0"     // 成交价（最后成交）
    #define META_FIELD_ACTIVE_ASK_DEAL_VOLUME       "1"     // 成交量
    #define META_FIELD_ACTIVE_ASK_DEAL_AMOUNT       "2"     // 成交额

// 深度行情
#define META_ID_DEPTH_MARKET_DATA       1010
#define META_ID_DEPTH_MARKET_TRADINGDAY                 "0"        ///交易日
#define META_ID_DEPTH_MARKET_INSTRUMENTID               "1"        ///合约代码
#define META_ID_DEPTH_MARKET_EXCHANGEID                 "2"        ///交易所代码
#define META_ID_DEPTH_MARKET_EXCHANGEINSTID             "3"        ///合约在交易所的代码
#define META_ID_DEPTH_MARKET_LASTPRICE                  "4"        ///最新价
#define META_ID_DEPTH_MARKET_PRESETTLEMENTPRICE         "5"        ///上次结算价
#define META_ID_DEPTH_MARKET_PRECLOSEPRICE              "6"        ///昨收盘
#define META_ID_DEPTH_MARKET_PREOPENINTEREST            "7"        ///昨持仓量
#define META_ID_DEPTH_MARKET_OPENPRICE                  "8"        ///今开盘
#define META_ID_DEPTH_MARKET_HIGHESTPRICE               "9"        ///最高价
#define META_ID_DEPTH_MARKET_LOWESTPRICE                "10"        ///最低价
#define META_ID_DEPTH_MARKET_VOLUME                     "11"        ///数量
#define META_ID_DEPTH_MARKET_TURNOVER                   "12"        ///成交金额
#define META_ID_DEPTH_MARKET_OPENINTEREST               "13"        ///持仓量
#define META_ID_DEPTH_MARKET_CLOSEPRICE                 "14"        ///今收盘
#define META_ID_DEPTH_MARKET_SETTLEMENTPRICE            "15"        ///本次结算价
#define META_ID_DEPTH_MARKET_UPPERLIMITPRICE            "16"        ///涨停板价
#define META_ID_DEPTH_MARKET_LOWERLIMITPRICE            "17"        ///跌停板价
#define META_ID_DEPTH_MARKET_PREDELTA                   "18"        ///昨虚实度
#define META_ID_DEPTH_MARKET_CURRDELTA                  "19"        ///今虚实度
#define META_ID_DEPTH_MARKET_UPDATETIME                 "20"        ///最后修改时间
#define META_ID_DEPTH_MARKET_UPDATEMILLISEC             "21"        ///最后修改毫秒
#define META_ID_DEPTH_MARKET_BIDPRICE1                  "22"        ///申买价一
#define META_ID_DEPTH_MARKET_BIDVOLUME1                 "23"        ///申买量一
#define META_ID_DEPTH_MARKET_ASKPRICE1                  "24"        ///申卖价一
#define META_ID_DEPTH_MARKET_ASKVOLUME1                 "25"        ///申卖量一
#define META_ID_DEPTH_MARKET_BIDPRICE2                  "26"        ///申买价二
#define META_ID_DEPTH_MARKET_BIDVOLUME2                 "27"        ///申买量二
#define META_ID_DEPTH_MARKET_ASKPRICE2                  "28"        ///申卖价二
#define META_ID_DEPTH_MARKET_ASKVOLUME2                 "29"        ///申卖量二
#define META_ID_DEPTH_MARKET_BIDPRICE3                  "30"        ///申买价三
#define META_ID_DEPTH_MARKET_BIDVOLUME3                 "31"        ///申买量三
#define META_ID_DEPTH_MARKET_ASKPRICE3                  "32"        ///申卖价三
#define META_ID_DEPTH_MARKET_ASKVOLUME3                 "33"        ///申卖量三
#define META_ID_DEPTH_MARKET_BIDPRICE4                  "34"        ///申买价四
#define META_ID_DEPTH_MARKET_BIDVOLUME4                 "35"        ///申买量四
#define META_ID_DEPTH_MARKET_ASKPRICE4                  "36"        ///申卖价四
#define META_ID_DEPTH_MARKET_ASKVOLUME4                 "37"        ///申卖量四
#define META_ID_DEPTH_MARKET_BIDPRICE5                  "38"        ///申买价五
#define META_ID_DEPTH_MARKET_BIDVOLUME5                 "39"        ///申买量五
#define META_ID_DEPTH_MARKET_ASKPRICE5                  "40"        ///申卖价五
#define META_ID_DEPTH_MARKET_ASKVOLUME5                 "41"        ///申卖量五
#define META_ID_DEPTH_MARKET_AVERAGEPRICE               "42"        ///当日均价


// 股票列表
#define META_ID_STOCK_LIST              2000
    #define META_FIELD_STOCK_LIST_STOCK_CODE            "0"         // 股票代码
    #define META_FIELD_STOCK_LIST_STOCK_NAME            "1"         // 股票名称
    #define META_FIELD_STOCK_LIST_LAST_CLOSE            "2"         // 昨日收盘价
    #define META_FIELD_STOCK_LIST_LAST_SETTLE_PRICE     "3"         // 昨日结算
    #define META_FIELD_STOCK_LIST_LAST_OPEN_INTEREST    "4"         // 昨日持仓
    #define META_FIELD_STOCK_LIST_UP_STOP_PRICE         "5"         // 涨停价
    #define META_FIELD_STOCK_LIST_DN_STOP_PRICE         "6"         // 跌停价
    #define META_FIELD_STOCK_LIST_TOTAL_VOLUMN          "7"         // 总股本
    #define META_FIELD_STOCK_LIST_FLOAT_VOLUMN          "8"         // 流通盘

    // 脚本引擎所需filed
    #define META_FIELD_STOCK_LIST_VOLUMN_MULTIPLE       "9"         // 合约数量乘数 -- 仅期货提供该数据
    #define META_FIELD_STOCK_LIST_PRODUCT_ID            "10"        // 产品代码
    #define META_FIELD_STOCK_LIST_PRODUCT_CLASS         "11"        // 产品类型
    #define META_FIELD_STOCK_LIST_DELIVERY_YEAR         "12"        // 交割年份
    #define META_FIELD_STOCK_LIST_DELIVERY_MONTH        "13"        // 交割月
    #define META_FIELD_STOCK_LIST_MAX_MARKET_ORDER_VOL  "14"        // 市价单最大下单量
    #define META_FIELD_STOCK_LIST_MIN_MARKET_ORDER_VOL  "15"        // 市价单最小下单量
    #define META_FIELD_STOCK_LIST_MAX_LIMIT_ORDER_VOL   "16"        // 限价单最大下单量
    #define META_FIELD_STOCK_LIST_MIN_LIMIT_ORDER_VOL   "17"        // 限价单最小下单量
    #define META_FIELD_STOCK_LIST_PRICE_TICK            "18"        // 最小变动价位
    #define META_FIELD_STOCK_LIST_CREATE_DATE           "19"        // 创建日
    #define META_FIELD_STOCK_LIST_OPEN_DATE             "20"        // 上市日
    #define META_FIELD_STOCK_LIST_EXPIRE_DATE           "21"        // 到期日
    #define META_FIELD_STOCK_LIST_START_DELIV_DATE      "22"        // 开始交割日
    #define META_FIELD_STOCK_LIST_END_DELIV_DATE        "23"        // 结束交割日
    #define META_FIELD_STOCK_LIST_INST_LIFE_PHASE       "24"        // 合约生命周期状态
    #define META_FIELD_STOCK_LIST_IS_TRADING            "25"        // 当前是否交易
    #define META_FIELD_STOCK_LIST_POSITION_TYPE         "26"        // 持仓类型
    #define META_FIELD_STOCK_LIST_POSITION_DATE_TYPE    "27"        // 持仓日期类型
    #define META_FIELD_STOCK_LIST_LONG_MARGIN_RATIO     "28"        // 多头保证金率
    #define META_FIELD_STOCK_LIST_SHORT_MARGIN_RATIO    "29"        // 空头保证金率
    #define META_FIELD_STOCK_LIST_MAIN_CONTRACT         "30"        // 是否主力合约
    #define META_FIELD_STOCK_LIST_RECENT_CONTRACT       "31"        // 是否近月合约    
    #define META_FIELD_STOCK_LIST_CODE_IN_RZRK          "32"        // 合约在RZRK的代码
    #define META_FIELD_STOCK_LIST_CODE_IN_EXCHANGE      "33"        // 合约在交易所的代码
    #define META_FIELD_STOCK_LIST_IS_NIGHT_FUTURE       "34"        // 是否夜盘合约
    #define META_FIELD_STOCK_LIST_AMOUNT_INTEREST       "35"        // 利息额


// 板块信息
#define META_ID_SECTOR_LIST             2001
    #define META_FIELD_SECTOR_NAME                      "0"         // 板块名称
    #define META_FIELD_SECTOR_STOCKS                    "1"         // 板块成分股
        #define META_FIELD_SECTOR_STOCK_MARKET          "2"         // 板块成分股市场代码
        #define META_FIELD_SECTOR_STOCK_CODE            "3"         // 板块成分股股票代码
        #define META_FIELD_SECTOR_STOCK_WEIGHT          "4"         // 板块成分股绝对权重
    #define META_FIELD_SECTOR_CATEGORY                  "5"         // 板块类别



// 实时行情（level-1 + 期货）
#define META_ID_L1_QUOTE                   3000
#define META_FIELD_L1_QUOTE_PRICE                  "0"         //当前价
#define META_FIELD_L1_QUOTE_VOLUEM                 "1"         //成交总量
#define META_FIELD_L1_QUOTE_AMOUNT                 "2"         //成交总额
#define META_FIELD_L1_QUOTE_OPEN_INTEREST          "3"         //持仓量
#define META_FIELD_L1_QUOTE_LAST_SETTLEMENT_PRICE  "4"         //昨结算（股票没有，塞0）
#define META_FIELD_L1_QUOTE_TRANSACTION_NUMBER     "5"         //成交笔数（期货没有，单独计算）
#define META_FIELD_L1_QUOTE_PE1                    "6"         //市盈率1
#define META_FIELD_L1_QUOTE_PE2                    "7"         //市盈率2
#define META_FIELD_L1_QUOTE_OPEN_PRICE             "8"         //开盘价
#define META_FIELD_L1_QUOTE_HIGH_PRICE             "9"         //最高价
#define META_FIELD_L1_QUOTE_LOW_PRICE              "a"         //最低价
#define META_FIELD_L1_QUOTE_SETTLEMENT_PRICE       "b"         //今结算（股票没有，塞0）
#define META_FIELD_L1_QUOTE_LAST_CLOSE_PRICE       "c"         //前收盘
#define META_FIELD_L1_BID_QUEUE_PRICE              "d"         //多档委买价 - 用五档委买价计算得到
#define META_FIELD_L1_BID_QUEUE_VOLUME             "e"         //多档委买量 - 用五档委买量计算得到
#define META_FIELD_L1_ASK_QUEUE_PRICE              "f"         //多档委卖价 - 用五档委卖价计算得到
#define META_FIELD_L1_ASK_QUEUE_VOLUME             "g"         //多档委卖量 - 用五档委卖量计算得到


// K线（level-1 + 期货）
#define META_ID_L1_KLINE                   3001
#define META_FIELD_L1_KLINE_OPEN_PRICE             "0"         //开盘价
#define META_FIELD_L1_KLINE_HIGH_PRICE             "1"         //最高价
#define META_FIELD_L1_KLINE_LOW_PRICE              "2"         //最低价
#define META_FIELD_L1_KLINE_CLOSE_PRICE            "3"         //收盘价
#define META_FIELD_L1_KLINE_LAST_CLOSE_PRICE       "4"         //前收盘价
#define META_FIELD_L1_KLINE_VOLUME                 "5"         //成交量
#define META_FIELD_L1_KLINE_AMOUNT                 "6"         //成交额
#define META_FIELD_L1_KLINE_OPEN_INETERST          "7"         //持仓量
#define META_FIELD_L1_KLINE_FLOAT_VOLUME           "8"         //流通盘
#define META_FIELD_L1_KLINE_TOTAL_VOLUME           "9"         //总股本
#define META_FIELD_L1_KLINE_DR                     "a"         //当日除权系数
#define META_FIELD_L1_KLINE_TOTAL_DR               "b"         //总的除权系数
#define META_FIELD_L1_KLINE_PAUSE_COUNT            "c"         //停牌周期数
#define META_FIELD_L1_KLINE_SETTLEMENT_PRICE       "d"         //今结算

#define P4DELTA_AMOUNT_SHIFT_BITS                   20           //成交额压缩的时候需要移位，分成两个字段进行
#define P4DELTA_AMOUNT_SHIFT_TAIL                  "at"          //成交额压缩移位后尾数部分

#define META_ID_EX_DIVIDEND             4000
#define META_ID_EX_DIVIDEND_INTEREST            "0"         //每股股利（税前，元）
#define META_ID_EX_DIVIDEND_STOCK_BONUS         "1"         //每股红股（股）
#define META_ID_EX_DIVIDEND_STOCK_GIFT          "2"         //每股转增股本（股）
#define META_ID_EX_DIVIDEND_ALLOT_NUM           "3"         //每股配股数（股）
#define META_ID_EX_DIVIDEND_ALLOT_PRICE         "4"         //配股价格（元）
#define META_ID_EX_DIVIDEND_STOCK_GUGAI         "5"         //是否股改

#define META_ID_CAPITAL_STOCK           4001
#define META_ID_CAPITAL_STOCK_FLOAT_VOLUMN      "0"    //流通A股
#define META_ID_CAPITAL_STOCK_TOTAL_VOLUMN      "1"    //总股本

#define META_ID_BUYSELL_VOL             4002           //内外盘
#define META_FIELD_BUYSELL_VOL_BUY                 "0"    //内盘
#define META_FIELD_BUYSELL_VOL_SELL                "1"    //外盘

#ifdef __cplusplus
    }
#endif

#endif
