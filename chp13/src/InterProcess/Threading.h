#ifndef UTILS_THREADING_H
#define UTILS_THREADING_H

#include <boost/thread/recursive_mutex.hpp>
//#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace utils
{
    typedef boost::recursive_mutex  RecMutex;
    typedef boost::recursive_mutex::scoped_lock RecLock;

    typedef boost::shared_mutex RWMutex;
    typedef boost::shared_lock<boost::shared_mutex> ReadLock;
    typedef boost::unique_lock<boost::shared_mutex> WriteLock;
}

#endif // UTILS_THREADING_H