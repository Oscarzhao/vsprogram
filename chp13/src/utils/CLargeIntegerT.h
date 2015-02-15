/**
 *  实现类似大整形数据
 *
 *  @author zhangyi
 *  @since  2012-09-25
 */

#ifndef SERVER3_BASE_UTILS_CLARGEINTEGERT_H
#define SERVER3_BASE_UTILS_CLARGEINTEGERT_H

#include <common/type.h>
#include <string>
using namespace std;

namespace utils
{
    template <int BITS_COUNT>
    class CLargeInteger
    {
    public:
        CLargeInteger();
        virtual ~CLargeInteger();

        void clear();
        void fullFill();
        
        r_uint32 data(int i) const;
        string toHex() const;
        static CLargeInteger fromHex(string strHex);

        void addBit(int nOffset);
        void removeBit(int nOffset);
        bool hasBit(int nOffset) const;

    protected:

        // 一个int32占bit数
        static const int INT_BITS_COUNT = 8 * sizeof(r_int32);

        // 当前类型所需int数
        static const int INTEGER_COUNT = (BITS_COUNT - 1) / INT_BITS_COUNT + 1;

        r_uint32 m_data[INTEGER_COUNT];
    };

}

#endif
