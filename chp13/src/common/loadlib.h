/**
* 导入动态链接库
* 
* @author   xujun
* @date     2012-1-3
* @time     21:51
* @version  1.0
*/

#ifndef LOADLIB_H_
#define LOADLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32

/*
** ========================================================================
** win32
** =========================================================================
*/
#include <windows.h>

inline void tt_unloadlib (void *lib) {
    FreeLibrary((HINSTANCE)lib);
}


inline void *tt_load (const char *path) {
    return LoadLibraryA(path);
}


inline void *tt_sym (void *lib, const char *sym) {
    return GetProcAddress((HINSTANCE)lib, sym);
}

#else

/*
** ========================================================================
** This is an implementation of loadlib based on the dlfcn interface.
** The dlfcn interface is available in Linux, SunOS, Solaris, IRIX, FreeBSD,
** NetBSD, AIX 4.2, HPUX 11, and  probably most other Unix flavors, at least
** as an emulation layer on top of native functions.
** =========================================================================
*/
#include <dlfcn.h>

inline void tt_unloadlib (void *lib) {
    dlclose(lib);
}


inline void *tt_load (const char *path) {
    return dlopen(path, RTLD_LAZY);
}

inline void *tt_sym (void *lib, const char *sym) {
    return dlsym(lib, sym);
}


#endif // WIN32


inline void * tt_loads (const std::string & path)
{
    return tt_load(path.c_str());
}

inline void * tt_syms (void *lib, const std::string & sym)
{
    return tt_sym(lib, sym.c_str());
}

#ifdef __cplusplus
}
#endif

#endif //LOADLIB_H_