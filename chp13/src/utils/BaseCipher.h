#ifndef BASE_CIPHER_H_
#define BASE_CIPHER_H_

namespace utils{

    enum CIPHER_STATUS{
        CIPHER_STATUS_OK,
        CIPHER_STATUS_BUFFER_ERROR,
        CIPHER_STATUS_UNKNOWN_ERROR
    };

    class BaseCipher
    {
    public:
        virtual r_int32 encrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) = 0;
        virtual r_int32 decrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) = 0;
    };

}

#endif