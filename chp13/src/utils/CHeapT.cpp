/**
 *  实现最大最小堆
 *
 *  @author zhangyi
 *  @since  2012-11-16
 */

#include <utils/CHeapT.h>
#include <utils/CHeapT.hpp>

namespace utils
{
    class CHeapIntSmallTraits
    {
    public:
        int compareValue(const int & lhs, const int & rhs) const
        {
            return lhs - rhs;
        }

        int key(const int & data) const
        {
            return data;
        }
    };

    class CHeapIntBigTraits
    {
    public:
        int compareValue(const int & lhs, const int & rhs) const
        {
            return rhs - lhs;
        }

        int key(const int & data) const
        {
            return data;
        }
    };

    template class CHeapT<int, CHeapIntSmallTraits>;
    template class CHeapT<int, CHeapIntBigTraits>;
}
