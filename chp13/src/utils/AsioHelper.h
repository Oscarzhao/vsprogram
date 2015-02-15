/**
 *  
 *
 *  @author xujun
 *  @since  2013-1-5
 */
#ifndef _AsioHelper_2013_1_5_h__
#define _AsioHelper_2013_1_5_h__

#include <boost/asio/strand.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace utils
{
    typedef boost::shared_ptr<boost::asio::strand> StrandPtr;
    typedef boost::shared_ptr<boost::asio::deadline_timer> AsioTimerPtr;

    void async_wait_seconds(StrandPtr strand, int seconds, const boost::function<void (const boost::system::error_code&)>& callback);
    void async_wait_milSeconds(StrandPtr strand, int milSeconds, const boost::function<void (const boost::system::error_code&)>& callback);

    void async_wait_seconds(AsioTimerPtr timer, int seconds, const boost::function<void (const boost::system::error_code&)>& callback);
    void async_wait_milSeconds(AsioTimerPtr timer, int milSeconds, const boost::function<void (const boost::system::error_code&)>& callback);

    void cancelTimer(AsioTimerPtr timer);
}

#endif // AsioHelper_h__