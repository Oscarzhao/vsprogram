/**
 * 监控系统mysql数据库操作模块
 *
 * @author   zhangyi
 * @since    2012-09-06
 */

#include <common/Stdafx.h>
#include <utils/CMySQLClient.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>
#include <boost/asio/placeholders.hpp>
#include <log4cxx/logger.h>

//#define ENABLE_MYSQL_DETAIL_LOG

namespace utils
{
    boost::shared_ptr<CMySQLClient> CMySQLClient::m_instance;
    boost::recursive_mutex CMySQLClient::m_instanceMutex;

    CMySQLClient::CMySQLClient(bool bAsync)
        : m_bAsync(bAsync),
        m_pWork(NULL),
        m_heartbeatTimer(m_ioService),
        m_writebackTimer(m_ioService),
        m_connection(NULL)
    {
        if (m_bAsync)
        {
            m_pWork = new boost::asio::io_service::work(m_ioService);
        }
    }

    CMySQLClient::~CMySQLClient()
    {
        stop();
    }

    void CMySQLClient::start( CMySQLConf conf )
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG

        m_conf = conf;

        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            boost::thread *pThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService, m_ec));
            m_thread.reset(pThread);

            m_ioService.post(boost::bind(&CMySQLClient::doConnect, this));
            m_ioService.post(boost::bind(&CMySQLClient::doStartcheck, this));
            m_ioService.post(boost::bind(&CMySQLClient::doStartWritebackCheck, this));
        }
        else
        {
            doConnect();
        }
    }

    void CMySQLClient::stop()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG

        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doDisconnect, this));
            boost::system::error_code ec;
            m_heartbeatTimer.cancel(ec);
            if(ec)
            {
                //TT_STDLOG(LLV_WARN, ec.message().c_str());
            }
            m_writebackTimer.cancel(ec);
            if(ec)
            {
                //TT_STDLOG(LLV_WARN, ec.message().c_str());
            }
            if(m_pWork)
            {
                delete m_pWork;
                m_pWork = NULL;
            }
            // m_ioService.stop();
        }
        else
        {
            doDisconnect();
        }
    }

    void CMySQLClient::waitForEnd()
    {
        m_thread->join();
    }

    bool CMySQLClient::doConnect()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_INFO, boost::format("Connecting to mysql on %s:%s") %(m_host) %(m_port));
        /* 关闭连接先 */
        doDisconnect();

        m_connection = mysql_init(NULL);
        if (m_connection == NULL)
        {
            //TT_STDLOG(LLV_ERROR, boost::format("Failed to init mysql handle"));
            return false;
        }

        if (
            NULL == mysql_real_connect(
            m_connection, m_conf.m_host.c_str(), m_conf.m_user.c_str(), m_conf.m_password.c_str(), m_conf.m_db.c_str(), 
            m_conf.m_port, NULL, CLIENT_MULTI_STATEMENTS
            ))
        {
            const char *szError = mysql_error(m_connection);
            //TT_STDLOG(LLV_ERROR, boost::format("Failed to connect mysql on %s:%s, Error: %s") %(m_host) %(m_port) %(mysql_error(m_connection)));
            return false;
        }

        //TT_STDLOG(LLV_INFO, boost::format("Connected to mysql on %s:%s") %(m_host) %(m_port));
        if (mysql_set_server_option(m_connection, MYSQL_OPTION_MULTI_STATEMENTS_ON))
        {
            //TT_STDLOG(LLV_ERROR, boost::format("Failed to set multi statements: Error: %s") %(mysql_error(m_connection)));
            return false;
        }

        my_bool reconnect = 1;
        if (mysql_options(m_connection, MYSQL_OPT_RECONNECT, &reconnect))
        {
            //TT_STDLOG(LLV_ERROR, boost::format("Failed to set reconnected: Error: %s") %(mysql_error(m_connection)));
            return false;
        }

        const char* oldCharSet = mysql_character_set_name(m_connection);
        if (!mysql_set_character_set(m_connection, "utf8"))
        {
            //TT_STDLOG(LLV_INFO, boost::format("Change CharacterSet from %1% to %2%") % oldCharSet % mysql_character_set_name(m_connection));
        }

        if (mysql_autocommit(m_connection, 0))
        {
            unsigned int myErrno = mysql_errno(m_connection);
            //TT_STDLOG(LLV_ERROR, boost::format("Close Auto-Commit, error: %1%") % mysql_error(m_connection));
        }
        else
        {
            //TT_STDLOG(LLV_INFO, "Close Auto-Commit");
        }
        return true;
    }

    void CMySQLClient::doDisconnect()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_connection)
        {
            mysql_close(m_connection);
            m_connection = NULL;
            //TT_STDLOG(LLV_WARN, boost::format("Disconnected to mysql"));
        }
    }

    void CMySQLClient::execute(const string& sql, bool commit)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG

        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doSingleSqlExecute, 
                this, 
                sql,
                commit));
        }
        else
        {
            doSingleSqlExecute(sql, commit);
        }
    }

    void CMySQLClient::execute(const string& sql, std::vector<boost::any>& param, bool commit)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doExecuteBindAux, 
                this, 
                sql,
                param,
                commit));
        }
        else
        {
            doExecuteBindAux(sql, param, commit);
        }
    }

    void CMySQLClient::lateExecute(const string& sql)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doLateExecute, 
                this, 
                sql));
        }
        else
        {
            doLateExecute(sql);
        }
    }

    void CMySQLClient::commit()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doCommit, 
                this));
        }
        else
        {
            doCommit();
        }
    }

    bool CMySQLClient::isFree() const
    {
        boost::mutex::scoped_lock lock(m_mutex, boost::defer_lock_t());
        return lock.try_lock();
    }

    void CMySQLClient::doLateExecute(string sql)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        m_lateSqlList.push_back(sql);
        vector<string>::size_type n = m_lateSqlList.size();
        if(n < MYSQL_LATE_EXECUTE_NUM)
            return;
        doLateExecuteAux();
        return;
    }

    boost::asio::io_service & CMySQLClient::ioService()
    {
        return m_ioService;
    }

    const MYSQL * CMySQLClient::connection() const
    {
        return m_connection;
    }

    void CMySQLClient::doSingleSqlExecute(string sql, bool bCommit)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        int myErrno = 0;
        LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), (boost::format("MySQL execute : connection = %X") % m_connection).str());
        if (mysql_real_query(m_connection, sql.c_str(), sql.length()))
        {
            myErrno = mysql_errno(m_connection);
            switch (myErrno)
            {
            case CR_SERVER_GONE_ERROR:
            case CR_SERVER_LOST:
                {
                    doPing();
                    if(mysql_real_query(m_connection, sql.c_str(), sql.length()))
                    {
                        //TT_STDLOG(LLV_ERROR, "Failed to execute single mysql, statement:");
                        //TT_STDLOG(LLV_ERROR, sql.c_str());
                        //TT_STDLOG(LLV_ERROR, boost::format("MySQL Error: %s") %(mysql_error(m_connection)));
                    }
                }
                break;
            case CR_COMMANDS_OUT_OF_SYNC:
            case CR_UNKNOWN_ERROR:
            default:
                {
                    //TT_STDLOG(LLV_ERROR, "Failed to execute single mysql, statement:");
                    //TT_STDLOG(LLV_ERROR, sql.c_str());
                    //TT_STDLOG(LLV_ERROR, boost::format("MySQL Error: %s") %(mysql_error(m_connection)));
                }
            }
        }

        if(bCommit)
            doCommit();
    }

    void CMySQLClient::doLateExecuteAux()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        static string s_delimit(";");

        vector<string>::size_type n = m_lateSqlList.size();
        if(0 == n)
        {

            return;
        }
        string multiSql;
        multiSql.reserve(1024 * MYSQL_LATE_EXECUTE_NUM);
        for(vector<string>::size_type i = 0; i < n; ++i)
        {
            multiSql += m_lateSqlList[i];
            multiSql += s_delimit;
        }
        
        unsigned int myErrno = 0;
        if (mysql_real_query(m_connection, multiSql.c_str(), multiSql.length()))
        {
            myErrno = mysql_errno(m_connection);
            if(myErrno == CR_SERVER_GONE_ERROR || myErrno == CR_SERVER_LOST)
            {
                doPing();
                if(mysql_real_query(m_connection, multiSql.c_str(), multiSql.length()))
                {
                    //TT_STDLOG(LLV_ERROR, "Failed to execute multi mysql, statement:");
                    //TT_STDLOG(LLV_ERROR, multiSql.c_str());
                    m_lateSqlList.clear();
                    //TT_STDLOG(LLV_ERROR, boost::format("MySQL Error: %s") %(mysql_error(m_connection)));
                    return;
                }
            }
            else
            {
                //TT_STDLOG(LLV_ERROR, boost::format("MySQL Error: %s") %(mysql_error(m_connection)));
            }
        }

        vector<string>::size_type p = 0;
        do
        {
            MYSQL_RES* result = mysql_store_result(m_connection);
            if(result)
            {
                mysql_free_result(result);
            }
            else
            {
                if (mysql_field_count(m_connection) == 0)
                {
                    r_int64 effectNum = mysql_affected_rows(m_connection);
                    if(effectNum == -1)
                    {
                        //TT_STDLOG(LLV_DEBUG, "0 rows affected");
                    }
                    else
                    {
                        //TT_STDLOG(LLV_DEBUG, boost::format("%1% rows affected") % effectNum);
                    }
                    
                }
                else
                {
                    //错误发生了
                    //TT_STDLOG(LLV_ERROR, "Failed to retrieve result set");
                }
            }
            ++p;
            if(p >= n)
                break;
            int more = 0;
            /* more results? -1 = no, >0 = error, 0 = yes (keep looping) */
            if ((more = mysql_next_result(m_connection)) > 0)
            {
                //TT_STDLOG(LLV_ERROR, "Failed to execute multi mysql, try execute single sql");
                for(; p < n; ++p)
                {
                    string& sql = m_lateSqlList[p];
                    doSingleSqlExecute(sql, false);
                }
                break;
            }
            
        }while(true);

        doCommit();
        m_lateSqlList.clear();

        return;
    }

    void CMySQLClient::doExecuteBindAux(string sql, std::vector<boost::any> param, bool commit)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        std::vector<boost::any>::iterator itrEnd = param.end();
        std::vector<MYSQL_BIND> vecMysqlBind;

        try
        {
            for(std::vector<boost::any>::iterator itr = param.begin(); itr != itrEnd; ++itr)
            {   
                boost::any& param = *itr;
                vecMysqlBind.push_back(boost::any_cast<MYSQL_BIND>(param));
            }
        }
        catch(...)
        {
            return;
        }

        MYSQL_STMT *stmt = mysql_stmt_init(m_connection);   
        if (!stmt)   
        {   
            //TT_STDLOG(LLV_ERROR, "Failed to init stmt, out of memory");
            return;
        }

        int myErrno = 0;
        if ( mysql_stmt_prepare(stmt, sql.c_str(), sql.length()))   
        {   
            myErrno = mysql_errno(m_connection);
            if((myErrno == CR_SERVER_LOST) || (myErrno == CR_SERVER_GONE_ERROR))
            {
                //尝试重连一次
                doPing();
                if(mysql_stmt_prepare(stmt, sql.c_str(), sql.length()))
                {
                    //是否需要保存在队列？
                    //TT_STDLOG(LLV_ERROR, boost::format("Failed to prepare stmt, %s") % mysql_stmt_error(stmt));
                    return;
                }
            }
            else
            {
                //TT_STDLOG(LLV_ERROR, boost::format("Failed to prepare stmt, %s") % mysql_stmt_error(stmt));
                return;
            }
        }

        if ( mysql_stmt_bind_param(stmt, (MYSQL_BIND*)&vecMysqlBind[0]))
        {
            //TT_STDLOG(LLV_ERROR, boost::format("Failed to bind param, %s") % mysql_stmt_error(stmt));
            return;
        }

        if ( mysql_stmt_execute(stmt))
        {
            myErrno = mysql_errno(m_connection);
            
            if((myErrno == CR_SERVER_LOST) || (myErrno == CR_SERVER_GONE_ERROR))
            {
                //尝试重连一次
                doPing();
                if(mysql_stmt_execute(stmt))
                {
                    //TT_STDLOG(LLV_ERROR, boost::format("Failed to prepare stmt, %s") % mysql_stmt_error(stmt));
                    return;
                }
            }
            else
            {
                //TT_STDLOG(LLV_ERROR, boost::format("Failed to execute stmt, %s") % mysql_stmt_error(stmt));
                return;
            }
        }
        mysql_stmt_free_result(stmt);
        if(commit)
            doCommit();
        return;
    }

    void CMySQLClient::query(const string& sql, TQueryCallback callback)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doQuery, 
                this, sql, callback));
        }
        else
        {
            doQuery(sql, callback);
        }
    }

    void CMySQLClient::doQuery(string sql, TQueryCallback callback)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        MYSQL_RES* res = NULL;

        if ( (m_connection == NULL) && (!doConnect()))
        {
            //应该是有人主动去断开连接了, 且重连失败
            //TT_STDLOG(LLV_WARN, "Failed to execute, miss connect with mysql");
            callback(res, false);
            return;
        }

        int myErrno = 0;
        //如果是因为连接断了,只尝试重连一次
        if(mysql_real_query(m_connection, sql.c_str(), sql.length()))
        {
            myErrno = mysql_errno(m_connection);
            if((myErrno == CR_SERVER_LOST) || (myErrno == CR_SERVER_GONE_ERROR))
            {
                doPing();
                if(mysql_real_query(m_connection, sql.c_str(), sql.length()))
                {
                    //TT_STDLOG(LLV_ERROR, boost::format("Failed to query mysql, statement: %s") %sql);
                    //TT_STDLOG(LLV_ERROR, boost::format("Mysql error: %s") %(mysql_error(m_connection)));
                    callback(res, false);
                    return;
                }
            }
            else
            {
                //TT_STDLOG(LLV_ERROR, boost::format("Failed to query mysql, statement: %s") %sql);
                //TT_STDLOG(LLV_ERROR, boost::format("Mysql error: %s") %(mysql_error(m_connection)));
                callback(res, false);
                return;
            }
        }

        res  = mysql_store_result( m_connection );
        if(!res)
        {
            if (mysql_field_count(m_connection) == 0)
            {
                r_int64 effectNum = mysql_affected_rows(m_connection);
                if(effectNum == -1)
                {
                    //TT_STDLOG(LLV_DEBUG, "0 rows affected");
                    callback(res, false);
                    return;
                }
                else
                {
                    //TT_STDLOG(LLV_DEBUG, boost::format("%1% rows affected") % effectNum);
                    callback(res, true);
                    return;
                }
                
            }
            else
            {
                //错误发生了
                //TT_STDLOG(LLV_ERROR, "Failed to retrieve result set");
                callback(res, false);
                return;
            }
        }
        callback(res, true);
        return;
    }

    void CMySQLClient::freeQueryRes(MYSQL_RES* res)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, to post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        // boost::lock_guard<boost::mutex> guard(m_mutex);
        if (m_bAsync)
        {
            boost::lock_guard<boost::mutex> guard(m_mutex);
            m_ioService.post(boost::bind(&CMySQLClient::doFreeQueryRes, 
                this, res));
        }
        else
        {
            doFreeQueryRes(res);
        }
    }

    void CMySQLClient::doFreeQueryRes(MYSQL_RES* res)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (res)
        {
            mysql_free_result(res);
        }
    }

    void CMySQLClient::doPing()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        unsigned long olderId = 0, newId = 0;
        if (m_connection)
        {
            olderId = mysql_thread_id(m_connection);
            mysql_ping(m_connection);
            newId = mysql_thread_id(m_connection);
        }

        if (olderId != newId) //发生了自动重连
        {   
            //TT_STDLOG(LLV_WARN, "Mysql reconnect");
        }
    }

    void CMySQLClient::doCommit()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if (m_connection)
        {
            mysql_commit(m_connection);
        }
    }

    void CMySQLClient::doStartcheck()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        ////TT_STDLOG(LLV_INFO, "Mysql connection status checked");
        doPing();
        m_heartbeatTimer.expires_from_now(boost::posix_time::seconds(MYSQL_CHECK_INTERVAL));
        m_heartbeatTimer.async_wait(boost::bind(&CMySQLClient::doCheck, this, boost::asio::placeholders::error));
    }

    void CMySQLClient::doCheck(const boost::system::error_code& err)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if(!err)
        {
            ////TT_STDLOG(LLV_INFO, "Mysql connection status checked");
            doPing();
            m_heartbeatTimer.expires_from_now(boost::posix_time::seconds(MYSQL_CHECK_INTERVAL));
            m_heartbeatTimer.async_wait(boost::bind(&CMySQLClient::doCheck, this, boost::asio::placeholders::error));
        }
    }

    void CMySQLClient::doStartWritebackCheck()
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        m_writebackTimer.expires_from_now(boost::posix_time::seconds(MYSQL_EXECUTE_INTERVAL));
        m_writebackTimer.async_wait(boost::bind(&CMySQLClient::doWritebackCheck, 
            this, 
            boost::asio::placeholders::error));
    }

    void CMySQLClient::doWritebackCheck(const boost::system::error_code& err)
    {
#ifdef ENABLE_MYSQL_DETAIL_LOG
        //TT_STDLOG(LLV_DEBUG, boost::format("%1% : this = %2%, AFTER post.") % m__m_FUNCTION_ % this);
#endif // ENABLE_MYSQL_DETAIL_LOG
        if(!err)
        {
            ////TT_STDLOG(LLV_INFO, "Mysql writeback status checked");
            doLateExecuteAux();
            m_writebackTimer.expires_from_now(boost::posix_time::seconds(MYSQL_EXECUTE_INTERVAL));
            m_writebackTimer.async_wait(boost::bind(&CMySQLClient::doWritebackCheck, 
                this, 
                boost::asio::placeholders::error));
        }
    }
}
