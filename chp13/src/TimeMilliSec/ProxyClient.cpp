#include <TimeMilliSec/ProxyClient.h>

ProxyClient::ProxyClient()
:m_strMsg("NONE")
,m_nDurationMilliSec(-1)
,m_timer(m_ioservice)
,m_work(m_ioservice)
{

}

ProxyClient::ProxyClient(std::string msg, int duration_sec)
:m_strMsg(msg)
,m_nDurationMilliSec(duration_sec)
,m_timer(m_ioservice)
,m_work(m_ioservice)
{

}

ProxyClient::~ProxyClient()
{

}

void ProxyClient::start()
{
    boost::system::error_code error;
    //  This function is used to ask the io_service to execute the given handler, but without allowing the io_service to call the handler from inside this function.
    //The io_service guarantees that the handler will only be called in a thread in which the run(), run_one(), poll() or poll_one() member functions is currently being invoked. 
    // 这里两种方法都可以
    if(m_nDurationMilliSec>0)
    {
        m_ioservice.post(boost::BOOST_BIND(&ProxyClient::timerExpirePrintMsg, this, error));
    }
    else
    {
        std::cout << "time duration is not valid" << std::endl;
    }
    //timerExpirePrintMsg(error);
    m_ioservice.run();
}

void ProxyClient::timerExpirePrintMsg(const boost::system::error_code& error)
{
    if(!error)
    {
        std::cout << "Msg: " << m_strMsg << std::endl;
        m_timer.expires_from_now(boost::posix_time::milliseconds(m_nDurationMilliSec));
        m_timer.async_wait(boost::BOOST_BIND(&ProxyClient::timerExpirePrintMsg, this, _1));
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
}