/**
 *  在单例基础上实现一个shadow copy
 *
 *  @author zhangyi
 *  @since  2012-05-05
 */

#ifndef SERVER3_BASE_COMMON_CSHADOWCOPYT_H
#define SERVER3_BASE_COMMON_CSHADOWCOPYT_H

#include <utils/CSingletonT.h>

namespace utils
{
    template <class T>
    class CShadowCopyT : public CSingletonT<T>
    {
    public:
        virtual ~CShadowCopyT() {};
        virtual void backup(const CShadowCopyT<T>& right);
        virtual void restore(const CShadowCopyT<T>& right);

        static T *shadowCopy();

        static void applyShadow();

        static void discardShadow();

        static void releaseShadow();

    protected:

        static T * s_pShadowInstance;
        static boost::recursive_mutex s_shadowMutex;
    };
}

#endif
