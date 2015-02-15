#include <iostream>
#include <boost/date_time.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <cstring>
#include <TimeMilliSec/ProxyClient.h>
int main()
{
    boost::posix_time::ptime tick = boost::posix_time::microsec_clock::local_time();
    boost::this_thread::sleep(boost::posix_time::millisec(500));
    boost::posix_time::ptime timeNow = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration diff = timeNow - tick;
    std::cout << diff.total_milliseconds() << " millisecond" << std::endl;

    std::cout << "Date now: " << boost::gregorian::day_clock::local_day() << std::endl;
    std::cout << "Time now: " << boost::posix_time::second_clock::local_time();

    
    std::cout << "a = " << boost::posix_time::milliseconds(-1) << std::endl;

    std::cout << "do nothing" << std::endl;

    ProxyClient client("do not stop", -400);
    client.start();
    system("pause");
    return 0;
}