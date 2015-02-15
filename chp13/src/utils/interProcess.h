/**
 *  进程间同步和互斥的一些简单实现
 *
 *  @author zhangyi
 *  @since  2012-08-12
 */

#ifndef SERVER3_BASE_UTILS_INTERPROCESS_H
#define SERVER3_BASE_UTILS_INTERPROCESS_H

namespace utils
{
    //////////////////////////////////////////////////////////////////////////
    // 1. 条件变量
    //////////////////////////////////////////////////////////////////////////

    static const char * const TT_COND_TTSERVICE_READY = "ThinkTrader.cond.ttservice.ready";

/*
    void condNotify(const char * szCondition);
    void condWait(const char * szCondition);
*/
}

#endif
