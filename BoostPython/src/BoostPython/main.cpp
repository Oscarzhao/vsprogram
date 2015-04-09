#include <iostream>
#include <BoostPython/ClassA.h>
#include <vector>

void testConst(const struct A & a)
{
    std::cout << "Test const start\n";
    a.getM_Person();
    a.getM_nA();
    std::cout << "Test const end\n";
}
int main(int argc, char* argv[])
{
    const A a;
    int ele = a.getM_nA();
    ele = 5;
    std::cout << "ele = " << ele << std::endl;
    a.getM_nA();

    A b;
    b.setM_nA(9);
    b.getM_Person().m_nAge = 5;
    testConst(b);

    const struct Person& p = a.getM_Person();
    std::cout << "Name: " << p.m_strName << ", age: " << p.m_nAge << std::endl;
    std::vector<int> vec;
    for(int i = 0;i < 10; i++) vec.push_back(2*i+1);
    std::vector<int> vec2(vec.begin(), vec.end());

    std::cout << "erase from start to end:\n";
    for(std::vector<int>::iterator iA = vec.begin(), iB = vec.end(); iA != vec.end();  ) 
    {
        std::vector<int>::iterator itmp = vec.end();
        std::cout << *iA << ", ";
        iA = vec.erase(iA);
    }
    std::cout << std::endl;
    std::cout << "erase from end to start:\n";
    //for(std::vector<int>::iterator iA = vec2.begin(), iB = vec2.end(); iA != iB; iB-- ) 
    //{
    //    std::cout << *iB << ", ";
    //    vec2.erase(iB);
    //}
    std::cout << std::endl;
    system("pause");
    return 0;
}