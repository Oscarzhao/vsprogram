#ifndef SERVER2_UTILS_STREAM_H_
#define SERVER2_UTILS_STREAM_H_

namespace utils
{

    /**
     * <p>调用这个函数将value存放到字符串中.</p>
     *
     *
     * @param pRet为转换后的字符串的首地址.
     * @param value为一个要存放的bool值.
     * @return 无.
     * @exception 无.
     */
    void encodeBool(char **pRet, const bool value);

    /**
     * <p>调用这个函数将value存放到字符串中.</p>
     *
     *
     * @param pRet为转换后的字符串的首地址.
     * @param value为一个要存放的整数.
     * @param toNet为是否转换为网络序
     * @return 无.
     * @exception 无.
     */
    void encodeShort(char **pRet, const short &value, bool toNet);
    void encodeInt(char **pRet, const r_int32 &value, bool toNet);

    /**
     * <p>调用这个函数将value存放到字符串中.</p>
     *
     *
     * @param pRet为转换后的字符串的首地址.
     * @param value为一个要存放的整数.
     * @param toNet为是否转换为网络序
     * @return 无.
     * @exception 无.
     */
    void encodeUint(char **pRet, const r_uint32 &value, bool toNet);

    /**
    * <p>调用这个函数将value存放到字符串中.</p>
    *
    *
    * @param p 转换后的字符串的首地址.
    * @param value 一个要存放的浮点数.
    * @return 无.
    * @exception 无.
    */
    void encodeFloat(char **p, const float &value);

    /**
     * <p>调用这个函数将value存放到字符串中.</p>
     *
     *
     * @param pRet为转换后的字符串的首地址.
     * @param value为一个要存放的浮点数.
     * @return 无.
     * @exception 无.
     */
    void encodeDouble(char **pRet, const double &value);

    /**
     * <p>调用这个函数将value存放到字符串中.</p>
     *
     *
     * @param pRet为转换后的字符串的首地址.
     * @param value为一个要存放的字符串.
     * @param toNet为是否转换为网络序
     * @return 无.
     * @exception 无.
     */
    void encodeString(char **pRet, const string &value, bool toNet);

    /**
     * <p>调用这个函数从pSrc中取出一个bool变量.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的bool变量.
     * @return 无.
     * @exception 无.
     */
    void decodeBool(char **pSrc, bool &value);

    /**
     * <p>调用这个函数从pSrc中取出一个r_int32变量.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的整数.
     * @param  fromNet为是否转换为本地序
     * @return 无.
     * @exception 无.
     */
     void decodeShort(char **pSrc, short &value, bool fromNet);
     void decodeInt(char **pSrc, r_int32 &value, bool fromNet);

    /**
     * <p>调用这个函数从pSrc中取出一个r_int32变量.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的整数.
     * @param  fromNet为是否转换为本地序
     * @return 无.
     * @exception 无.
     */
     void decodeUint(char **pSrc, r_uint32 &value, bool fromNet);

    /**
     * <p>调用这个函数从pSrc中取出一个double变量.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的浮点数.
     * @return 无.
     * @exception 无.
     */
     void decodeDouble(char **pSrc, double &value);

    /**
     * <p>调用这个函数从pSrc中取出一个float变量.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的浮点数.
     * @return 无.
     * @exception 无.
     */
    void decodeFloat(char **pSrc, float &value);

    /**
     * <p>调用这个函数从vStream以nPos+sizeof(r_int32)开始的地方取出一个string类型的变量，变量的size为从vStream以nPos开始的地方取出的一个转化为本地序的r_int32变量的值.</p>
     *
     *
     * @param pSrc为一个要解析的字符串的首地址
     * @param value为一个要得到的字符串.
     * @param  fromNet为是否转换为本地序
     * @return 无.
     * @exception 无.
     */
    void decodeString(char **pSrc, string &value, bool fromNet);

    //压缩函数，在头部添加"ZiPeDiT"压缩头
    bool zlibCompress(const ByteArray& srcData, ByteArray& dstData);

    //解压缩函数
    bool zlibDecompress(const ByteArray& srcData, ByteArray& dstData);

    //压缩函数，在头部添加"ZiPeDiT"压缩头
    //reservedBytes是预留位置，如果reservedBytes小于0，则说明不用预留
    bool zlibCompress(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedLen = -1);

    //解压缩函数
    bool zlibDecompress(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedLen = -1);

    //Lzma压缩函数
   // reservedBytes是预留位置，如果reservedBytes小于0，则说明不用预留
    bool LzmaComp(const ByteArray& srcData, ByteArray& dstData);
    bool LzmaComp(unsigned char* const srcData, size_t srcDatalen, ByteArray& dstData, r_int32 reservedBytes = 0);

    //Lzma解压缩函数
    bool LzmaDecomp(const ByteArray& srcData, ByteArray& dstData);
    bool LzmaDecomp(unsigned char* const srcData, size_t srcDatalen, ByteArray& dstData, r_int32 reservedBytes = 0);

    /**
    * zlib压缩函数，保留头部预留空间
    * @dstData  压缩结果
    * @srcData  被压缩数据
    * @param    reservedBytes 压缩结果头部预留的空白字节数，一般被用来做一些标记字段
    * @mark     解压缩函数使用zlibDecompress2()
    * @author   huangjian
    */
    bool zlibCompress2(ByteArray& dstData, const ByteArray& srcData, r_int32 reservedBytes = 0);

    /**
    * zlib解压缩函数，保留头部预留空间
    * @dstData  解压缩结果
    * @srcData  被解压缩数据
    * @param    reservedBytes 被解压缩数据头部预留的字节数，不属于zip解压缩范围，一般被用来做一些标记字段
    * @mark     压缩函数使用zlibCompress2()
    * @author   huangjian
    */
    bool zlibDecompress2(ByteArray& dstData, const ByteArray& srcData, r_int32 reservedBytes = 0);
    bool zlibDecompress2(unsigned char* const srcData, size_t srcDataLen, ByteArray& dstData, r_int32 reservedLen);

    /*
    将二进制内容转化为base64编码
    @param input : 二进制数据的起始地址
    @param inputLength: 二进制数据的长度
    @param output:输出结果保存在这个变量
    @exception tlexception::InvalidArgumentException:如果input==NULL || inputLength==0
    
    */
    void encodeBase64(const BYTE* input, size_t inputLength, string&output);
    
    /*
    将base64编码字符串转化为二进制内容
    @param input : Base64编码字符串
    @param output:输出结果
    */
    void decodeBase64(string& input,vector<BYTE>& output);

}

#endif /* SERVER2_UTILS_STREAM_H */
