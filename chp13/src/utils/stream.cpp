
#include "common/Stdafx.h"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include "zlib/zlib.h"
#include "lzma/LzmaLib.h"
#include "lzma/Types.h"
#include "utils/stream.h"
#ifdef _WIN32_WINNT
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

namespace utils
{
    void encodeBool(char **p, const bool value)
    {
        char ch = value ? '1' : '0';
        memcpy(*p, &ch, sizeof(char));
        *p += sizeof(char);
    }

    void encodeShort(char **p, const short &value, bool toNet)
    {
        short t = toNet ? htons(value) : value;
        memcpy(*p, &t, sizeof(short));
        *p += sizeof(short);
    }

    void encodeInt(char **p, const r_int32 &value, bool toNet)
    {
        r_int32 t = toNet ? htonl(value) : value;
        memcpy(*p, &t, sizeof(r_int32));
        *p += sizeof(r_int32);
    }

    void encodeUint(char **p, const r_uint32 &value, bool toNet)
    {
        r_uint32 t = toNet ? htonl(value) : value;
        memcpy(*p, &t, sizeof(r_uint32));
        *p += sizeof(r_uint32);
    }

    void encodeFloat(char **p, const float &value)
    {
        memcpy(*p, &value, sizeof(float));
        *p += sizeof(float);
    }

    void encodeDouble(char **p, const double &value)
    {
        memcpy(*p, &value, sizeof(double));
        *p += sizeof(double);
    }

    void encodeString(char **p, const string &value, bool toNet)
    {
        r_int32 nSize = value.size();
        if (toNet)
        {
            nSize = htonl(nSize);
        }
        memcpy(*p, &nSize, sizeof(r_int32));
        *p += sizeof(r_int32);
        memcpy(*p, (char*)value.data(), value.size());
        *p += value.size();
    }

    void decodeBool(char **p, bool &value)
    {
        char ch;
        memcpy(&ch, *p, sizeof(char));
        if (ch=='1')
        {
            value = true;
        }
        else
        {
            value = false;
        }
        *p += sizeof(char);
    }

    void decodeShort(char **p, short &value, bool fromNet)
    {
        memcpy(&value, *p, sizeof(short));
        if (fromNet)
        {
            value = ntohs(value);
        }
        *p += sizeof(short);
    }

    void decodeInt(char **p, r_int32 &value, bool fromNet)
    {
        memcpy(&value, *p, sizeof(r_int32));
        if (fromNet)
        {
            value = ntohl(value);
        }
        *p += sizeof(r_int32);
    }

    void decodeUint(char **p, r_uint32 &value, bool fromNet)
    {
        memcpy(&value, *p, sizeof(r_uint32));
        if (fromNet)
        {
            value = ntohl(value);
        }
        *p += sizeof(r_uint32);
    }

    void decodeDouble(char **p, double &value)
    {
        memcpy(&value, *p, sizeof(double));
        *p += sizeof(double);
    }

    void decodeFloat(char **p, float &value)
    {
        memcpy(&value, *p, sizeof(float));
        *p += sizeof(float);
    }

    void decodeString(char **p, string &value, bool fromNet)
    {
        r_int32 nLength = 0;
        memcpy(&nLength, *p, sizeof(r_int32));
        if (fromNet)
        {
            nLength = ntohl(nLength);
        }
        *p += sizeof(r_int32);
        value.resize(nLength);
        memcpy((char*)value.data(), *p, nLength);
        *p += nLength;
    }

    static const string zipString = "ZiPeDiT";
    static const string::size_type zipLen = zipString.length();

