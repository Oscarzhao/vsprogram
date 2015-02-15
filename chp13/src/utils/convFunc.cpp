/**
 *  编码转换函数
 *
 *  @author zoubailiu
 *  @since  2012-09-25
 */

#include <utils/convFunc.h>
#include <iconv/iconv.h>
#ifndef WIN32
#include <string.h>
#endif
namespace utils
{

    int safeGbk2Utf8( std::string &strGbk )
    {
        if(strGbk.empty())
            return 0;

        const char *inbuf = strGbk.c_str();
        size_t inlen      = strGbk.length();

        size_t bufsize = 2 * inlen;
        char *buf      = new char[bufsize];
        memset(buf, 0, bufsize);
        char *pbuf = buf;

        iconv_t cd;
        cd = iconv_open("UTF-8", "GBK");
        if ( cd == 0 )
        {
            delete buf;
            return -1;
        }

        size_t leftsize   = bufsize;
        if ( iconv(cd, &inbuf, &inlen, &pbuf, &leftsize) == -1 )
        {
            iconv_close(cd);
            delete buf;
            return -1;
        }

        iconv_close(cd);
        strGbk = std::string(buf, bufsize-leftsize);
        delete buf;
        return 0;
    }

    int safeUtf82Gbk( std::string &strUtf8 )
    {
        if(strUtf8.empty())
            return 0;

        const char *inbuf = strUtf8.c_str();
        size_t inlen      = strUtf8.length();

        size_t bufsize = inlen;
        char *buf      = new char[bufsize];
        memset(buf, 0, bufsize);
        char *pbuf = buf;

        iconv_t cd;
        cd = iconv_open("GBK", "UTF-8");
        if ( cd == 0 )
        {
            delete buf;
            return -1;
        }

        size_t leftsize   = bufsize;
        if ( iconv(cd, &inbuf, &inlen, &pbuf, &leftsize) == -1 )
        {
            iconv_close(cd);
            delete buf;
            return -1;
        }

        iconv_close(cd);
        strUtf8 = std::string(buf, bufsize-leftsize);
        delete buf;
        return 0;

    }

    inline char _xtAtoi(const char & c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'z')
        {
            return c - 'a' + 10;
        }
        return c - '0';
    }

    int safeFormatConvert( std::string & str, const char * const szSrcEncoding, const char * const szDstEncoding )
    {
        if(str.empty())
            return 0;

        const char *inbuf = str.c_str();
        size_t inlen      = str.length();

        size_t bufsize = inlen;
        char *buf      = new char[bufsize];
        memset(buf, 0, bufsize);
        char *pbuf = buf;

        iconv_t cd = 0;
        cd = iconv_open(szDstEncoding, szSrcEncoding);
        if ( cd == 0 )
        {
            delete buf;
            return -1;
        }

        size_t leftsize   = bufsize;
        if ( iconv(cd, &inbuf, &inlen, &pbuf, &leftsize) == -1 )
        {
            iconv_close(cd);
            delete buf;
            return -1;
        }

        iconv_close(cd);
        str = std::string(buf, bufsize-leftsize);
        delete buf;
        return 0;
    }

    const char * const g_szUnicode = "UCS-2";
    // const char * const g_szUtf8 = "UTF-8";

    int safeUFormatToString( std::string & str )
    {
        std::string::size_type pos = 0;
        std::string temp = "  "; // 预留2个字节
        size_t size = str.size();
        while( (pos = str.find("\\u", pos)) != std::string::npos && pos <= size)
        {
            char c1 = _xtAtoi(str[pos + 2]);
            char c2 = _xtAtoi(str[pos + 3]);
            temp[0] = c1 * 16 + c2;
            char c3 = _xtAtoi(str[pos + 4]);
            char c4 = _xtAtoi(str[pos + 5]);
            temp[1] = c3 * 16 + c4;
            safeFormatConvert(temp, g_szUnicode, "GBK");
            str.replace(pos, 6, temp);
            pos += temp.size();
        }

        return 0;
    }

}

