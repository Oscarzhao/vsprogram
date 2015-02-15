/**
 *  单体模板
 *
 *  @author zhangyi
 *  @since  2012-06-05
 */

#include <utils/CSingletonT.h>

namespace utils
{
    template <class T>
    T * CSingletonT<T>::instance()
    {
        if (NULL == s_pInstance)
        {
            s_pInstance = new T();
        }
        return s_pInstance;
    }

    template <class T>
    void CSingletonT<T>::release()
    {
        if (NULL != s_pInstance)
        {
            delete s_pInstance;
            s_pInstance = NULL;
        }
    }

#define IMPL_SINGLETON_CLASS(subClass)  \
    namespace utils \
    {   \
        template<> subClass * CSingletonT<subClass>::s_pInstance = NULL;   \
    }\
    template class utils::CSingletonT<subClass>
}
