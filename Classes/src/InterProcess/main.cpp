#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/date_time.hpp>
#include <InterProcess/persist.h>

void testDataTime()
{
    using namespace boost::gregorian;
    date weekstart(2014, Jun, 1);
    date weekend = weekstart + weeks(1);
    date today = day_clock::local_day();
    if(weekstart >= today)
    {
        std::cout << "the day " << "weekstart is later than today" << std::endl;
    }

    day_iterator iter(weekstart);
    while(iter <= weekend)
    {
        std::cout << (*iter) << std::endl;
        ++iter;
    }

    // input stream
    date d3;
    std::stringstream ss("2014-Dec-11");
    ss >> d3;

    std::cout << d3 << std::endl;
}
//template<typename T>
void myfunc(std::string str)
{
    std::cout <<" " << str;
}
void testString()
{
    std::vector< std::string > vecStr;
    vecStr.push_back("C");
    vecStr.push_back("C++");
    vecStr.push_back("Java");
    vecStr.push_back("C#");
    vecStr.push_back("Python");
    for_each(vecStr.begin(), vecStr.end(), myfunc);
    std::cout << std::endl;

}
void testPersist()
{
    persist::CPersistService pers;
    bool success = false;
    std::string retStr;

    pers.addPair("C++", success);
    std::cout << "Add pair \"C++\" succeeded: " << success << std::endl;
    pers.testPair("C++", retStr);
    std::cout << retStr << std::endl;
}
int main()
{
    // testDataTime();
    //testString();
    
    testPersist();
    system("pause");
    return 0;
}