/**
* 网络命令
*
* @author   huangjian
* @date     2011-12-18
*/


#ifndef __NET_COMMAND__
#define __NET_COMMAND__


enum NetCommand
{
    NET_CMD_UNKOWN              = 0,
    NET_CMD_KEEPALIVE           = 1,
    NET_CMD_QUOTER              = 2,
    NET_CMD_RPC                 = 3,    // 客户端向服务器请求
    NET_CMD_NOTIFICATON         = 4,
    NET_CMD_VSS_MARKET_STATUS   = 5,    //VSS推送的市场状态
    NET_CMD_VSS_QUOTE           = 6,    //VSS推送的原始行情数据
    NET_CMD_COMMODITY_FUTURE    = 7,    //商品期货
    NET_CMD_KEEPALIVE_RESPONSE  = 8,
};

#endif
