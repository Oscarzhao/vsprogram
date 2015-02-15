/**
 *  在单例基础上实现一个shadow copy
 *
 *  @author zhangyi
 *  @since  2012-05-05
 */

#include <utils/CShadowCopyT.h>
#include <utils/CSingletonT.hpp>

namespace utils
{
    template <class T>
    T * CShadowCopyT<T>::shadowCopy()
    {
        if (NULL == s_pShadowInstance)
        {
            boost::recursive_mutex::scoped_lock guard(s_shadowMutex);
            s_pShadowInstance = new T();
            s_pShadowInstance->backup(*CSingletonT<T>::instance());
        }
        return s_pShadowInstance;
    }

    template <class T>
    void CShadowCopyT<T>::applyShadow()
    {
        if (NULL != s_pShadowInstance)
        {
            boost::recursive_mutex::scoped_lock guard(s_shadowMutex);
            CSingletonT<T>::instance()->restore(*s_pShadowInstance);
        }
    }

    template <class T>
    void CShadowCopyT<T>::discardShadow()
    {
        if (NULL != s_pShadowInstance)
        {
            boost::recursive_mutex::scoped_lock guard(s_shadowMutex);
            s_pShadowInstance->backup(*CSingletonT<T>::instance());
        }
    }

    template <class T>
    void CShadowCopyT<T>::releaseShadow()
    {
        if (NULL != s_pShadowInstance)
        {
            boost::recursive_mutex::scoped_lock guard(s_shadowMutex);
            delete s_pShadowInstance;
        }
    }

    template <class T>
    void  CShadowCopyT<T>::backup(const CShadowCopyT<T>& right)
    {
        *this = right;
    }

    template <class T>
    void  CShadowCopyT<T>::restore(const CShadowCopyT<T>& right)
    {
        *this = right;
    }

#define IMPL_SHADOWCOPY_CLASS(subClass)  \
    namespace utils \
    {   \
        template<> subClass * CShadowCopyT<subClass>::s_pShadowInstance = NULL; \
        template<> boost::recursive_mutex CShadowCopyT<subClass>::s_shadowMutex;    \
    }   \
    template class utils::CShadowCopyT<subClass>;  \
    IMPL_SINGLETON_CLASS(subClass)
}
