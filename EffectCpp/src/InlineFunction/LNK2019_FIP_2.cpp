// LNK2019_FIP_2.cpp
// compile with: LNK2019_FIP.cpp
// LNK2019 expected
// ref: http://msdn.microsoft.com/en-us/library/78t98006.aspx

// ��ȷ���÷��ǣ�inline ���εĺ���ʵ��Ҳ���� header �ļ���
#include "LNK2019_FIP.h"
int main() {
    testclass testclsObject;

    // module cannot see the implementation of PublicStatMemFunc1
    testclsObject.PublicStatMemFunc1();

    return 0;
}