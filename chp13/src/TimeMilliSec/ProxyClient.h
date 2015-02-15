/**
 *	定时打印一个字符串，时间间隔为 -1 millisec
 *  验证 -1 millisec 有多长
 *  @author: zhaoshuailong
 *  @date:   2014-11-06
 */

#ifndef PROXY_CLIENT_H_
#define PROXY_CLIENT_H_
#include <iostream>
#include <boost/date_time.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
class ProxyClient
{
public:
    ProxyClient();
    ProxyClient(std::string msg, int duration_sec);
    ~ProxyClient();
    void start();
private:
    void timerExpirePrintMsg(const boost::system::error_code& error);
private:
    boost::asio::io_service             m_ioservice;
    boost::asio::io_service::work       m_work;
    boost::asio::deadline_timer         m_timer;

    std::string                         m_strMsg;
    int                                 m_nDurationMilliSec;
};
#endif
