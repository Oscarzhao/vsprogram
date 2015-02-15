/**
 *  编码转换函数
 *
 *  @author zoubailiu
 *  @since  2012-09-25
 */

#ifndef __SERVER3_BASE_UTILS_CONVFUNC_H__
#define __SERVER3_BASE_UTILS_CONVFUNC_H__
#include <string>

namespace utils
{
    int safeGbk2Utf8(std::string &strGbk);

    int safeUtf82Gbk(std::string &strUtf8);

    int safeFormatConvert(std::string & str, const char * const szSrcEncoding, const char * const szDstEncoding);

    // '\uxxxx' -> string
    int safeUFormatToString(std::string & str);
}

#endif
