/**
 *  进程间同步和互斥的一些简单实现
 *
 *  @author zhangyi
 *  @since  2012-08-12
 */

#include <utils/interProcess.h>
#include <utils/TimeTool.h>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/thread/thread.hpp>
#include <set>

namespace utils
{
    //////////////////////////////////////////////////////////////////////////
    // 1. 条件变量
    //////////////////////////////////////////////////////////////////////////

    class CCond;

    class CCondCleaner
    {
    protected:
        bool bAlive_;
        set<string> conds_;
        boost::mutex aliveMutex_;
        boost::mutex condMutex_;

    public:
        CCondCleaner()
            : bAlive_(true)
        {

        }

        ~CCondCleaner()
        {
            boost::lock_guard<boost::mutex> aliveGuard(aliveMutex_);
            bAlive_ = false;

            boost::lock_guard<boost::mutex> condGuard(condMutex_);
            for (set<string>::const_iterator cIter = conds_.begin(); cIter != conds_.end(); cIter++)
            {
                boost::interprocess::shared_memory_object::remove((*cIter).c_str());
            }
        }

        void insert(const char * szCondition)
        {
            boost::lock_guard<boost::mutex> aliveGuard(aliveMutex_);
            if (bAlive_)
            {
                boost::lock_guard<boost::mutex> condGuard(condMutex_);
                conds_.insert(szCondition);
            }
        }
    };

    static CCondCleaner g_sCleaner;

    class CCond
    {
    protected:
        const char * szCondName_;
        boost::interprocess::interprocess_mutex mutex_;
        boost::interprocess::interprocess_condition cond_;

    public:
        CCond(const char * szCondName)
            : szCondName_(szCondName_)
        {

        }

        void notify()
        {
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex_);
            cond_.notify_one();
        }

        void wait()
        {
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex_);
            cond_.wait(lock);
        }
    };

    void condNotify( const char *szCondition )
    {
        try
        {
            boost::interprocess::shared_memory_object::remove(szCondition);
            boost::interprocess::shared_memory_object smo(
                boost::interprocess::create_only,
                szCondition,
                boost::interprocess::read_write
                );

            smo.truncate(sizeof(CCond));
            boost::interprocess::mapped_region region(smo, boost::interprocess::read_write);
            void * pAddr = region.get_address();
            if (NULL != pAddr)
            {
                CCond * pCond = new (pAddr) CCond(szCondition);
                if (NULL != pCond)
                {
                    pCond->notify();
                }
            }
            
            g_sCleaner.insert(szCondition);
        }
        catch (boost::interprocess::interprocess_exception &)
        {
            
        }
    }

    void condWait( const char * szCondition )
    {
        bool bSuccess = true;
        
        do 
        {
            try
            {
                boost::interprocess::shared_memory_object smo(
                    boost::interprocess::open_only,
                    szCondition,
                    boost::interprocess::read_only
                    );
                boost::interprocess::mapped_region region(smo, boost::interprocess::read_only);
                void * pAddr = region.get_address();
                if (NULL != pAddr)
                {
                    CCond * pCond = new (pAddr) CCond(szCondition);
                    if (NULL != pCond)
                    {
                        pCond->wait();
                    }
                }
            }
            catch (boost::interprocess::interprocess_exception &)
            {
                bSuccess = false;
                boost::thread::sleep(utils::fromNow(0.1));
            }
        } while (!bSuccess);
    }
}