    //压缩函数，在头部添加"ZiPeDiT"压缩头
    bool zlibCompress(const ByteArray& srcData, ByteArray& dstData)
    {
        bool ok = false;  //  压缩是否成功
        unsigned long reservedBytes = zipLen;
        unsigned long srcDataLen = srcData.size();
        unsigned long dstDataLen = 2 * srcDataLen + 16; /* 预分配压缩结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen + reservedBytes);

        r_int32 ret = compress(&dstData[reservedBytes], &dstDataLen, &srcData[0], srcDataLen);
        switch (ret)
        {
        case Z_OK:
            {
                ok = true;
                dstData.resize(dstDataLen + reservedBytes);
                // 压缩成功，拷贝压缩头
                memcpy(&dstData[0], zipString.c_str(), reservedBytes);
                break;
            }
        default:
            {
                dstData.resize(srcDataLen);
                // 压缩失败，不拷贝压缩头
                memcpy(&dstData[0], &srcData[0], srcDataLen);
                break;
            }
        }

        return ok;
    }

    //解压缩函数
    bool zlibDecompress(const ByteArray& srcData, ByteArray& dstData)
    {        
        bool ok = false;  // 解压是否成功
        bool flag = false; // 防止程序死循环

        unsigned long reservedBytes = zipLen;
        unsigned long srcDataLen = srcData.size();
        unsigned long dstDataLen = srcDataLen * 10;   /* 预分配解压结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen);

        // 是否有压缩头
        if ((srcDataLen < reservedBytes) || (strncmp((const char *)&srcData[0], zipString.c_str(), zipLen) != 0))
        {
            reservedBytes = 0;
            dstData.resize(srcDataLen - reservedBytes);
            memcpy(&dstData[0], &srcData[reservedBytes], srcDataLen - reservedBytes);
            ok = true;

            return ok;
        }

        while(true)
        {
            r_int32 ret = uncompress(&dstData[0], &dstDataLen, &srcData[reservedBytes], srcDataLen - reservedBytes);
            switch(ret)
            {
            case Z_OK:
                {
                    ok = true;
                    flag = true;
                    dstData.resize(dstDataLen);
                }
                break;

            case Z_BUF_ERROR:
                {
                    dstDataLen += dstDataLen;
                    dstData.resize(dstDataLen);
                }
                break;

            case Z_MEM_ERROR:
                flag = true;
                break;

            case Z_DATA_ERROR:
                flag = true;
                break;

            default:
                flag = true;
                break;
            }

            if (flag)
            {
                break;
            }
        }

        if (!ok)
        {
            // 解压失败，拷贝原始数据
            dstData.resize(srcDataLen - reservedBytes);
            memcpy(&dstData[0], &srcData[reservedBytes], srcDataLen - reservedBytes);
        }

        return ok;
    }

    //压缩函数，在头部添加"ZiPeDiT"
    bool zlibCompress(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedLen)
    {
        bool ok = false;  //  压缩是否成功
        unsigned long reservedBytes = zipLen;
        if (reservedLen >= 0 )
        {
            reservedBytes = reservedLen;
        }
        unsigned long dstDataLen = 2 * srcDataLen + 16; /* 预分配压缩结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen + reservedBytes);

        r_int32 ret = compress(&dstData[reservedBytes], &dstDataLen, &srcData[0], srcDataLen);
        switch (ret)
        {
        case Z_OK:
            {
                ok = true;
                dstData.resize(dstDataLen + reservedBytes);
                // 压缩成功，拷贝压缩头
                if (reservedLen < 0)
                {
                    memcpy(&dstData[0], zipString.c_str(), reservedBytes);
                }
                break;
            }
        default:
            {
                dstData.resize(srcDataLen);
                // 压缩失败，不拷贝压缩头
                if (reservedLen < 0)
                {
                    memcpy(&dstData[0], &srcData[0], srcDataLen);
                }
                else
                {
                    memcpy(&dstData[reservedLen], &srcData[0], srcDataLen);
                }
                break;
            }
        }

        return ok;
    }

     struct pras
     {
         int level ;
         int lc;
         int lp;
         int pb;
         int fb;
         int numThreads;
         unsigned int dictSize;
     } ;

     static const pras pp = {4,3,0,2,32,1,1<<22};
     typedef unsigned char props[5];
     static const size_t LzmaLen = sizeof(props);
     static const size_t ssLen = sizeof(size_t);

    bool LzmaComp(const ByteArray& srcData, ByteArray& dstData)
    {
        bool ok = false;
       
        size_t srcLen = srcData.size();
        if (srcLen <= 0)
        {
            return ok;
        }
        size_t destLen = srcLen * 2 + LzmaLen + ssLen;
        props outProp = {0};
        size_t sizeProp = 5;
        dstData.resize(destLen);
        r_int32 ret = LzmaCompress(&dstData[LzmaLen + ssLen], &destLen, &srcData[0], srcLen, &outProp[0], &sizeProp, \
            pp.level, pp.dictSize, pp.lc, pp.lp, pp.pb, pp.fb, pp.numThreads);
       
        switch(ret)
        {
        case SZ_OK:
            {
                ok = true;
                memcpy(&dstData[0], &outProp, LzmaLen);
                memcpy(&dstData[LzmaLen], &srcLen, ssLen);
                dstData.resize(destLen + LzmaLen + ssLen);
            }
            break;

        default:
            {
                memcpy(&dstData[0], &srcData[0], srcLen);
                dstData.resize(srcLen);
            }
            break;
        }
        return ok;
    }

    bool LzmaDecomp(const ByteArray& srcData, ByteArray& dstData)
    {
        bool ok = false;
       
        size_t sizeProp = 5;
        size_t srcLen = srcData.size() - LzmaLen -ssLen;
        if (srcLen <= 0)
        {
            return ok;
        }
        size_t dstLen =  0;
        props prop;
        memcpy(&prop[0], &srcData[0], LzmaLen);
        memcpy(&dstLen,&srcData[LzmaLen],ssLen);
        dstData.resize(dstLen);
        r_int32 ret = LzmaUncompress(&dstData[0], &dstLen, &srcData[LzmaLen+ssLen], &srcLen, &prop[0], sizeProp);
        switch(ret)
        {
        case SZ_OK:
            {
                ok = true;
                dstData.resize(dstLen);
            }
            break;
        default:
            {
                ok = false;
                memcpy(&dstData[0], &srcData[0], srcLen);
                dstData.resize(srcLen);
            }
            break;
        }
        return ok;
    }

    bool LzmaComp(unsigned char* const srcData, size_t srcDatalen, ByteArray& dstData, r_int32 reservedBytes)
    {
        bool ok = false;
        if (reservedBytes < 0)
        {
            reservedBytes = 0;
        }
        if (srcDatalen <= 0)
        {
            return ok;
        }

        size_t destLen = srcDatalen * 2 + LzmaLen + ssLen + reservedBytes;
        props outProp = {0};
        size_t sizeProp = 5;
        dstData.resize(destLen);
        r_int32 ret = LzmaCompress(&dstData[LzmaLen + ssLen +reservedBytes],&destLen,&srcData[0],srcDatalen,&outProp[0],&sizeProp,\
            pp.level,pp.dictSize,pp.lc,pp.lp,pp.pb,pp.fb,pp.numThreads);

        switch(ret)
        {
        case SZ_OK:
            {
                ok = true;
                memcpy(&dstData[reservedBytes],&outProp,LzmaLen);
                memcpy(&dstData[LzmaLen+ reservedBytes],&srcDatalen,ssLen);
               
                dstData.resize(destLen + LzmaLen + ssLen + reservedBytes);
            }
            break;

        default:
            {                 
                memcpy(&dstData[reservedBytes],&srcData[0],srcDatalen);
                dstData.resize(srcDatalen+reservedBytes); 
            }
            break;
        }
        return ok;
    }

    bool LzmaDecomp(unsigned char* const srcData, size_t srcDatalen, ByteArray& dstData, r_int32 reservedBytes)
    {
        bool ok = false;
        if (reservedBytes < 0)
        {
            reservedBytes = 0;
        }

        size_t sizeProp = 5;
        size_t srcLen =srcDatalen - LzmaLen - ssLen - reservedBytes;
        if (srcLen <= 0)
        {
            return ok;
        }
        size_t dstLen =  0;
        props prop;
        memcpy(&prop[0],&srcData[reservedBytes],LzmaLen);
        memcpy(&dstLen,&srcData[LzmaLen+reservedBytes],ssLen);
        dstData.resize(dstLen);
        r_int32 ret = LzmaUncompress(&dstData[0],&dstLen,&srcData[LzmaLen+ssLen + reservedBytes ],&srcLen,&prop[0],sizeProp);
        switch(ret)
        {
        case SZ_OK:
            {
                ok = true;
                dstData.resize(dstLen);
            }
            break;
        default:
            {
                ok = false;
                memcpy(&dstData[0],&srcData[reservedBytes ],srcDatalen);
                dstData.resize(srcDatalen-reservedBytes);
            }
            break;
        }
        return ok;
    }

    bool zlibDecompress2(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedBytes)
    {
        bool ok = false;  // 解压是否成功
        bool flag = false; // 防止程序死循环
        unsigned long dstDataLen = srcDataLen * 10;   /* 预分配解压结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen + reservedBytes);

        while(true)
        {
            r_int32 ret = uncompress(&dstData[reservedBytes], &dstDataLen, &srcData[reservedBytes], srcDataLen - reservedBytes);
            switch(ret)
            {
            case Z_OK:
                {
                    ok = true;
                    flag = true;
                    dstData.resize(dstDataLen + reservedBytes);

                    /* 拷贝预留头部数据 */
                    if (reservedBytes > 0)
                    {
                        memcpy(&dstData[0], &srcData[0], reservedBytes);
                    }
                }
                break;

