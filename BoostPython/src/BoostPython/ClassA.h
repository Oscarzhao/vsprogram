#ifndef CLASS_A_H_
#define CLASS_A_H_

#include <iostream>
#include <string>

struct Person
{
public:
    Person()
        :m_strName("Anonymous")
        ,m_nAge(12)
    {
    };

    Person(std::string name, int age)
        :m_strName(name)
        ,m_nAge(age)
    {
    };
    std::string    m_strName;
    int            m_nAge;
};

class A 
{
private:
    int m_nA;
    struct Person m_person;
public:
    //int getM_nA()
    //{
    //    std::cout << "non const A\n";
    //    return m_nA;
    //}
    const int & getM_nA() const
    {
        std::cout << "const A, m_nA = " << m_nA << "\n";
        return m_nA;
    }
    void setM_nA(int val)
    {
        m_nA = val;
    }

    struct Person& getM_Person()
    {
        std::cout << "non const Person\n";
        return m_person;
    }

    const struct Person& getM_Person() const
    {
        std::cout << "const person\n";
        return m_person;
    }
};
#endif