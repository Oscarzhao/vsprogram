#ifndef MATH_2009_06_04_H
#define MATH_2009_06_04_H

#include <common/type.h>
#include <boost/lexical_cast.hpp>
#include "utils/commonFunc.h"
#include <math.h>
using namespace std;

#define FLOAT_ERROR 1e-4  // 浮点误差值


#define ARRAYLEN(x) ((sizeof((x)) / (sizeof(((x)[0])))))


namespace utils
{
    const double IllegalDouble = (std::numeric_limits<double>::max)();
    const r_int32 IllegalInt32 = (std::numeric_limits<r_int32>::max)();

    //--------------------------------------------------------------------------

    /**
    * 判断浮点数是否等于零
    */
    inline bool isZero(const double& data) { return fabs(data) <= FLOAT_ERROR; }
    inline bool isZero(const double& data, const double& dTick) { return fabs(data) <= dTick;}

    /**
    * 判断浮点数是否大于零
    */
    inline bool isGreaterThanZero(const double& data) { return data > FLOAT_ERROR; }

    /**
    * 判断浮点数是否小于零
    */
    inline bool isLessThanZero(const double& data) { return data < -1 * FLOAT_ERROR; }

    /**
    * 判断两个浮点数是否相等
    */
    inline bool isEqual(const double& data1, const double& data2) { return isZero(data1 - data2); }
    inline bool isEqual(const double& data1, const double& data2, const double& dTick)
    {
        //return isEqual(
        //    static_cast<int>((data1 + 0.5) / dTick) * dTick,
        //    static_cast<int>((data2 + 0.5) / dTick) * dTick
        //    );
        //模型客户端的分价表用被注释的算法无法得到正确的结果
        if (fabs(data1 - data2) < dTick)
        {
            return true;
        }
        return false;
    }

    /**
     *  判断浮点数是否实际上是个整数
     */
    inline bool isInteger(const double& data) { return isEqual(data, (const double&)((int)data)); }

    /**
    * 判断两个浮点数前者大
    */
    inline bool isGreater(const double& data1, const double& data2) { return isGreaterThanZero(data1 - data2); }

    /**
    * 判断两个浮点数前者大于等于后者
    */
    inline bool isGreaterEqual(const double& data1, const double& data2) { return !isLessThanZero(data1 - data2); }

    /**
    * 判断两个浮点数前者小
    */
    inline bool isLess(const double& data1, const double& data2) { return isLessThanZero(data1 - data2); }

    /**
    * 判断两个浮点数前者小于等于后者
    */
    inline bool isLessEqual(const double& data1, const double& data2) { return !isGreaterThanZero(data1 - data2); }

    /**
    * 把浮点数化的整数还原为整数
    */
    inline void getIntFromDouble(const double& fValue, r_int32& iValue) { iValue = (r_int32)(fValue + FLOAT_ERROR);}

    /**
    * 把浮点数化的整数还原为整数
    */
    inline void getIntFromDouble(const double& fValue, r_int64& iValue) { iValue = (r_int64)(fValue + FLOAT_ERROR);}

    /**
    * 把浮点数化的整数还原为整数
    */
    inline r_int32 getInt32FromDouble(const double& fValue) { return (r_int32)(fValue + FLOAT_ERROR);}

    /**
    * 把浮点数化的整数还原为整数
    */
    inline r_int64 getInt64FromDouble(const double& fValue) { return (r_int64)(fValue + FLOAT_ERROR);}

    /**
    * 求最大公约数
    */
    inline int getCommonDivisor(int a, int b)
    {
        int nMax = max<int>(a, b);
        int nMin = min<int>(a, b);

        int t =  nMax% nMin;
        while( t > 0 )
        {
            nMax= nMin;
            nMin= t;
            t = nMax% nMin;   
        }
        return nMin;
    }

    /**
    * 把浮点数按照单位和精度做整数化
    *
    * 注意：unit应该是1, 10, 100... 的形式，而prec指在unit下保留几位小数的精度
    */
    inline r_int64 getIntFromDouble(const double& fValue, r_int32 unit, r_uint32 prec) 
    { 
        r_int32 multiplier = 1;
        while((prec--) > 0)
        {
            multiplier *= 10;
        }

        if (fValue < 0)
        {
            return (r_int64)(((fValue) * multiplier / unit) - 0.5);
        }
        
        return (r_int64)(0.5 + ((fValue) * multiplier / unit));
    }

    /**
    * 把整数按照单位和精度做规范化
    *
    * 注意：unit应该是1, 10, 100... 的形式，而prec指在unit下保留几位小数的精度
    */
    inline r_int64 adjustInt(r_int32 iValue, r_int32 unit, r_uint32 prec) 
    { 
        r_int32 multiplier = 1;
        while((prec--) > 0)
        {
            multiplier *= 10;
        }

        if (iValue < 0)
        {
            return (r_int64)(((iValue) * multiplier / unit) - 0.5);
        }

        return (r_int64)(0.5 + ((iValue) * multiplier / unit));
    }

    inline int doubleTickToPrecision(const double& dTick)
    {
        if (dTick == getInvalidValue<double>())
        {
            return 2;
        }

        int nPrecision = 0;
        int nMaxPrecision = 10;     // 防止死循环
        double nowTick = dTick;
        while (!utils::isInteger(nowTick) && nPrecision <= nMaxPrecision)
        {
            nowTick*= 10.0;
            nPrecision++;
        }
        return nPrecision;
    }

    inline r_int64 xt_add(const r_int64& left, const r_int64& right, bool egnoreInvalid = true)
    {
        r_int64 ret = getInvalidValue<r_int64>();
        if (egnoreInvalid)
        {
            if (!utils::isInvalidValue(left))
                ret = left;

            if (!utils::isInvalidValue(right))
            {
                if (!utils::isInvalidValue(ret))
                {
                    ret += right;
                } else {
                    ret = right;
                }
            }
        } else {
            if (!isInvalidValue(left) && !isInvalidValue(right))
            {
                ret = left + right;
            }
        }
        return ret;
    }

    inline double xt_add_double(const double& left, const double& right, bool egnoreInvalid = true)
    {
        double ret = getInvalidValue<double>();
        if (egnoreInvalid)
        {
            if (!utils::isInvalidValue(left))
                ret = left;

            if (!utils::isInvalidValue(right))
            {
                if (!utils::isInvalidValue(ret))
                {
                    ret += right;
                } else {
                    ret = right;
                }
            }
        } else {
            if (!isInvalidValue(left) && !isInvalidValue(right))
            {
                ret = left + right;
            }
        }
        return ret;
    }

    /**
     *  判断浮点数为最小值
     */
#ifdef WIN32
    inline bool isMinDouble(const double& data) { return DBL_MIN == data || -HUGE_VAL == data; }
#else
    inline bool isMinDouble(const double& data) { return isEqual(-HUGE_VAL, data); }
#endif // WIN32

    /**
     *  判断浮点数为最大值
     */
#ifdef WIN32
    inline bool isMaxDouble(const double& data) { return DBL_MAX == data || HUGE_VAL == data; }
#else
    inline bool isMaxDouble(const double& data) { return isEqual(HUGE_VAL, data); }
#endif // WIN32

    inline int round(const double& data) { return isGreaterThanZero(data) ? (int)(data + 0.5) : (int)(data - 0.5); }
    inline r_int64 round64(const double& data) { return isGreaterThanZero(data) ? (r_int64)(data + 0.5) : (r_int64)(data - 0.5); }
}

#endif //MATH_2009_06_04_H
