/**
* 该文件中的数值是参考上期接口
* 
* @author   xujun
* @date     2012-2-28
* @time     13:42
* @version  1.0
*/
#ifndef TRADER_H_
#define TRADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PRODUCT_INFO "rzrk_pc"

    // 订单类型
    const static r_int32 TRADER_ORDER_MARKET        = '1';        // 任意价
    const static r_int32 TRADER_ORDER_LIMIT         = '2';        // 限价
    const static r_int32 TRADER_ORDER_BEST          = '3';        // 最优价

    // openClose:  开平标记
    const static r_int32 TRADER_TAG_OPEN            = '0';        // 开仓
    const static r_int32 TRADER_TAG_CLOSE           = '1';        // 平仓  
    const static r_int32 TRADER_TAG_FORCECLOSE      = '2';        // 强平
    const static r_int32 TRADER_TAG_CLOSETODAY      = '3';        // 平今
    const static r_int32 TRADER_TAG_CLOSEYESTODAY   = '4';        // 平昨
    const static r_int32 TRADER_TAG_FORCEOFF        = '5';        // 强减
    const static r_int32 TRADER_TAG_LOCALFORCECLOSE = '6';        // 本地强平

    // 买卖方向, 又名多空
    const static r_int32 TRADER_DIRECTION_BUY       = '0';        // 买(多)
    const static r_int32 TRADER_DIRECTION_SELL      = '1';        // 卖(空)

    // 投保标记
    const static r_int32 TRADER_HEDGE_SPECULATION   = '1';      // 投机
    const static r_int32 TRADER_HEDGE_ARBITRAGE     = '2';      // 套利
    const static r_int32 TRADER_HEDGE_HEDGE         = '3';      // 套保

    // 主推数据类型
    const static r_int32 TRADER_PUSH_ORDER              = 1;    // 委托
    const static r_int32 TRADER_PUSH_BUSINESS           = 2;    // 成交
    const static r_int32 TRADER_PUSH_INSTRUMENT_STATUS  = 3;    // 合约状态
    const static r_int32 TRADER_PUSH_ERROR_INSERT       = 4;    // 报单录入错误
    const static r_int32 TRADER_PUSH_ERROR_ACTION       = 5;    // 报单操作错误
    const static r_int32 TRADER_PUSH_TASK               = 6;    // 任务
    const static r_int32 TRADER_PUSH_NET_STATUS         = 7;    // 网络状态
    const static r_int32 TRADER_PUSH_MONEY_TRASFER      = 8;    // 资金转账
    const static r_int32 TRADER_PUSH_TRADING_NOTICE     = 9;    // 交易通知

    // 网络状态
    const static r_int32 BROKER_CONNECT_SUCCESS         = 0;    // 连接正常
    const static r_int32 BROKER_CONNECT_DISCONNECT      = -1;    // 未建立连接
    const static r_int32 BROKER_CONNECT_WAITING_CONNECT = -2;    // 等待建立连接
    const static r_int32 BROKER_CONNECT_CONNECTING      = -3;    // 正在连接中
    const static r_int32 BROKER_CONNECT_ERROR           = -4;    // 连接失败

    enum TRANS_TYPE
    {
        TRANS_DIRECT_BANK_TO_FUTURE = '1',     // 银行转期货
        TRANS_DIRECT_FUTURE_TO_BANK = '2'      // 期货转银行
    };

    ///上海期货交易所
#define THOST_FTDC_EXCHAGE_SHFE "SHFE"
    ///郑州商品交易所
#define THOST_FTDC_EXCHAGE_CZCE "CZCE"
    ///大连商品交易所
#define THOST_FTDC_EXCHAGE_DCE "DCE"
    ///中国金融期货交易所
