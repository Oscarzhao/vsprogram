/**
 *    @file   CMySQLClientPool.inl
 *
 *  @brief  mysql对象池
 *          为了发挥inno db的并发性，这里提供一个mysql对象池
 *          对象池提供接口返回N个空闲的mysql对象
 *
 *  @author zhangyi
 *  @since  2012-09-06
 */

#include <corestdafx.h>
#include <global/CMySQLClientPool.h>

namespace utils
{
    template <class TSubClient>
    boost::shared_ptr<CMySQLClientPoolT<TSubClient> > CMySQLClientPoolT<TSubClient>::m_instance;

    template <class TSubClient>
    boost::recursive_mutex CMySQLClientPoolT<TSubClient>::m_instanceMutex;

    template <class TSubClient>
    CMySQLClientPoolT<TSubClient>::CMySQLClientPoolT()
    {

    }

    template <class TSubClient>
    CMySQLClientPoolT<TSubClient>::~CMySQLClientPoolT()
    {
        stopAll();
        destroyAll();
    }

    template <class TSubClient>
    boost::shared_ptr<CMySQLClientPoolT<TSubClient> > CMySQLClientPoolT<TSubClient>::getInstance()
    {
        if (m_instance.get() == NULL)
        {
            utils::RecLock rlock(m_instanceMutex);
            if (m_instance.get() == NULL)
            {
                m_instance.reset(new CMySQLClientPoolT());
            }
        }
        return m_instance;
    }

    template <class TSubClient>
    void CMySQLClientPoolT<TSubClient>::init()
    {
        
    }

    /**
     *  当前实现：
     *  for循环遍历池中所有clients（数量级M）。
     *  判断每一个client是否空闲，如是，加入返回列表中。
     *  如果结果数目不够N，则扩充池。
     *
     *  问题：
     *  for循环效率。
     *  如果当前池中空闲client很少，则需要新生成的client较多。
     *  最差时间复杂度：O(M+N)
     *
     *  可能的改进：
     *  每一个client加一个id。
     *  每个client在“开始工作”和“空闲”时分别调用pool的某个接口，
     *  通知pool“我开始工作了”或“我空闲了”。
     *  pool在这两个接口的实现中，维护两个map来保存所有“工作中”和“空闲”client集合。
     *  这样的最差时间复杂度：O(N)
     *
     *  可能的改进的问题：
     *  因为每个client的post操作用时很短，
     *  故目前的实现时间复杂度达到O(M+N)的前提是，
     *  短时间内大量并发的线程同时请求很多很多query。
     *  目前可以暂时认为这个可能性不高。
     *  同时，加入上述两个表意味着pool要引入几个新的锁，这些本身也是开销，
     *  故暂不改进。
     */
    template <class TSubClient>
    std::vector<boost::shared_ptr<TSubClient> > CMySQLClientPoolT<TSubClient>::getClients( size_t num )
    {
        boost::lock_guard<boost::mutex> guard(m_mutex);

        std::vector<boost::shared_ptr<TSubClient> > result;
        result.resize(num);
        size_t iOut = 0;

        for (std::vector<boost::shared_ptr<TSubClient> >::const_iterator cIt = m_clients.begin(); cIt != m_clients.end(); cIt++)
        {
            boost::shared_ptr<TSubClient> ptrClient = *cIt;
            if (ptrClient->isFree())
            {
                result[iOut] = *cIt;
                iOut++;
                if (iOut >= result.size())
                {
                    // 在目前的列表中已经找到足够多clients
                    break;
                }
            }
        }

        if (iOut < result.size())
        {
            // clients数目不够
            size_t more = result.size() - iOut;
            size_t iPool = m_clients.size();
            LOG4CXX_DEBUG(
                m_loggerPtr,
                boost::format("[CMySQLClientPoolT] Only %1% clients found in current %2% clients, %3% more generated.") % iOut % m_clients.size() % more
                );
            m_clients.resize(m_clients.size() + more);
            for (; iPool < m_clients.size() && iOut < result.size(); iPool++, iOut++)
            {
                boost::shared_ptr<TSubClient> ptrClient(new TSubClient());
                ptrClient->start();
                m_clients[iPool] = ptrClient;
                result[iOut] = ptrClient;
            }
        }
        else
        {
            LOG4CXX_DEBUG(
                m_loggerPtr,
                boost::format("[CMySQLClientPoolT] Enough %1% clients found in current %2% clients.") % num % m_clients.size()
                );
        }

        return result;
    }

    template <class TSubClient>
    void CMySQLClientPoolT<TSubClient>::stopAll()
    {
        for (std::vector<boost::shared_ptr<TSubClient> >::iterator it = m_clients.begin(); it != m_clients.end(); it++)
        {
            (*it)->stop();
        }
        for (std::vector<boost::shared_ptr<TSubClient> >::iterator it = m_clients.begin(); it != m_clients.end(); it++)
        {
            (*it)->waitForEnd();
        }
    }

    template <class TSubClient>
    void CMySQLClientPoolT<TSubClient>::destroyAll()
    {
        m_clients.clear();
    }

}
