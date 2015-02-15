#ifndef VECTOR_MAP_2009_09_01_H
#define VECTOR_MAP_2009_09_01_H

#include <boost/thread/shared_mutex.hpp>

using namespace std;
using namespace boost;

namespace utils
{    
    namespace customMap{

        /**
        * @brief 该类实现一个Map的功能, 其中Map的Value是一个vector,
        * @details 
        * 
        * <p>所有的类成员均为是线程安全</p>
        * 
        * @author   徐军
        * @version  1.0
        * @Date [9/3/2009]
        */    
        template<typename K, typename V>
        class VectorMap
        {
            typedef map<K, vector<V> >   MapType;
        public:
            VectorMap(){};
            ~VectorMap(){};

            //返回值代表在map中是否产生一个新的Key
            bool addItem(const K& key, const V& value)
            {
                bool genNewKey = false;
                utils::WriteLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                if(iter != m_map.end())
                {
                    vector<V>& values = iter->second;
                    typename vector<V>::iterator valueIter = find(values.begin(), values.end(), value);
                    if(valueIter == values.end())
                    {
                        values.push_back(value);
                    } else {
                        //do nothing
                    }
                } else {
                    m_map[key] = vector<V>(1, value);
                    genNewKey = true;
                }
                return genNewKey;
            }

            vector<V> getItems()
            {
                vector<V> result;
                utils::ReadLock lock(m_mutex);
                for(typename MapType::iterator itr = m_map.begin(); itr != m_map.end(); ++itr)
                {
                    result.insert(result.end(), itr->second.begin(), itr->second.end());
                }
                return result;
            }

            vector<V> getItems(const K& key)
            {
                utils::ReadLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                if(iter != m_map.end())
                {
                    return iter->second;
                } else {
                    return vector<V>();
                }
            }

            //返回值代表是否在map中删除一个key
            bool remove(const K& key, const V& value)
            {
                bool deleteKey = false;
                utils::WriteLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                if(iter != m_map.end())
                {
                    vector<V>& values = iter->second;
                    typename vector<V>::iterator valueIter = find(values.begin(), values.end(), value);
                    if(valueIter != values.end())
                    {
                        values.erase(valueIter);
                        if(values.size() == 0)
                        {
                            m_map.erase(key);
                            deleteKey = true;
                        }
                    } else {
                        //do nothing
                    }        
                } else {
                    // do nothing
                }
                return deleteKey;
            }

            bool removeKey(const K& key)
            {
                utils::WriteLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                if(iter != m_map.end())
                {    
                    m_map.erase(key);
                } else {
                    // do nothing
                }
                return true;
            }

            bool removeValue(const V& value)
            {
                vector<K> vec;
                utils::WriteLock lock(m_mutex);

                typename MapType::iterator countIter = m_map.begin();
                typename MapType::iterator iter;
                while (countIter != m_map.end())
                {
                    iter = countIter++;
                    vector<V>& values = iter->second;
                    typename vector<V>::iterator valueIter = find(values.begin(), values.end(), value);
                    if(valueIter != values.end())
                    {
                        values.erase(valueIter);
                        if(values.size() == 0)
                        {
                            vec.push_back(iter->first);
                            m_map.erase(iter);
                        }
                    } else {
                        //do nothing
                    }
                }
                return true;
            }

            vector<K> getKeys()
            {
                vector<K> keys;
                utils::ReadLock lock(m_mutex);
                for (typename MapType::iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
                {
                    keys.push_back(iter->first);
                }
                return keys;
            }

            bool hasKey(const K& key)
            {
                utils::ReadLock lock(m_mutex);
                typename MapType::iterator iter = m_map.find(key);
                return iter != m_map.end();
            }

            void clear()
            {
                utils::WriteLock lock(m_mutex);
                m_map.clear();
            }

        private:
            utils::RWMutex m_mutex;
            MapType m_map;
        };
    }
}

#endif //VECTOR_MAP_2009_09_01_H