#define THOST_FTDC_EXCHAGE_CFFEX "CFFEX"

    /************************************************************************/
    /* 报单状态                                                            */
    /************************************************************************/
        ///全部成交
    #define TRADER__OST_AllTraded '0'
        ///部分成交还在队列中
    #define TRADER__OST_PartTradedQueueing '1'
        ///部分成交不在队列中
    #define TRADER__OST_PartTradedNotQueueing '2'
        ///未成交还在队列中
    #define TRADER__OST_NoTradeQueueing '3'
        ///未成交不在队列中
    #define TRADER__OST_NoTradeNotQueueing '4'
        ///撤单
    #define TRADER__OST_Canceled '5'
        ///未知
    #define TRADER__OST_Unknown 'a'
        ///尚未触发
    #define TRADER__OST_NotTouched 'b'
        ///已触发
    #define TRADER__OST_Touched 'c'

        ///全部成交
    #define THOST_FTDC_OST_AllTraded '0'
        ///部分成交还在队列中
    #define THOST_FTDC_OST_PartTradedQueueing '1'
        ///部分成交不在队列中
    #define THOST_FTDC_OST_PartTradedNotQueueing '2'
        ///未成交还在队列中
    #define THOST_FTDC_OST_NoTradeQueueing '3'
        ///未成交不在队列中
    #define THOST_FTDC_OST_NoTradeNotQueueing '4'
        ///撤单
    #define THOST_FTDC_OST_Canceled '5'
        ///未知
    #define THOST_FTDC_OST_Unknown 'a'
        ///尚未触发
    #define THOST_FTDC_OST_NotTouched 'b'
        ///已触发
    #define THOST_FTDC_OST_Touched 'c'

    const static int THOST_FTDC_OF_XTClose =  '7';  //内部定义的平仓(不区分昨仓和今仓）

    inline bool isFinalOrderStatus(int orderStatus, int orderSubmitStatus)
    {
        static r_int32 s_statusArray[] = {THOST_FTDC_OST_AllTraded, THOST_FTDC_OST_Canceled, \
            THOST_FTDC_OST_PartTradedNotQueueing, THOST_FTDC_OST_NoTradeNotQueueing};
        static set<r_int32> s_status(s_statusArray, s_statusArray + sizeof(s_statusArray));

        if (s_status.find(orderStatus) != s_status.end())
            return true;

        if (orderSubmitStatus > '3') 
            return true;

        return false;
    }

    // 下单系统所有错误代码

