#ifndef THREAD_SAFE_MAP_2011_12_23_H
#define THREAD_SAFE_MAP_2011_12_23_H

#include <boost/thread/shared_mutex.hpp>
#include "utils/Threading.h"
#include <string>

using namespace std;
using namespace boost;

namespace utils
{    
    namespace customMap{

        /**
        * @brief 该类实现一个Map的功能
        * @details 
        * 
        * <p>所有的类成员均为是线程安全</p>
        * 
        * @author   徐军
        * @editor   zld
        * @version  1.01
        * @Date [23/12/2011]
        */    
        template<typename K, typename V>
        class ThreadSafeMap
        {
            typedef map<K, V>   MapType;
        public:
            ThreadSafeMap(){};
            ~ThreadSafeMap(){};

            void addItem(const K& key, const V& value)
            {
                utils::WriteLock lock(m_mutex);
                //m_map.insert(map< K, V>::value_type(key, value));
                m_map[key] = value;
            }

            bool getItem(const K& key, V& value)
            {
                bool find = false;
                utils::ReadLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                if(iter != m_map.end())
                {
                    value = iter->second;
                    find = true;
                } else {
                    //do nothing
                }
                return find;
            }

            bool hasKey(const K& key)
            {
                utils::ReadLock lock(m_mutex);
                return m_map.find(key) != m_map.end();
            }

            void removeKey(const K& key)
            {
                utils::WriteLock lock(m_mutex);
                m_map.erase(key);
            }

            void clear()
            {
                utils::WriteLock lock(m_mutex);
                m_map.clear();
            }

        private:
            boost::shared_mutex m_mutex;
            MapType m_map;
        };
    }
}

#endif //THREAD_SAFE_MAP_2011_12_23_H

