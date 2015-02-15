#include "common/Stdafx.h"
#include "utils/AsioHelper.h"
#include <boost/asio.hpp>

namespace utils
{
    static void on_timer(boost::shared_ptr<boost::asio::deadline_timer> timer, const boost::function<void (const boost::system::error_code&)>& callback, const boost::system::error_code& error)
    {
        callback(error);
    }

    void async_wait_seconds(StrandPtr strand, int seconds, const boost::function<void (const boost::system::error_code&)>& callback)
    {
        boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(strand->get_io_service()));
        timer->expires_from_now(boost::posix_time::seconds(seconds));
        timer->async_wait(strand->wrap(boost::bind(&on_timer, timer, callback, _1)));
    }

    void async_wait_milSeconds(StrandPtr strand, int milSeconds, const boost::function<void (const boost::system::error_code&)>& callback)
    {
        AsioTimerPtr timer(new boost::asio::deadline_timer(strand->get_io_service()));
        timer->expires_from_now(boost::posix_time::millisec(milSeconds));
        timer->async_wait(strand->wrap(boost::bind(&on_timer, timer, callback, _1)));
    }

    void async_wait_seconds(AsioTimerPtr timer, int seconds, const boost::function<void (const boost::system::error_code&)>& callback)
    {
        timer->expires_from_now(boost::posix_time::seconds(seconds));
        timer->async_wait(boost::bind(&on_timer, timer, callback, _1));
    }

    void async_wait_milSeconds(AsioTimerPtr timer, int milSeconds, const boost::function<void (const boost::system::error_code&)>& callback)
    {
        timer->expires_from_now(boost::posix_time::millisec(milSeconds));
        timer->async_wait(boost::bind(&on_timer, timer, callback, _1));
    }

    void cancelTimer(AsioTimerPtr timer)
    {
        if (NULL != timer)
        {
            boost::system::error_code error;
            timer->cancel(error);
        }
    }
}