#define NONE     0 // 综合交易平台：正确
#define INVALID_DATA_SYNC_STATUS     1 // 综合交易平台：不在已同步状态
#define INCONSISTENT_INFORMATION     2 // 综合交易平台：会话信息不一致
#define INVALID_LOGIN     3 // 综合交易平台：不合法的登录
#define USER_NOT_ACTIVE     4 // 综合交易平台：用户不活跃
#define DUPLICATE_LOGIN     5 // 综合交易平台：重复的登录
#define NOT_LOGIN_YET     6 // 综合交易平台：还没有登录
#define NOT_INITED     7 // 综合交易平台：还没有初始化
#define FRONT_NOT_ACTIVE     8 // 综合交易平台：前置不活跃
#define NO_PRIVILEGE     9 // 综合交易平台：无此权限
#define CHANGE_OTHER_PASSWORD     10 // 综合交易平台：修改别人的口令
#define USER_NOT_FOUND     11 // 综合交易平台：找不到该用户
#define BROKER_NOT_FOUND     12 // 综合交易平台：找不到该经纪公司
#define INVESTOR_NOT_FOUND     13 // 综合交易平台：找不到投资者
#define OLD_PASSWORD_MISMATCH     14 // 综合交易平台：原口令不匹配
#define BAD_FIELD     15 // 综合交易平台：报单字段有误
#define INSTRUMENT_NOT_FOUND     16 // 综合交易平台：找不到合约
#define INSTRUMENT_NOT_TRADING     17 // 综合交易平台：合约不能交易
#define NOT_EXCHANGE_PARTICIPANT     18 // 综合交易平台：经纪公司不是交易所的会员
#define INVESTOR_NOT_ACTIVE     19 // 综合交易平台：投资者不活跃
#define NOT_EXCHANGE_CLIENT     20 // 综合交易平台：投资者未在交易所开户
#define NO_VALID_TRADER_AVAILABLE     21 // 综合交易平台：该交易席位未连接到交易所
#define DUPLICATE_ORDER_REF     22 // 综合交易平台：报单错误：不允许重复报单
#define BAD_ORDER_ACTION_FIELD     23 // 综合交易平台：错误的报单操作字段
#define DUPLICATE_ORDER_ACTION_REF     24 // 综合交易平台：撤单已报送，不允许重复撤单
#define ORDER_NOT_FOUND     25 // 综合交易平台：撤单找不到相应报单
#define INSUITABLE_ORDER_STATUS     26 // 综合交易平台：报单已全成交或已撤销，不能再撤
#define UNSUPPORTED_FUNCTION     27 // 综合交易平台：不支持的功能
#define NO_TRADING_RIGHT     28 // 综合交易平台：没有报单交易权限
#define CLOSE_ONLY     29 // 综合交易平台：只能平仓
#define OVER_CLOSE_POSITION     30 // 综合交易平台：平仓量超过持仓量
#define INSUFFICIENT_MONEY     31 // 综合交易平台：资金不足
#define DUPLICATE_PK     32 // 综合交易平台：主键重复
#define CANNOT_FIND_PK     33 // 综合交易平台：找不到主键
#define CAN_NOT_INACTIVE_BROKER     34 // 综合交易平台：设置经纪公司不活跃状态失败
#define BROKER_SYNCHRONIZING     35 // 综合交易平台：经纪公司正在同步
#define BROKER_SYNCHRONIZED     36 // 综合交易平台：经纪公司已同步
#define SHORT_SELL     37 // 综合交易平台：现货交易不能卖空
#define INVALID_SETTLEMENT_REF     38 // 综合交易平台：不合法的结算引用
#define CFFEX_NETWORK_ERROR     39 // 综合交易平台：中金所网络连接失败
#define CFFEX_OVER_REQUEST     40 // 综合交易平台：中金所未处理请求超过许可数
#define CFFEX_OVER_REQUEST_PER_SECOND     41 // 综合交易平台：中金所每秒发送请求数超过许可数
#define SETTLEMENT_INFO_NOT_CONFIRMED     42 // 综合交易平台：结算结果未确认
#define DEPOSIT_NOT_FOUND     43 // 综合交易平台：没有对应的入金记录
#define EXCHANG_TRADING     44 // 综合交易平台：交易所已经进入连续交易状态
#define PARKEDORDER_NOT_FOUND     45 // 综合交易平台：找不到预埋（撤单）单
#define PARKEDORDER_HASSENDED     46 // 综合交易平台：预埋（撤单）单已经发送
#define PARKEDORDER_HASDELETE     47 // 综合交易平台：预埋（撤单）单已经删除
#define INVALID_INVESTORIDORPASSWORD     48 // 综合交易平台：无效的投资者或者密码
#define INVALID_LOGIN_IPADDRESS     49 // 综合交易平台：不合法的登录IP地址
#define OVER_CLOSETODAY_POSITION     50 // 综合交易平台：平今仓位不足
#define OVER_CLOSEYESTERDAY_POSITION     51 // 综合交易平台：平昨仓位不足
#define BROKER_NOT_ENOUGH_CONDORDER     52 // 综合交易平台：经纪公司没有足够可用的条件单数量
#define INVESTOR_NOT_ENOUGH_CONDORDER     53 // 综合交易平台：投资者没有足够可用的条件单数量
#define BROKER_NOT_SUPPORT_CONDORDER     54 // 综合交易平台：经纪公司不支持条件单
#define RESEND_ORDER_BROKERINVESTOR_NOTMATCH     55 // 综合交易平台：重发未知单经济公司/投资者不匹配
#define AUTH_FAILED        63   //CTP:客户端认证失败
#define NOT_AUTHENT        64   //CTP:客户端未认证
#define LOGIN_FORBIDDEN    75   //CTP:连续登录失败次数超限，登录被禁止
#define NEED_RETRY         90  //CTP：查询未就绪，请稍后重试
#define MARKET_CLOSED     100 // 交易市场状态停止
#define TIME_NO_ENTRUSTING  101 // 当前时间不允许委托
#define SEND_INSTITUTION_CODE_ERROR     1000 // 银期转账：发送机构代码错误
#define NO_GET_PLATFORM_SN     1001 // 银期转账：取平台流水号错误
#define ILLEGAL_TRANSFER_BANK     1002 // 银期转账：不合法的转账银行
#define ALREADY_OPEN_ACCOUNT     1003 // 银期转账：已经开户
#define NOT_OPEN_ACCOUNT     1004 // 银期转账：未开户
#define PROCESSING     1005 // 银期转账：处理中
#define OVERTIME     1006 // 银期转账：交易超时
#define RECORD_NOT_FOUND     1007 // 银期转账：找不到记录
#define NO_FOUND_REVERSAL_ORIGINAL_TRANSACTION     1008 // 银期转账：找不到被冲正的原始交易
#define CONNECT_HOST_FAILED     1009 // 银期转账：连接主机失败
#define SEND_FAILED     1010 // 银期转账：发送失败
#define LATE_RESPONSE     1011 // 银期转账：迟到应答
#define REVERSAL_BANKID_NOT_MATCH     1012 // 银期转账：冲正交易银行代码错误
#define REVERSAL_BANKACCOUNT_NOT_MATCH     1013 // 银期转账：冲正交易银行账户错误
#define REVERSAL_BROKERID_NOT_MATCH     1014 // 银期转账：冲正交易经纪公司代码错误
#define REVERSAL_ACCOUNTID_NOT_MATCH     1015 // 银期转账：冲正交易资金账户错误
#define REVERSAL_AMOUNT_NOT_MATCH     1016 // 银期转账：冲正交易交易金额错误
#define DB_OPERATION_FAILED     1017 // 银期转账：数据库操作错误
#define SEND_ASP_FAILURE     1018 // 银期转账：发送到交易系统失败
#define NOT_SIGNIN     1019 // 银期转账：没有签到
#define ALREADY_SIGNIN     1020 // 银期转账：已经签到
#define AMOUNT_OR_TIMES_OVER     1021 // 银期转账：金额或次数超限
#define NOT_IN_TRANSFER_TIME     1022 // 银期转账：这一时间段不能转账
#define BANK_SERVER_ERROR     1023 // 银行主机错
#define BANK_SERIAL_IS_REPEALED     1024 // 银期转账：银行已经冲正
#define BANK_SERIAL_NOT_EXIST     1025 // 银期转账：银行流水不存在
#define NOT_ORGAN_MAP     1026 // 银期转账：机构没有签约
#define EXIST_TRANSFER     1027 // 银期转账：存在转账，不能销户
#define BANK_FORBID_REVERSAL     1028 // 银期转账：银行不支持冲正
#define DUP_BANK_SERIAL     1029 // 银期转账：重复的银行流水
#define FBT_SYSTEM_BUSY     1030 // 银期转账：转账系统忙，稍后再试
#define MACKEY_SYNCING     1031 // 银期转账：MAC密钥正在同步
#define ACCOUNTID_ALREADY_REGISTER     1032 // 银期转账：资金账户已经登记
#define BANKACCOUNT_ALREADY_REGISTER     1033 // 银期转账：银行账户已经登记
#define DUP_BANK_SERIAL_REDO_OK     1034 // 银期转账：重复的银行流水,重发成功
#define NO_VALID_BANKOFFER_AVAILABLE     2000 // 综合交易平台：该报盘未连接到银行
#define PASSWORD_MISMATCH     2001 // 综合交易平台：资金密码错误
#define DUPLATION_BANK_SERIAL     2004 // 综合交易平台：银行流水号重复
#define DUPLATION_OFFER_SERIAL     2005 // 综合交易平台：报盘流水号重复
#define SERIAL_NOT_EXSIT     2006 // 综合交易平台：被冲正流水不存在(冲正交易)
#define SERIAL_IS_REPEALED     2007 // 综合交易平台：原流水已冲正(冲正交易)
#define SERIAL_MISMATCH     2008 // 综合交易平台：与原流水信息不符(冲正交易)
#define IdentifiedCardNo_MISMATCH     2009 // 综合交易平台：证件号码或类型错误
#define ACCOUNT_NOT_FUND     2011 // 综合交易平台：资金账户不存在
#define ACCOUNT_NOT_ACTIVE     2012 // 综合交易平台：资金账户已经销户
#define NOT_ALLOW_REPEAL_BYMANUAL     2013 // 综合交易平台：该交易不能执行手工冲正
#define AMOUNT_OUTOFTHEWAY     2014 // 综合交易平台：转帐金额错误
#define WAITING_OFFER_RSP     999999 // 综合交易平台：等待银期报盘处理结果


// 自定义错误
#define DISCONNECT_ERROR 10000  // 网络断开
#define LOGIN_TIMEOUT   10001   // 登录超时
#define INSTANCE_OVER_MAX 10002 // 超过CTP对象最大上限
#define FLOW_LIMIT_ERROR 10003  // 流速限制
#define UNKNOWN_ERROR 10004  // 登录未知错误

#ifdef __cplusplus
}
#endif

#endif