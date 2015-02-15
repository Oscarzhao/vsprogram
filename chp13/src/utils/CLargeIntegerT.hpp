/**
 *  实现类似大整形数据
 *
 *  @author zhangyi
 *  @since  2012-09-25
 */

#include <utils/CLargeIntegerT.h>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace utils
{
    template <int BITS_COUNT>
    CLargeInteger<BITS_COUNT>::CLargeInteger()
    {
        clear();
    }

    template <int BITS_COUNT>
    CLargeInteger<BITS_COUNT>::~CLargeInteger()
    {

    }

    template <int BITS_COUNT>
    void CLargeInteger<BITS_COUNT>::clear()
    {
        memset(m_data, 0, INTEGER_COUNT * sizeof(r_uint32));
    }

    template <int BITS_COUNT>
    void CLargeInteger<BITS_COUNT>::fullFill()
    {
        memset(m_data, 0xff, INTEGER_COUNT * sizeof(r_uint32));
    }

    template <int BITS_COUNT>
    r_uint32 CLargeInteger<BITS_COUNT>::data( int i ) const
    {
        return m_data[i];
    }

    template <int BITS_COUNT>
    string CLargeInteger<BITS_COUNT>::toHex() const
    {
        string strOut;
        static const int c_nIntCharCount = 8;   // 8字节string描述一个int
        char sz[c_nIntCharCount + 1];

        for (int i = 0; i < INTEGER_COUNT - 1; i++)
        {
            memset(sz, 0, c_nIntCharCount + 1);
            sprintf(sz, "%08x", m_data[i]);
            strOut += sz;
        }

/*
        int nShift = INT_BITS_COUNT - LAST_INT_BITS_OCCUPY;
        int nLast = (m_data[INTEGER_COUNT - 1] >> nShift);
*/
        memset(sz, 0, c_nIntCharCount + 1);
        sprintf(sz, "%08x", m_data[INTEGER_COUNT - 1]);
        strOut += sz;

        return strOut;
    }

    inline r_uint32 cToInt(char c)
    {
        if (c >= 'a')
        {
            return c - 'a' + 10;
        }
        if (c >= 'A')
        {
            return c - 'A' + 10;
        }
        return c - '0';
    }

    template <int BITS_COUNT>
    CLargeInteger<BITS_COUNT> CLargeInteger<BITS_COUNT>::fromHex( string strHex )
    {
        CLargeInteger<BITS_COUNT> largeInt;

        const char * c = strHex.c_str();
        for (int iInt = 0; iInt < INTEGER_COUNT - 1; iInt++)
        {
            r_uint32 n = 0;
            for (int iNum = 0; iNum < 8; iNum++, c++)
            {
                n = n * 16 + cToInt(*c);
            }
            largeInt.m_data[iInt] = n;
        }
        r_uint32 n = 0;
        for (int iNum = 0; *c != 0; iNum++, c++)
        {
            n = n * 16 + cToInt(*c);
        }
        largeInt.m_data[INTEGER_COUNT - 1] = n;

        return largeInt;
    }

    template <int BITS_COUNT>
    void CLargeInteger<BITS_COUNT>::addBit( int nOffset )
    {
        int iInt = nOffset / INT_BITS_COUNT;
        int iOff = nOffset % INT_BITS_COUNT;
        m_data[iInt] |= (0x80000000 >> iOff);
    }

    template <int BITS_COUNT>
    void CLargeInteger<BITS_COUNT>::removeBit( int nOffset )
    {
        int iInt = nOffset / INT_BITS_COUNT;
        int iOff = nOffset % INT_BITS_COUNT;
        m_data[iInt] &= (~(0x80000000 >> iOff));
    }

    template <int BITS_COUNT>
    bool CLargeInteger<BITS_COUNT>::hasBit( int nOffset ) const
    {
        int iInt = nOffset / INT_BITS_COUNT;
        int iOff = nOffset % INT_BITS_COUNT;
        return (0 != (m_data[iInt] & (0x80000000 >> iOff)));
    }

}
