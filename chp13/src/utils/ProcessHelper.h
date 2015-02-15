#ifndef PROCESSHELPER_20120604_H_
#define PROCESSHELPER_20120604_H_

#include <boost/interprocess/sync/file_lock.hpp>
namespace utils
{
    class FileLock
    {
    public:
        FileLock(const char* path);
        ~FileLock();

    protected:
    private:
        boost::interprocess::file_lock* m_lock;
    };
}

#endif