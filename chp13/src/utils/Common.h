/**
* C++公用头文件
* 
* @author   xujun
* @date     2010-10-22
* @time     18:37
* @version  1.0
*/


#ifndef COMMON_H__
#define COMMON_H__

#include "common/type.h"

#include <assert.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#if defined(__osf__)
// Tru64 lacks stdint.h, but has inttypes.h which defines a superset of
// what stdint.h would define.
#include <inttypes.h>
#elif !defined(_MSC_VER)
#include <stdint.h>
#endif

namespace std {}

//////////////////////////////////////////////////////////////////////////
/// dll exports, modify from proto buffer common.h
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
    #ifdef TT_EXPORTS
    #define TT_EXPORT __declspec(dllexport)
    #else
    #define TT_EXPORT __declspec(dllimport)
    #endif
#else
    #define TT_EXPORT
#endif

//////////////////////////////////////////////////////////////////////////
/// byte order, modify from IceUtil/Config.h
//////////////////////////////////////////////////////////////////////////
#ifdef __GLIBC__
# include <endian.h>
#endif

#if defined(__i386)     || defined(_M_IX86) || defined(__x86_64)  || \
    defined(_M_X64)     || defined(_M_IA64) || defined(__alpha__) || \
    defined(__MIPSEL__) || (defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN))
#   define TT_LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) || defined(__hppa)      || \
    defined(__ppc__) || defined(__powerpc) || defined(_ARCH_COM) || \
    defined(__MIPSEB__) || (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN))
#   define TT_BIG_ENDIAN
#else
#   error "Unknown architecture"
#endif


using namespace std;

#endif  // COMMON_H__
