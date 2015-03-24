#include <iostream>
/**
 *	功能：成员函数指针的使用
 *  ref1: http://msdn.microsoft.com/en-us/library/b0x1aatf(v=vs.90).aspx
 *  ref2: http://www.cnblogs.com/satng/archive/2011/03/14/2138809.html
 */
class X
{
public:
	int m_nInt;
	//double m_dDouble;
	
	virtual ~X() {std::cout << "X destructor\n"; };
	virtual void vmf() {std::cout << "virtual function of X \n"; }
    void mf(){ std::cout << "Class X::mf\n"; }
	static void smf() {std::cout << "Class X::smf\n"; };

};

class Y: public X
{
public:
	int m_nInt;

	~Y(){ std::cout << "Y destructor\n"; }
	void vmf() {std::cout << "virtual function of Y:\n"; }
	static void ysmf(){std::cout << "Y: static function\n"; }
	static void ysmf2() {std::cout << "Y::ysmf2 static function\n"; }
};

typedef void (X::*FunctionPtr)();

void test()
{
	X x;
	Y y;
	std::cout << "sizeof(x) = " << sizeof(x) << ", sizeof(y) = " << sizeof(y) << std::endl;
	std::cout << "sizeof(X) = " << sizeof(X) << ", sizeof(Y) = " << sizeof(Y) << std::endl;
};

int main()
{
    // FunctionPtr pmf = X::mf; // error C3867
    FunctionPtr pmf = &X::mf;
	X x;
    (x.*pmf)();

	test();
    system("pause");
    return 0;
}