/**
 *  单体模板
 *
 *  @author zhangyi
 *  @since  2012-06-05
 */

#ifndef SERVER3_BASE_UTILS_TTSINGLETONT_H
#define SERVER3_BASE_UTILS_TTSINGLETONT_H

#include <boost/thread/recursive_mutex.hpp>

namespace utils
{
    template <class T>
    class CSingletonT
    {
    public:
        static T *instance();

        static void release();

    protected:

        static T *s_pInstance;
    };

}

#endif
