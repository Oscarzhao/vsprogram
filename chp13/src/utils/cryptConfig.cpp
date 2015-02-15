#include <utils/cryptConfig.h>
#include "utils/FileHelper.h"
#include <utils/sysutils.h>
#include <common/type.h>
#include "openssl/aes.h"
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/des.h"
#include "openssl/evp.h"
#include "utils/Cipher.h"

namespace utils
{
    using namespace std;

    static string getKey();
    static bool encrypt(const char* raw, size_t len, const string& cryptKey, string& encryptStr);
    static bool decrypt(const char* raw, size_t len, const string& cryptKey, string& decryptStr);
    static utils::AESCipher& initCipher();

    static string s_key = getKey();
    static utils::AESCipher& s_cipher = initCipher();

    utils::AESCipher& initCipher()
    {
        static utils::AESCipher s_cipher;
        s_cipher.init((unsigned char*)s_key.c_str(), s_key.size());
        return s_cipher;
    };

    bool CCryptConfig::load(utils::Configuration& config, string configFilePath)
    {
        bool ret = false;
    
        config.load(configFilePath);

        string content;
        if (!utils::FileHelper::open(configFilePath, content))
        {
            return false;
        }

        string decryptContent;
        if (!decrypt(content.c_str(), content.size(), s_key, decryptContent))
        {
            return false;
        }

        return config.parse(decryptContent);
    }

    bool CCryptConfig::save(utils::Configuration& config)
    {
        string dumpStr = config.dump();
        string encryptStr;
        if (!encrypt(dumpStr.c_str(), dumpStr.size(), s_key, encryptStr))
        {
            return false;
        }

        return config.save(encryptStr);
    }

    bool CCryptConfig::load(string configFilePath, string& content)
    {
        string raw;
        if (!utils::FileHelper::open(configFilePath, raw))
        {
            return false;
        }

        if (!decrypt(raw.c_str(), raw.size(), s_key, content))
        {
            return false;
        }
        return true;
    }

    bool CCryptConfig::save(string configFilePath, string content)
    {
        string encryptStr;
        if (!encrypt(content.c_str(), content.size(), s_key, encryptStr))
        {
            return false;
        }

        FILE *file = fopen(configFilePath.c_str(), "wb");
        if (file == NULL)
            return false;

        fwrite(encryptStr.c_str(), sizeof(char), encryptStr.size(), file);
        fclose(file);
        return true;
    }

    string getKey()
    {
        string key = utils::getCpuId();
        if (key.empty())
        {
            key = utils::getBiosId();
        }

        if (key.empty())
        {
            key = utils::getDiskId();
        }

        if (key.empty())
        {
            key = "unknown, just use this";
        }
        
        return key;
    }

    bool encrypt(const char* raw, size_t rawLen, const string& cryptKey, string& encryptStr)
    {
        size_t encrypedLen = rawLen % AES_BLOCK_SIZE == 0 ? rawLen : (rawLen / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE; 
        encryptStr.resize(encrypedLen);
        
        int ret = s_cipher.encrypt((unsigned char *)raw, rawLen, (unsigned char *)&encryptStr[0], &encrypedLen);
        if (ret == utils::CIPHER_STATUS_OK)
        {
            if (encryptStr.size() > encrypedLen)
            {
                encryptStr.resize(encrypedLen);
            }
            return true;
        } else {
            return false;
        }
    }

    bool decrypt(const char* raw, size_t rawLen, const string& cryptKey, string& decryptStr)
    {
        size_t decrypedLen = rawLen ; 
        decryptStr.resize(decrypedLen);

        int ret = s_cipher.decrypt((unsigned char *)raw, rawLen, (unsigned char *)&decryptStr[0], &decrypedLen);
        if (ret == utils::CIPHER_STATUS_OK)
        {
            if (decryptStr.size() > decrypedLen)
            {
                decryptStr.resize(decrypedLen);
            }
            return true;
        } else {
            return false;
        }
        
        return true;
    }

    void CCryptConfig::test()
    {
    }

    void sha1sum( unsigned char digest[20], const void *input, size_t length )
    {
        SHA_CTX sha_ctx;
        //memset(digest, 0, sizeof(digest));
        SHA1_Init(&sha_ctx);
        SHA1_Update(&sha_ctx, input, length);
        SHA1_Final(digest, &sha_ctx);
    }

}