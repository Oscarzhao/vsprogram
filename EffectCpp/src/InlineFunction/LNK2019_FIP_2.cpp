// LNK2019_FIP_2.cpp
// compile with: LNK2019_FIP.cpp
// LNK2019 expected
// ref: http://msdn.microsoft.com/en-us/library/78t98006.aspx

// 正确的用法是：inline 修饰的函数实现也放在 header 文件中
#include "LNK2019_FIP.h"
int main() {
    testclass testclsObject;

    // module cannot see the implementation of PublicStatMemFunc1
    testclsObject.PublicStatMemFunc1();

    return 0;
}