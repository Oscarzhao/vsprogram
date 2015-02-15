/// exported in test.dll / libtest.so / libtest.dylib
namespace test
{
    class MyClass {
    public:
        MyClass();
        ~MyClass();
        virtual void virtualMethod(int i, float f);
        void normalMethod(int i);
    };
    void getSomeCount(int* countOut);
    void test() {
        int count;
        getSomeCount(&count);
        MyClass t;
        t.virtualMethod(count, 0.5f);
    }
}