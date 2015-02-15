#ifndef HEADERS_H_
#define HEADERS_H_
#include <iostream>
#include <boost/signals2/signal.hpp>
struct HelloWorld
{
    void operator()() const
    {
        std::cout << "Hello, World!" << std::endl;
    }
};

void test_bind_as_compose()
{
    // Signal with no arguments and a void return value
    boost::signals2::signal<void()> sig;

    // Connect a HelloWorld slot
    HelloWorld hello;
    sig.connect(hello);

    // call the slots
    sig();
}
#endif