#include "common/Stdafx.h"
#ifdef WIN32

namespace utils{
    bool setMaxOpenFile(int maxlimit)
    {
        return false;
    }
}
#else

#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>

namespace utils
{
    bool setMaxOpenFile(int maxlimit)
    {
        bool ret = false;
        struct rlimit limit;
        memset(&limit, 0, sizeof(limit));
        int e = getrlimit(RLIMIT_NOFILE, &limit);
        if(e < 0)
            return ret;
        struct rlimit newlimit;
        memset(&newlimit, 0, sizeof(newlimit));
        newlimit.rlim_cur = maxlimit;
        newlimit.rlim_max = maxlimit;
        if( (e = setrlimit(RLIMIT_NOFILE, &newlimit)) < 0)
        {
            setrlimit(RLIMIT_NOFILE, &limit);
        }
        else
        {
            ret = true;
        }
        return ret;
    }
}

#endif

