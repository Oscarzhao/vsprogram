/**
 *    @file   CMySQLClientPool.h
 *
 *  @brief  mysql对象池
 *          为了发挥inno db的并发性，这里提供一个mysql对象池
 *          对象池提供接口返回N个空闲的mysql对象
 *
 *  @author  zhangyi
 *  @since   2012-09-06
 */

#ifndef SERVER3_BASE_UTILS_CMYSQLCLIENTPOOLT_H
#define SERVER3_BASE_UTILS_CMYSQLCLIENTPOOLT_H

namespace utils
{
    /**
     *    mysql client对象池
     *  由于各模块可能派生CMySQLClient
     *  因此做成模板
     */
    template <class TSubClient>
    class CMySQLClientPoolT : public boost::enable_shared_from_this<CMySQLClientPoolT<TSubClient> >
    {
    protected:
        /**
         *    构造函数
         */
        CMySQLClientPoolT();

    public:

        /**
         *    析构函数
         */
        virtual ~CMySQLClientPoolT();

        /**
         *  获取实例的智能指针
         *  @return     CMySQLClientPoolT的智能指针
         */
        static boost::shared_ptr<CMySQLClientPoolT> getInstance();

        /**
         *    在调用getClients前，需要首先调用的函数，设置全局信息
         */
        void init();

        /**
         *    获取一定数目的CMySQLClient对象
         *  @see        获取到的对象已经开始运行，不能再调用start()！
         *  @return     num个CMySQLClient对象
         */
        std::vector<boost::shared_ptr<TSubClient> > getClients(size_t num);

        /**
         *    停止所有mysql client（阻塞）
         *  向
         */
        void stopAll();

    protected:

        log4cxx::LoggerPtr                  m_loggerPtr;
        boost::mutex                        m_mutex;     /* getClients操作的互斥锁 */
        std::vector<boost::shared_ptr<TSubClient> >     m_clients;   /* 当前池中所有clients */

        void destroyAll();

    private:
        
        static boost::shared_ptr<CMySQLClientPoolT> m_instance;
        static boost::recursive_mutex               m_instanceMutex;
    };

    // 基本mysql对象池
    class CMySQLClient;
    typedef CMySQLClientPoolT<CMySQLClient> CMySQLClientPool;
}

#endif
