/**
* meta id 的定义
*
* @author   huangjian
* @date     2011-12-18
*/


#ifndef __DATA_TYPE__
#define __DATA_TYPE__

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef std::vector<BYTE> ByteArray;

    typedef enum {
        FIELD_DATA_INT      = 0,
        FIELD_DATA_LONG     = 1,
        FIELD_DATA_DOUBLE   = 2,
        FIELD_DATA_STRING   = 3,
        FIELD_DATA_VINT     = 4,
        FIELD_DATA_VLONG    = 5,
        FIELD_DATA_VDOUBLE  = 6,
        FIELD_DATA_VSTRING  = 7,
    } FieldDataType;

#ifdef __cplusplus
}
#endif

#endif