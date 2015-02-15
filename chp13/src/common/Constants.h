/**
* 常量 的定义
*
* @author   zld
* @date     2012-01-06
*/

#ifndef CONSTANT_H
#define CONSTANT_H

#include "bson/src/bson.h"

namespace constants
{

#define ENCRYPTION_NONE 0
#define ENCRYPTION_SHA  1

#define COMPRESS_NONE 0
#define COMPRESS_ZLIB 1
#define COMPRESS_DOUBLE_ZLIB 2//双向压缩，即如果客户端是压缩的，则要求服务端对该connection的所有推送、返回都使用zlib压缩
#define COMPRESS_LZMA 3
#define COMPRESS_DOUBLE_LZMA 4//双向压缩，即如果客户端是压缩的，则要求服务端对该connection的所有推送、返回都使用lzma压缩
}

#endif // CONSTANT_H