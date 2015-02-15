/**
 * 监控系统mysql数据库操作模块
 *
 * @author   zhangyi
 * @since    2012-09-06
 *
 * 所有外部接口均线程安全。
 * 使用示例：
 *
 * 1. 运行模块
 * CMySQLClient* pMysqlClient = CMySQLClient::getInstance();
 * pMysqlClient->start();
 * pMysqlClient->waitForEnd();
 *
 * 2. 停止模块
 * pMysqlClient->stop(); 
 *  
 * 3. 查询
 * void process_query_result(mysql_res* res, bool status) 
 * { 
 *     //do something; 
 *     pMysqlClient->freeQueryRes(res);
 * }
 * pMysqlClient->query(sql, boost::bind(process_query_result, _1, _2)); 
 *
 * 4. 执行非查询语句
 * pMysqlClient->execute(sql);
 *
 * 5. 配置要求：
 * [mysql]
 * host=
 * port=
 * db=
 * user=
 * password=
 *
 */

#ifndef SERVER3_BASE_UTILS_CMYSQLCLIENT_H
#define SERVER3_BASE_UTILS_CMYSQLCLIENT_H

#include <utils/mysqlConf.h>
#include <boost/asio/deadline_timer.hpp>

using namespace std;

namespace utils
{
    #define MYSQL_CHECK_INTERVAL 20
    #define MYSQL_EXECUTE_INTERVAL 20
    #define MYSQL_LATE_EXECUTE_NUM 1000

    typedef boost::function<void(MYSQL_RES*,bool)> TQueryCallback;

    /**
     * 对mysql c-api的封装
     * 所有的接口都是异步的
     * 
     * zhangyi revision
     * @version 1.1
     * @see 代码从monitor中分离
     * @see 传入IConfiguration指针作为构造参数
     * @see 加入是否正在工作的判断m_mutex，使不可能有两个线程同时执行post，加强线程安全
     * @see 修改成员命名风格
     * @since 2012-09-06
     */
    class CMySQLClient : private boost::noncopyable
    {
    public:
        CMySQLClient(bool bAsync);
        virtual ~CMySQLClient();

    public:

        /**
         * 创建新线程，模块运行于该线程。
         */
        void start(CMySQLConf conf);

        /**
         * 停止模块，断开和数据库服务器的连接 
         */
        void stop();

        /**
        * 等待模块线程结束
        */
        void waitForEnd();

        /**
         * 执行非查询sql语句
         * sql语句是单条语句，而且结尾不能为;和/g
         * 
         * @param sql    sql语句
         * @param commit 是否立刻提交事务
         * 
         * @see 如果语句中含有blob，longblob或者是包含转义字符的语句，必须用其他接口
         * @see Mysql c-api中单行语句不能包含;和\g，原文是: sql must 
         *      consist of a single SQL statement and you should
         *         not add a terminating semicolon (“;”) or \g to the
         *         statement
         */
        void execute(const string& sql, bool commit);
        
        /**
         * 执行非查询sql语句
         * sql语句是单条语句，而且结尾不能为;和\g
         * 
         * @param sql    sql预处理语句
         * @param param  MYSQL_BIND的vector
         * @param commit 是否立刻提交事务
         * 
         * @see 这个接口性能较差，因为这个接口并非多条语句一次执行的。所以当需要存blob，longblob和其他二进制数据，需要转义的数据的时候才使用这个接口
         */
        void execute(const string& sql, std::vector<boost::any>& param, bool commit);

        /**
         * @brief 延时执行非查询sql语句
         * 
         * 该函数主要是为了提高效率， 按照MYSQL_EXECUTE_INTERVAL指定的时间
         * 间隔，当条数大于MYSQL_LATE_EXECUTE_NUM或者MYSQL_EXECUTE_INTERVAL
         * 指定的时间间隔到了，将多个单条语句拼成多行语句执行。
         * 这是一个异步的方法，真正操作在模块线程完成。
         * 如果sql执行失败，会将sql语句打印到日志，用户自己用脚本导入。
         * 如果是client掉线，则会多一次尝试重连。
         * 
         * @see 即使没有性能的问题也应该用这个函数，因为这个函数保证了这
         * 样一直sql缓存策略，按时间和数据条数，来保证sql语句的缓存不会占
         * 用太大的内存空间，当数据库服务器lost或是gone away的时候，会把语
         * 句持久化下来
         * 
         * @warn 使用这个函数可能会产生Packet too large的错误，服务器运维的
         * 必须按照http://dev.mysql.com/doc/refman/5.0/en/packet-too-large.html
         * 来配置服务器
         * @author junjunchai
         * @param sql    数据库语句
         * 
         * @see 如果多行语句执行失败，接口内部会从失败的单行语句开始，逐一执行语句
         */
        void lateExecute(const string& sql);

        /**
        * 执行查询sql语句
        * @return 返回查询结果集
        * @remark 该函数为异步函数。
        *         1\ 查询结果集的释放须调用freeQueryRes()函数。
        *         2\ 支持二进制数据, 比如\0, 更多的需要转义的接口暂时没有加
        */
        void query(const string& sql, TQueryCallback callback);
        
        /**
        * 释放查询结果集
        */
        void freeQueryRes(MYSQL_RES* res);

        /**
        * 提交事务
        */
        void commit();

        /**
         *    是否正空闲
         *  @return     是否正在往工作线程post任务
         */
        bool isFree() const;

        boost::asio::io_service & ioService();

        const MYSQL * connection() const;

    private:

        /* 连接数据库 */
        bool doConnect();

        /* 关闭数据库 */
        void doDisconnect();

        /* 提交事务 */
        void doCommit();

        /* ping数据库 */
        void doPing();

        /* 模块定时检查 */
        void doStartcheck();
        void doCheck(const boost::system::error_code& err);
        void doStartWritebackCheck();
        void doWritebackCheck(const boost::system::error_code& err);

        /* 查询结果集 */
        void doQuery(string sql, TQueryCallback callback);

        void doFreeQueryRes(MYSQL_RES* res);
        /* 执行缓存中的所有sql语句 */

        void doLateExecute(string sql);
        void doLateExecuteAux();
        void doSingleSqlExecute(string sql, bool bCommit);

        void doExecuteBindAux(string sql, std::vector<boost::any> param, bool commit);

    private:
        
        static boost::shared_ptr<CMySQLClient>  m_instance;          /* 实例 */
        static boost::recursive_mutex           m_instanceMutex;     /* 实例锁 */

    private:
        mutable boost::mutex                    m_mutex;             /* 自身对象mutex */
        boost::asio::io_service                 m_ioService;         /* io_service */
        boost::asio::io_service::work*          m_pWork;            /* io_service work */
        boost::shared_ptr<boost::thread>        m_thread;            /* 工作线程 */
        boost::asio::deadline_timer             m_heartbeatTimer;
        boost::asio::deadline_timer             m_writebackTimer;
        vector<string>                          m_lateSqlList;
        MYSQL*                                  m_connection;        /* mysql数据库连接句柄 */
        
        CMySQLConf                              m_conf;
        bool                                    m_bAsync;

        boost::system::error_code               m_ec;
    };
}

#endif // m__MONITOR_MYSQL_CLIENT_m_H_

