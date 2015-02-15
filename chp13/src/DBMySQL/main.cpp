#include <iostream>
#include <DBMySQL/DBMySQL.h>
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_service io_service;
    ttservice::MySQLLoginInfo info("127.0.0.1", 3306, "root", "123456", "test");
    ttservice::CMySQL instance(info, "white_list", io_service);

    instance.init();

    std::vector< std::string > vList;
    instance.getWhiteList(vList);

    std::vector< std::string >::const_iterator iter = vList.begin();
    while(iter != vList.end())
    {
        std::cout << *iter;
        iter++;
    }
    system("pause");
    return 0;
}