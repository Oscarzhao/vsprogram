/**
 *  通用函数
 *
 *  @author zhangyi
 *  @since  2012-08-10
 */

#ifndef SERVER3_BASE_UTILS_COMMONFUNC_H
#define SERVER3_BASE_UTILS_COMMONFUNC_H

#include <utils/flags.h>
#include <string>
#include <boost/filesystem/path.hpp>
#include <limits>
using namespace std;

namespace utils
{
    template <typename T>
    inline T getInvalidValue()
    {
        return (numeric_limits<T>::max)();
    }

    template <>
    inline bool getInvalidValue()
    {
        return false;
    }

    template <typename T>
    inline bool isInvalidValue(T t)
    {
        return t == getInvalidValue<T>();
    }

    template <>
    inline vector<int> getInvalidValue()
    {
        vector<int> vei(5, 0);
        return vei;
    }

    template <>
    inline vector<double> getInvalidValue()
    {
        vector<double> vei(5, 0);
        return vei;
    }
    template <>
    inline const vector<double>& getInvalidValue()
    {
        static vector<double> vei(5, 0);
        return vei;
    }

    template <typename T>
    inline void safeDeletePtr(T *&p)
    {
        if (NULL != p)
        {
            delete p;
            p = NULL;
        }
    }

    template <typename T>
    inline void safeDeleteArr(T *&p)
    {
        if (NULL != p)
        {
            delete [] p;
            p = NULL;
        }
    }

    template <typename TContainer>
    inline void safeClearContainer(TContainer *pContainer)
    {
        typedef typename TContainer::iterator TContainerItr;
        if (NULL != pContainer)
        {
            for (TContainerItr it = pContainer->begin();
                it != pContainer->end(); it++)
            {
                delete (*it);
            }
            pContainer->clear();
        }
    }
    
    template<typename T>
    T stdMax(const T& a,const T& b)
    {
        return a<b?b:a;
    }

    template<typename T>
    T stdMin(const T& a,const T& b)
    {
        return a>b?b:a;
    }

    template <typename T>
    inline void limitValue(T &dst, const T &v1, const T &v2)
    {
        T realMin = stdMin<T>(v1, v2);
        T realMax = stdMax<T>(v1, v2);
        dst = stdMax<T>(dst, realMin);
        dst = stdMin<T>(dst, realMax);
    }

    inline string str_s(const char * const sz)
    {
        return (NULL != sz) ? sz : "";
    }

    inline int atoi_s(const char * const sz)
    {
        return (NULL != sz) ? atoi(sz) : getInvalidValue<int>();
    }

    inline double atof_s(const char * const sz)
    {
        return (NULL != sz) ? atof(sz) : getInvalidValue<double>();
    }

    string getDir( const string &fileName );

    string dirString( const boost::filesystem::path &path );

    string wsToString(wstring str);
    wstring sToWString(string str);

    enum EFormatDoubleNumberFlag
    {
        FDF_NONE            = 0x00000000,
        
        FDF_PRECISION_1     = 0x00000001,
        FDF_PRECISION_2     = 0x00000002,
        FDF_PRECISION_3     = 0x00000003,
        FDF_PRECISION_4     = 0x00000004,
        FDF_PRECISION_MASK  = 0x0000FFFF,

        FDF_PERCENTAGE      = 0x00010000,
        FDF_SEGMENTED       = 0x00020000,
        FDF_AUTO_TRIM       = 0x00040000,
    };

    TT_DECLARE_FLAGS(FFormatDoubleNumberFlag, EFormatDoubleNumberFlag)

    string formatDoubleNumber(const long double dPrice, FFormatDoubleNumberFlag flag = FDF_PRECISION_2);
}

TT_DECLARE_OPERATORS_FOR_FLAGS(utils::FFormatDoubleNumberFlag)

#define     XT_VALID(value, type, def)    (!utils::isInvalidValue<type>((value)) ? (value) : (def))

#endif
