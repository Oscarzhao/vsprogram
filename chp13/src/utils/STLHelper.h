/**
 *  
 *
 *  @author xujun
 *  @since  2012-8-1
 */
#ifndef _STLHelper_2012_8_1_h__
#define _STLHelper_2012_8_1_h__

#include <map>
#include <set>
#include <string>
using namespace std;

namespace utils
{
    template <typename Key, typename Value>
    Value getMapValue(const map<Key, Value>& m, const Key& key, const Value& defautV = Value())
    {
        Value d = defautV;
        typename map<Key, Value>::const_iterator iter = m.find(key);
        if (iter != m.end())
        {
            d = iter->second;
        }
        return d;
    }

    template <typename Key, typename Value>
    Value& getMapValueRef(map<Key, Value>& m, const Key& key)
    {
        typename map<Key, Value>::iterator iter = m.find(key);
        if (iter != m.end())
        {
            return iter->second;
        } else {
            static Value d;
            return d;
        }
    }


    template <typename Key, typename Value>
    const Value& getMapValueConstRef(const map<Key, Value>& m, const Key& key)
    {
        typename map<Key, Value>::const_iterator iter = m.find(key);
        if (iter != m.end())
        {
            return iter->second;
        } else {
            static Value d;
            return d;
        }
    }

    template <typename Key, typename Value>
    bool remove(map<Key, Value>& m, const Key& key)
    {
        typename map<Key, Value>::iterator iter = m.find(key);
        if (iter != m.end())
        {
            m.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool remove(std::set<T>& m, const T& key)
    {
        typename set<T>::iterator iter = m.find(key);
        if (iter != m.end())
        {
            m.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool remove(vector<T>& m, const T& key)
    {
        bool removed = false;
        for (typename vector<T>::iterator iter = m.begin(); iter != m.end(); )
        {
            if (*iter == key)
            {
                iter = m.erase(iter);
                removed = true;
            }
            else
            {
                iter++;
            }
        }
        return removed;        
    }

    template <typename T>
    bool remove(list<T>& m, const T& key)
    {
        bool removed = false;
        for (typename list<T>::iterator iter = m.begin(); iter != m.end(); )
        {
            if (*iter == key)
            {
                iter = m.erase(iter);
                removed = true;
            }
            else
            {
                iter++;
            }
        }
        return removed;     
    }

    template <typename T>
    bool removeOne(list<T>& m, const T& key)
    {
        typename list<T>::iterator iter = find(m.begin(), m.end(), key);
        if (iter != m.end())
        {
            m.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool removeOne(vector<T>& m, const T& key)
    {
        typename vector<T>::iterator iter = find(m.begin(), m.end(), key);
        if (iter != m.end())
        {
            m.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool contain(const set<T>& m, const T& key)
    {
        typename set<T>::const_iterator iter = m.find(key);
        if (iter != m.end())
        {
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool contain(const vector<T>& m, const T& key)
    {
        typename vector<T>::const_iterator iter = std::find(m.begin(), m.end(), key);
        if (iter != m.end())
        {
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool contain(const list<T>& m, const T& key)
    {
        typename list<T>::const_iterator iter = std::find(m.begin(), m.end(), key);
        if (iter != m.end())
        {
            return true;
        } else {
            return false;
        }
    }

    template <typename Key, typename Value>
    bool contain(const map<Key, Value>& m, const Key& key)
    {
        typename map<Key, Value>::const_iterator iter = m.find(key);
        if (iter != m.end())
        {
            return true;
        } else {
            return false;
        }
    }

    template <typename Key, typename Value>
    vector<Key> keys(const map<Key, Value>& m)
    {
        vector<Key> ret(m.size());
        if (!m.empty())
        {
            int index = 0;
            for (typename map<Key, Value>::const_iterator iter = m.begin(); iter != m.end(); ++iter, ++index)
            {
                ret[index] = iter->first;
            }
        }
        return ret;
    }

    template <typename Key, typename Value>
    vector<Value> values(const map<Key, Value>& m)
    {
        vector<Value> ret(m.size());
        if (!m.empty())
        {
            int index = 0;
            for (typename map<Key, Value>::const_iterator iter = m.begin(); iter != m.end(); ++iter, ++index)
            {
                ret[index] = iter->second;
            }
        }
        return ret;
    }

    template< class CurElement>
    inline static void ExcludeDuplicates(vector< CurElement >& val)
    {
        set< CurElement > tempSet;
        sort(val.begin(), val.end());
        tempSet.insert(val.begin(), val.end());
        val.resize(tempSet.size());
        int i = 0;
        for (typename set< CurElement >::iterator iter = tempSet.begin(); iter != tempSet.end(); iter++)
        {
            val[i] = *iter;
            i++;
        }
        tempSet.clear();
    }


    template <typename Key, typename Value>
    void replace(std::map<Key, Value>& m, const Key& key, const Value& value)
    {
        std::pair<typename std::map<Key, Value>::iterator, bool> iret = m.insert(make_pair(key, value));
        if (!iret.second)
        {
            iret.first->second = value;
        }
    }


    template <typename Key, typename Value>
    void replacePtr(std::map<Key, boost::shared_ptr<Value> >& m, const Key& key, const Value& value)
    {
        boost::shared_ptr<Value> ptrValue = utils::getMapValue(m, key);
        if (NULL != ptrValue)
        {
            *ptrValue = value;
        } else {
            replace(m, key, boost::shared_ptr<Value>(new Value(value)));
        }
    }

    template <typename Key, typename Value>
    bool tryReplace(map<Key, Value>& m, const Key& key, const Value& value)
    {
        pair<typename map<Key, Value>::iterator, bool> ret = m.insert(make_pair(key, value));
        return ret.second;
    }

    template <typename Key, typename Base, typename Extend>
    void shared_dynamic_cast(const map<Key, boost::shared_ptr<Base> >& raw, map<Key, boost::shared_ptr<Extend> >& ret)
    {
        for (typename map<Key, boost::shared_ptr<Base> >::const_iterator iter = raw.begin(); iter != raw.end(); ++iter)
        {
            if (NULL != iter->second)
            {
                ret.insert(make_pair(iter->first, boost::shared_dynamic_cast<Extend>(iter->second)));
            }
        }
    }

    //template <typename Base, typename Extend>
    //vector<boost::shared_ptr<Extend> > shared_dynamic_cast(const vector<boost::shared_ptr<Base> >& raw)
    //{
    //    vector<boost::shared_ptr<Extend> > ret(raw.size());
    //    for (size_t i = 0; i < raw.size(); ++i)
    //    {
    //        ret[i] = boost::shared_dynamic_cast<Extend>(raw[i]);
    //    }
    //    return ret;
    //}


}

#endif // STLHelper_h__