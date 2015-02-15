#include <iostream>
/**
 *	功能：成员函数指针的使用
 *  ref1: http://msdn.microsoft.com/en-us/library/b0x1aatf(v=vs.90).aspx
 *  ref2: http://www.cnblogs.com/satng/archive/2011/03/14/2138809.html
 */
class X
{
public:
    void mf(){ std::cout << "Class X::mf\n"; }
};

typedef void (X::*FunctionPtr)();
int main()
{
    // FunctionPtr pmf = X::mf; // error C3867
    FunctionPtr pmf = &X::mf;
    X x;
    (x.*pmf)();
    system("pause");
    return 0;
}