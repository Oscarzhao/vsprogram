#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

typedef boost::function<int (int, int)> CallBackFunc;

class Calculation
{
public:
    int add(int a, int b)
    {
        return a+b;
    }
};

int main()
{
    Calculation cal;
    CallBackFunc func = boost::BOOST_BIND(&Calculation::add, &cal, _1, _2);
    std::cout << func(1, 2) << std::endl;
    system("pause");
    return 0;
}