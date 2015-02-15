#include <DBMySQL/DBMySQL.h>
#include <utils/TimeTool.h>
#include <utils/strings.h>

namespace ttservice
{
    CMySQL::CMySQL(const struct MySQLLoginInfo& info, std::string mysqlTableName, boost::asio::io_service& ios)
        :m_mysqlLoginInfo(info), m_mysqlTable(mysqlTableName), m_timer(ios)
    {
    }

    CMySQL::~CMySQL()
    {
        if(NULL != m_mysql)
        {
            mysql_close(m_mysql);
        }
    }
    
    bool CMySQL::init()
    {
        MYSQL* mysql = NULL;
        if(
            (mysql = mysql_init(NULL))
            && mysql_real_connect(mysql, m_mysqlLoginInfo.serverIp.c_str(), m_mysqlLoginInfo.UserName.c_str(), m_mysqlLoginInfo.Password.c_str(), m_mysqlLoginInfo.DatabaseName.c_str(), m_mysqlLoginInfo.serverPort, NULL, CLIENT_MULTI_STATEMENTS)
          )
        {
            if(mysql_set_server_option(mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON))
            {
                m_mysql = NULL;
                return false;
            }

            my_bool reconnect = 1;
            if(mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect))
            {
                m_mysql = NULL;
                return false;
            }

            if(mysql_set_character_set(mysql, "utf8"))
            {
                m_mysql = NULL;
                return false;
            }
        }
        else
        {
            m_mysql = NULL;
            return false;
        }

        m_mysql = mysql;

        m_timer.expires_from_now(boost::posix_time::minutes(3));
        m_timer.async_wait(boost::bind(&CMySQL::pingDataBase, this, boost::asio::placeholders::error));

        return true;
    }

    void CMySQL::pingDataBase(const boost::system::error_code& error)
    {
        utils::RecLock(m_mutex);
        if(!error)
        {
            bool reinit = false;
            if(m_mysql != NULL)
            {
                unsigned long olderId = mysql_thread_id(m_mysql);
                mysql_ping(m_mysql);
                unsigned long newId = mysql_thread_id(m_mysql);
                reinit = (olderId != newId);
            }

            if(reinit)  // 发生了自动重连
            {
                mysql_close(m_mysql);
                m_mysql = NULL;
            }
            init();
        }

        m_timer.expires_from_now(boost::posix_time::minutes(3));
        m_timer.async_wait(boost::bind(&CMySQL::pingDataBase, this, _1));
    }

    bool CMySQL::getWhiteList(std::vector< std::string > & vWhiteList)
    {
        if(m_mysqlTable.empty())
        {
            return false;
        }

        std::string sql = "SELECT username FROM " + m_mysqlLoginInfo.DatabaseName + "." + m_mysqlTable + " where allowLogin=1;";

        if(mysql_query(m_mysql, sql.c_str())) 
        {
            std::cout << "Fetch white list from MySQL failed, 数据库查询错误" << std::endl;
            return false;
        }
        else
        {
            MYSQL_RES * results = mysql_store_result(m_mysql);
            if(results)
            {
                int num_fields = mysql_num_fields(results);
                MYSQL_ROW row;
                while((row = mysql_fetch_row(results)))
                {
                    vWhiteList.push_back(row[0]);
                }
            }
            return true;
        }
    }
}