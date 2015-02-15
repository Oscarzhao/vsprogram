/**
 *  mysql简易客户端
 *
 *  @author zhangyi
 *  @since  2012-10-24
 */

#include <common/Stdafx.h>
#include <utils/CMySQLSimpleClient.h>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>

namespace utils
{
    CMySQLSimpleClient::CMySQLSimpleClient()
        : m_connection(NULL)
    {
        
    }

    CMySQLSimpleClient::~CMySQLSimpleClient()
    {
        stop();
    }

    void CMySQLSimpleClient::start( CMySQLConf conf )
    {
        m_conf = conf;

        doConnect();
    }

    void CMySQLSimpleClient::stop()
    {
        doDisconnect();
    }

    void CMySQLSimpleClient::execute(const string& sql)
    {
        try
        {
            if (
                NULL != m_connection
                && mysql_real_query(m_connection, sql.c_str(), sql.length())
                )
            {
                int myErrno = mysql_errno(m_connection);
                switch (myErrno)
                {
                case CR_SERVER_GONE_ERROR:
                case CR_SERVER_LOST:
                    {
                        if (mysql_real_query(m_connection, sql.c_str(), sql.length()))
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
        }
        catch (...)
        {
            
        }
    }

    MYSQL_RES * CMySQLSimpleClient::query(const string& sql)
    {
        MYSQL_RES* res = NULL;

        if ( (m_connection == NULL) && (!doConnect()))
        {
            //应该是有人主动去断开连接了, 且重连失败
            //TT_STDLOG(LLV_WARN, "Failed to execute, miss connect with mysql");
            return NULL;
        }

        int myErrno = 0;
        //如果是因为连接断了,只尝试重连一次
        if (mysql_real_query(m_connection, sql.c_str(), sql.length()))
        {
            myErrno = mysql_errno(m_connection);
            if ((myErrno == CR_SERVER_LOST) || (myErrno == CR_SERVER_GONE_ERROR))
            {
                if (mysql_real_query(m_connection, sql.c_str(), sql.length()))
                {
                    //TT_STDLOG(LLV_ERROR, boost::format("Failed to query mysql, statement: %s") %sql);
                    //TT_STDLOG(LLV_ERROR, boost::format("Mysql error: %s") %(mysql_error(m_connection)));
                    return NULL;
                }
            }
            else
            {
                //TT_STDLOG(LLV_ERROR, boost::format("Failed to query mysql, statement: %s") %sql);
                //TT_STDLOG(LLV_ERROR, boost::format("Mysql error: %s") %(mysql_error(m_connection)));
                return NULL;
            }
        }

        res  = mysql_store_result( m_connection );
        if (!res)
        {
            if (mysql_field_count(m_connection) == 0)
            {
                r_int64 effectNum = mysql_affected_rows(m_connection);
                if(effectNum == -1)
                {
                    //TT_STDLOG(LLV_DEBUG, "0 rows affected");
                    return NULL;
                }
                else
                {
                    //TT_STDLOG(LLV_DEBUG, boost::format("%1% rows affected") % effectNum);
                    return res;
                }

            }
            else
            {
                //错误发生了
                //TT_STDLOG(LLV_ERROR, "Failed to retrieve result set");
                return NULL;
            }
        }

        return res;
    }

    const MYSQL * CMySQLSimpleClient::connection() const
    {
        return m_connection;
    }

    void CMySQLSimpleClient::freeQueryRes(MYSQL_RES* res)
    {
        if (res)
        {
            mysql_free_result(res);
        }
    }

    bool CMySQLSimpleClient::doConnect()
    {
        /* 关闭连接先 */
        doDisconnect();

        m_connection = mysql_init(NULL);
        if (m_connection == NULL)
        {
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
            return false;
        }

        return true;
    }

    void CMySQLSimpleClient::doDisconnect()
    {
        if (m_connection)
        {
            mysql_close(m_connection);
            m_connection = NULL;
            //TT_STDLOG(LLV_WARN, boost::format("Disconnected to mysql"));
        }
    }

}
