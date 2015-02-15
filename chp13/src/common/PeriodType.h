/**
* 周期定义
*
* @author   xujun
* @date     2010-10-22
* @time     19:02
* @version  1.0
*/
#include "type.h"

#ifndef PERIODTYPE_H_
#define PERIODTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PERIOD_TYPE_CONTINUE            (r_int64)(0)                     // 连续值
#define PERIOD_TYPE_ONE_SECOND          (r_int64)(1000)                  // 1秒
#define PERIOD_TYPE_FIVE_SECOND         (r_int64)(1000 * 5)              // 5秒
#define PERIOD_TYPE_ONE_MINITUE         (r_int64)(1000 * 60)             // 1分钟
#define PERIOD_TYPE_FIVE_MINITUE        (r_int64)(1000 * 60 * 5)         // 5分钟
#define PERIOD_TYPE_ONE_HOUR            (r_int64)(1000 * 60 * 60)        // 1小时
#define PERIOD_TYPE_ONE_DAY             (r_int64)(1000 * 60 * 60 * 24)   // 1天

#ifdef __cplusplus
}
#endif

#endif

