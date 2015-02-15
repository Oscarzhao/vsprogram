#include "common/Stdafx.h"
#include "utils/OpenSSLHelper.h"
#include <openssl/crypto.h>
#include <boost/lexical_cast.hpp>

namespace utils
{
    static void init_locks(void);
    static void kill_locks(void);

    OpenSSLHelper::OpenSSLHelper()
    {
        init_locks();
    }

    OpenSSLHelper::~OpenSSLHelper()
    {
        kill_locks();
    }

    static boost::mutex *lockarray;  

    static void lock_callback(int mode, int type, char *file, int line)
    {
        (void)file;
        (void)line;
        if (mode & CRYPTO_LOCK) {
#ifdef WIN32
            lockarray[type].lock();
#endif
        }
        else {
#ifdef WIN32
            lockarray[type].unlock();
#endif
        }
    }

    static unsigned long thread_id(void)
    {
        std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
        unsigned long threadNumber = 0;
        sscanf(threadId.c_str(), "%lx", &threadNumber);
        return threadNumber;
    }

    void init_locks(void)
    {
        using namespace boost;
        size_t num =  CRYPTO_num_locks() ;
        lockarray = (boost::mutex*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(boost::mutex));
        for (int i=0; i < CRYPTO_num_locks(); i++) 
        {
#ifdef WIN32
            lockarray[i].initialize();
#endif
        }

        CRYPTO_set_id_callback((unsigned long (*)())thread_id);
        CRYPTO_set_locking_callback((void (*)(int mode,int type, const char *file,int line))lock_callback);
    }

    void kill_locks(void)
    {
        CRYPTO_set_locking_callback(NULL);
        for (int i=0; i < CRYPTO_num_locks(); i++)
        {
#ifdef WIN32
            lockarray[i].destroy();
#endif
        }
        OPENSSL_free(lockarray);
    }
}