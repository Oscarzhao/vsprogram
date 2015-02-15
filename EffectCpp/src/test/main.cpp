#include <iostream>
#include <boost/signals2.hpp>
/**
 *	信号/插槽机制
 */
struct HelloWorld
{
    void operator()() const
    {
        std::cout << "Hello, World!" << std::endl;
    }
};

int main()
{
    // Signal with no arguments and a void return value
    boost::signals2::signal<void()> sig;

    // Connect a HelloWorld slot
    HelloWorld hello;
    sig.connect(hello);

    // call the slots
    sig();

    system("pause");
    return 0;
}