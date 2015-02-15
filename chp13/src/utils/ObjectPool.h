/********************************************************************
    created:    2013/10/12
    created:    12:10:2013   21:18
    filename:     f:\server5\ttservice\branches\shoushan_20130828\libs\base\src\utils\ObjectPool.h
    file path:    f:\server5\ttservice\branches\shoushan_20130828\libs\base\src\utils
    file base:    ObjectPool
    file ext:    h
    author:        xujun
    
    purpose:    ∂‘œÛ≥ÿ
*********************************************************************/
#ifndef ObjectPool_2013_10_12_H
#define ObjectPool_2013_10_12_H

#include <iostream>
#include <boost/pool/object_pool.hpp>

using namespace std;

namespace utils
{
    template <class T> 
    class ObjectPool{ 
    public: 
        static  boost::object_pool<T> _objpool; 
        static void * operator new(size_t size){
            return _objpool.malloc();
        }
        static void operator delete(void *rawmemory, size_t size){
            _objpool.free((T*)rawmemory);
        }
    };


}
#endif