/**
 *  订阅表
 *
 *  @author xujun
 *  @since  2013-1-13
 */
#ifndef _SubTable_2013_1_13_h__
#define _SubTable_2013_1_13_h__
#include "utils/Threading.h"
#include "utils/STLHelper.h"

namespace utils
{
    template<typename TKey, typename TValue>
    class SubTable
    {
        typedef boost::function<void (const TValue&)> SubCallback;
        typedef boost::shared_ptr<SubTable> SubTablePtr;

        class RequestInfo
        {
        public:
            TKey m_key;
            SubCallback m_callback;
        };
    public :
        typedef boost::shared_ptr<RequestInfo> RequestInfoPtr;

        class SubRequest
        {
        public:
            SubRequest(){};
            ~SubRequest() {
                boost::shared_ptr<SubTable> subTable = m_subTable.lock();
                if (NULL != subTable)
                {
                    subTable->unsubscribe(m_requestInfo->m_key, m_requestInfo);
                }
            };

        public:
            boost::weak_ptr<SubTable> m_subTable;
            RequestInfoPtr m_requestInfo;
        };
    public :
        typedef boost::shared_ptr<SubRequest> SubRequestPtr;

    public:
        SubTable() {};
        ~SubTable() {};

        RequestInfoPtr subscribe(const TKey& key, const SubCallback& callback)
        {
            RequestInfoPtr info(new RequestInfo);
            info->m_key = key;
            info->m_callback = callback;

            utils::RecLock lock(m_mutex);
            m_subs[key].push_back(info);

            return info;
        };

        void unsubscribe(const TKey& key, RequestInfoPtr request)
        {
            utils::RecLock lock(m_mutex);
            if (contain(m_subs, key))
            {
                vector<RequestInfoPtr>& requests = utils::getMapValueRef(m_subs, key);
                utils::remove(requests, request);
                if (requests.empty())
                {
                    utils::remove(m_subs, key);
                }
            }
        }

        void notify(const TKey& key, const TValue& value)
        {
            vector<RequestInfoPtr> requests;
            {
                utils::RecLock lock(m_mutex);
                requests = utils::getMapValue(m_subs, key);
            }
            for (typename vector<RequestInfoPtr>::iterator iter = requests.begin(); iter != requests.end(); ++iter)
            {
                if (NULL != *iter && NULL != (*iter)->m_callback)
                {
                    (*iter)->m_callback(value);
                }
            }
        };

        void clear()
        {
            utils::RecLock lock(m_mutex);
            m_subs.clear();
        }

    private:
        RecMutex                 m_mutex;
        map<TKey, vector<RequestInfoPtr> >  m_subs;
    };

    template<typename TKey, typename TValue>
    typename SubTable<TKey, TValue>::SubRequestPtr subscribe(boost::shared_ptr< SubTable<TKey, TValue> > subTable, const TKey& key, const boost::function<void (const TValue&)>& callback)
    {
        typename SubTable<TKey, TValue>::RequestInfoPtr request = subTable->subscribe(key, callback);
        typename SubTable<TKey, TValue>::SubRequestPtr wrapper(new typename SubTable<TKey, TValue>::SubRequest());
        wrapper->m_subTable = subTable;
        wrapper->m_requestInfo = request;
        return wrapper;
    }
}

#endif // SubTable_h__
