
/**
*  操作系统相关信息获取
*
*  @uathor zld
*  @since  2013-06-05
*/

#ifndef SERVER5_BASE_UTILS_SYSINFO_H
#define SERVER5_BASE_UTILS_SYSINFO_H

#include <string>
#include <list>
using namespace std;

namespace utils
{
    string getLocalIP();
    string getLocalMac();

    // 逐渐废弃
    string getMac();
}

#endif
