#include "utils/ProcessHelper.h"

#ifdef WIN32
#include <process.h>
#endif

namespace utils
{
    
    FileLock::FileLock(const char* path) 
        :m_lock(new boost::interprocess::file_lock(path))
    {
        m_lock->lock();
    }

    FileLock::~FileLock()
    {
        m_lock->unlock();
        delete m_lock;
        m_lock = NULL;
    }

}
