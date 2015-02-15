#include <iostream>
#include <test/MyClass.h>

namespace test
{
    MyClass::MyClass(){}
    MyClass::~MyClass(){}
    void MyClass::virtualMethod(int i, float f)
    {
        std::cout << "[virtualMethod] i = " << i << ", f = " << f << std::endl;
    }

    void MyClass::normalMethod(int i)
    {
        std::cout << "[normalMethod] i = " << i << std::endl;
    }

    void getSomeCount(int* countOut)
    {
        std::cout << "[getSomeCount]" << std::endl;
    }
}