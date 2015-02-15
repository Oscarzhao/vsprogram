#ifndef CIPHER_H_
#define CIPHER_H_
#include "BaseCipher.h"
#include "openssl/aes.h"
#include "openssl/evp.h"
#include "openssl/ossl_typ.h"

namespace utils
{
    class AESCipher : public BaseCipher
    {
    public:
        AESCipher();
        ~AESCipher();

        bool init(unsigned char *key_data, int key_data_len);
        bool init2(unsigned char *key_data, int key_data_len); // 保持和portal的加解密算法一致

        virtual r_int32 encrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) ;

        virtual r_int32 decrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) ;

    private:
        EVP_CIPHER_CTX m_ectx;
        EVP_CIPHER_CTX m_dctx;
    };

    boost::shared_ptr<BaseCipher> createCipher(const char* key, const size_t& times);

    string encryptPassword(const string& strUser, const string& strPassword);
    string decryptPassword(const string& strUser, const string& strPassword);
}

#endif