            case Z_BUF_ERROR:
                {
                    dstDataLen += dstDataLen;
                    dstData.resize(dstDataLen + reservedBytes);
                }
                break;

            case Z_MEM_ERROR:
                flag = true;
                break;

            case Z_DATA_ERROR:
                flag = true;
                break;

            default:
                flag = true;
                break;
            }

            if (flag)
            {
                break;
            }
        }

        if (!ok)
        {
            // 解压失败，拷贝原始数据
            dstData.resize(srcDataLen);
            memcpy(&dstData[0], &srcData[0], srcDataLen);
        }

        return ok;
    }

    //解压缩函数
    bool zlibDecompress(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedLen)
    {
        bool ok = false;  // 解压是否成功
        bool flag = false; // 防止程序死循环

        if (reservedLen >= 0)
        {
            ok = zlibDecompress2(srcData, srcDataLen, dstData, reservedLen);
            return ok;
        }

        unsigned long reservedBytes = zipLen;
        
        unsigned long dstDataLen = srcDataLen * 10;   /* 预分配解压结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen);

        // 是否有压缩头
        if ((srcDataLen < reservedBytes) || (strncmp((const char *)&srcData[0], zipString.c_str(), zipLen) != 0))
        {
            reservedBytes = 0;
            dstData.resize(srcDataLen - reservedBytes);
            memcpy(&dstData[0], &srcData[reservedBytes], srcDataLen - reservedBytes);
            ok = true;

            return ok;
        }

        while(true)
        {
            r_int32 ret = uncompress(&dstData[0], &dstDataLen, &srcData[reservedBytes], srcDataLen - reservedBytes);
            switch(ret)
            {
            case Z_OK:
                {
                    ok = true;
                    flag = true;
                    dstData.resize(dstDataLen);
                }
                break;

            case Z_BUF_ERROR:
                {
                    dstDataLen += dstDataLen;
                    dstData.resize(dstDataLen);
                }
                break;

            case Z_MEM_ERROR:
                flag = true;
                break;

            case Z_DATA_ERROR:
                flag = true;
                break;

            default:
                flag = true;
                break;
            }

            if (flag)
            {
                break;
            }
        }

        if (!ok)
        {
            // 解压失败，拷贝原始数据
            dstData.resize(srcDataLen - reservedBytes);
            memcpy(&dstData[0], &srcData[reservedBytes], srcDataLen - reservedBytes);
        }

        return ok;
    }


    //zlib压缩函数，保留头部预留空间
    bool zlibCompress2(ByteArray& dstData, const ByteArray& srcData, r_int32 reservedBytes)
    {
        bool ok = false;  //  压缩是否成功
        unsigned long srcDataLen = srcData.size();
        unsigned long dstDataLen = srcDataLen + 16; /* 预分配压缩结果长度，如果压缩结果反而更大，那么将不会被压缩 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen + reservedBytes);

        r_int32 ret = compress(&dstData[reservedBytes], &dstDataLen, &srcData[0], srcDataLen);
        switch (ret)
        {
        case Z_OK:
            {
                ok = true;
                dstData.resize(dstDataLen + reservedBytes);
                break;
            }
        default:
            {
                dstData.resize(srcDataLen + reservedBytes);
                memcpy(&dstData[reservedBytes], &srcData[0], srcDataLen);
                break;
            }
        }

        return ok;
    }

    // zlib解压缩函数，保留头部预留空间
    bool zlibDecompress2(ByteArray& dstData, const ByteArray& srcData, r_int32 reservedBytes)
    {
        bool ok = false;  // 解压是否成功
        bool flag = false; // 防止程序死循环

        unsigned long srcDataLen = srcData.size();
        unsigned long dstDataLen = srcDataLen * 10;   /* 预分配解压结果长度 */

        if (srcDataLen <= 0)
        {
            return ok;
        }

        dstData.resize(dstDataLen + reservedBytes);

        while(true)
        {
            r_int32 ret = uncompress(&dstData[reservedBytes], &dstDataLen, &srcData[reservedBytes], srcDataLen - reservedBytes);
            switch(ret)
            {
            case Z_OK:
                {
                    ok = true;
                    flag = true;
                    dstData.resize(dstDataLen + reservedBytes);

                    /* 拷贝预留头部数据 */
                    if (reservedBytes > 0)
                    {
                        memcpy(&dstData[0], &srcData[0], reservedBytes);
                    }
                }
                break;

            case Z_BUF_ERROR:
                {
                    dstDataLen += dstDataLen;
                    dstData.resize(dstDataLen + reservedBytes);
                }
                break;

            case Z_MEM_ERROR:
                flag = true;
                break;

            case Z_DATA_ERROR:
                flag = true;
                break;

            default:
                flag = true;
                break;
            }

            if (flag)
            {
                break;
            }
        }

        if (!ok)
        {
            // 解压失败，拷贝原始数据
            dstData.resize(srcDataLen);
            memcpy(&dstData[0], &srcData[0], srcDataLen);
        }

        return ok;
    }

    /*
    将二进制内容转化为base64编码
    @param input : 二进制数据的起始地址
    @param inputLength: 二进制数据的长度
    @param output:输出结果保存在这个变量
    @exception tlexception::InvalidArgumentException:如果input==NULL || inputLength==0
    */
    void encodeBase64(const BYTE* input, size_t inputLength, string& output)
    {
        using namespace boost::archive::iterators;
        typedef base64_from_binary<
            transform_width<
            const BYTE*, 6, 8, boost::uint8_t
            >
        > base64_t;
        output = string(base64_t(&input[0]), base64_t(&input[0] + inputLength));
    }
    
    /*
    将base64编码字符串转化为二进制内容
    @param input : Base64编码字符串
    @param output:输出结果

    @note 考虑到该函数在linux gcc4.4下编译有问题，现推荐采用/include/crypt/TLCipher.h中的接口doBase64(...)
    */
    void decodeBase64(string& input,vector<BYTE>& output)
    {/*
        input.append(input.size() % 4, '=');
        typedef boost::archive::iterators::transform_width<
            boost::archive::iterators::binary_from_base64<char *>, 8, 6, boost::uint8_t
        > binary_t;

        basic_string<char>::size_type pos = input.find_last_not_of('=');
        // calculate how many characters we need to process
        pos = (pos == input.size() -1 ? input.size() : pos );
        string s(binary_t(&input[0]), binary_t(&input[0] + pos));   // 这里的代码在linux gcc4.4下编译有问题
        if(s.size()>0)
        {
            output.resize(s.size());
            memcpy(&output[0],&s[0],s.size());
        }*/
    }
}
