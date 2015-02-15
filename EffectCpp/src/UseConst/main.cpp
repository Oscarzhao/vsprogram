#include <iostream>
/**
 *	const 的使用
 */

class A
{
private:
    int m_n;
public:
    A():m_n(0){}
    int get() const
    {
        return m_n;
    }
    int get2()
    {
        return m_n;
    }
};

int main()
{

    const char * sz = "aaa";
    // const 在前修饰，不可以改变 sz 指向的内存地址存储的值，但可以将其指向另外一块地址
    //sz[0] = 'b';      // invalid
    sz = "bbb";         // valid

    char * const tz = "aaa";
    // const 在中间修饰，可以改变 tz 指向的内存地址存储的值，但不可以将其指向另外一块内存地址
    tz[0] = 'b';        // valid
    //tz = "bbb";         // invalid

    const char * const uz = "aaa";
    // 不可以更改  uz
    //uz[0] = 'b';      // invalid
    //uz = "bbb";       // invalid

    const A a;
    // 1. const 对象 不能调用 非 const 成员函数
    // int result = a.get2();   // invalid
    a.get();                    // valid


    system("pause");
    return 0;
}