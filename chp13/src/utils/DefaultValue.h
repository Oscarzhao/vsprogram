/********************************************************************
    company:    北京睿智融科控股有限公司
    fileName:    DefalutValue.h
    author:        xujun    
    created:    2014/110/   13:22    
    purpose:    默认值
*********************************************************************/
#ifndef DefaultValue_2014_1_10_H
#define DefaultValue_2014_1_10_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace utils
{
    template <typename T>
    inline T getDefaultValue()
    {
        return (numeric_limits<T>::max)();
    }

    template <>
    inline bool getDefaultValue()
    {
        return false;
    }
}

#endif