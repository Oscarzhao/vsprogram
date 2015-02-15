/// exported in test.dll / libtest.so / libtest.dylib

#ifdef LIB_XT_API_EXPORT
#define XT_API_EXPORT __declspec(dllexport)
#else
#define XT_API_EXPORT __declspec(dllimport)
#endif