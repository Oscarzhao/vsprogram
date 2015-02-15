#include <iostream>
#include "Boost/Headers.h"
#include <boost/filesystem.hpp>
#include <cstring>
#include <utils/bsonFunc.h>

int main()
{
    //test_bind_as_compose();
    boost::filesystem::path path = "F:/vsprgram/cppPrimer/chp13/_runtime/Debug/Signals2.pdb";
    boost::filesystem::path path2 = "F:\\vsprgram\\cppPrimer\\chp13\\_runtime\\Debug\\Signals2.pdb";
    bool exist = boost::filesystem::exists(path2);
    std::cout << path << " exists: " << exist << std::endl;
    if(exist)
    {
        std::cout << "\"" << path << "\" shall be removed\n";
        boost::filesystem::remove(path2);

    }
    exist = boost::filesystem::exists(path2);
    std::cout << path << " exists: " << exist << std::endl;

    int a = -1;
    unsigned int b = 10;
    unsigned int c = static_cast<unsigned int>(a) ;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "b > a: " << (b>a) << std::endl;
    std::cout << "b > c: " << (b>c) << std::endl;

    bson::bob builder;
    builder.append("function", "login");
    builder.append("user", "Oscar");
    builder.append("api_version", 1.0);

    bson::bo obj = builder.obj();

    double api_version = -1;
    utils::safeParseBsonDouble(api_version, obj, "api_version");

    std::cout << "api_version == 1.0: " << (api_version == 1.0) << std::endl;
    std::cout << "api version: " << api_version << std::endl;
    system("pause");
    return 0;
}