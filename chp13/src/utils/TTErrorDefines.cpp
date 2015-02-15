#include "common/Stdafx.h"
#include "utils/TTErrorDefines.h"
#include "utils/STLHelper.h"

namespace utils
{
    std::map<unsigned int, std::string> genBaseErrorMap()
    {
        std::map<unsigned int, std::string> errorMap;

#define TT_ERROR_ADD(key, value) utils::replace(errorMap, (unsigned int)key, std::string(value))

        TT_ERROR_ADD(TT_ERROR_SUCCESS, "成功");
        TT_ERROR_ADD(TT_ERROR_NET_DISCONNECTED, "未建立连接");
        TT_ERROR_ADD(TT_ERROR_NOT_LOGINED, "未登录");
        TT_ERROR_ADD(TT_ERROR_NOT_INITIALIZED, "初始化未完成");
        TT_ERROR_ADD(TT_ERROR_TIME_OUT, "超时");
        TT_ERROR_ADD(TT_ERROR_NOT_FIND_ACCOUNT, "未找到账号");
        TT_ERROR_ADD(TT_ERROR_NOT_FIND_FUNCTION, "未找到处理函数");
        TT_ERROR_ADD(TT_ERROR_INVALID_REQUEST_PARAM, "无效的请求参数");
#undef TT_ERROR_ADD

        return errorMap;
    }
}