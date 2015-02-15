/**
 *  错误号
 *
 *  @author xujun
 *  @since  2012-10-8
 */
#ifndef _TTErrorDefines_2012_10_8_h__
#define _TTErrorDefines_2012_10_8_h__

#include <map>
#include <string>

/************************************************************************/
/* 系统错误(100000以下, 预留)                                           */
/************************************************************************/

// 成功
#define TT_ERROR_SUCCESS 0



/************************************************************************/
/* 通用逻辑错误(20,0000 - 30,0000)                                      */
/************************************************************************/
// 未建立连接
#define TT_ERROR_NET_DISCONNECTED 200000
// 未登录
#define TT_ERROR_NOT_LOGINED 200001
// 初始化未完成
#define TT_ERROR_NOT_INITIALIZED 200002
// 超时
#define TT_ERROR_TIME_OUT 200003
// 未找到账号
#define TT_ERROR_NOT_FIND_ACCOUNT 200004
// 未找到处理函数
#define TT_ERROR_NOT_FIND_FUNCTION 200005
// 请求参数有误
#define TT_ERROR_INVALID_REQUEST_PARAM 200006

// 默认错误号
#define TT_ERROR_DEFAULT 300000

namespace utils
{
    std::map<unsigned int, std::string> genBaseErrorMap();
}

#endif // TTErrorDefines_h__