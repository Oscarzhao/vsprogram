/**
 *  操作系统相关系统调用
 *
 *  @uathor zhangyi
 *  @since  2012-06-05
 */

#ifndef SERVER3_BASE_UTILS_SYSUTILS_H
#define SERVER3_BASE_UTILS_SYSUTILS_H

#include <string>
#include <list>
using namespace std;

namespace utils
{
    static const unsigned int INVALID_PID = -1;

    int getPid();

    void filter_enter(char *str);

    string getAppFullPath();
    string getAppDir();
    string getAppDataDir();
    string getAppName();

    unsigned int startProcess( const string & appFullPath, const string & args );
    unsigned int startProcess( const string & appFullPath, const list<string> & args );

    unsigned int startProcessWithOutQuotation( const string & appFullPath, const string & args );

    void killProcesses(string strProcessName);
    void killProcessesFull(string strProcessFullPath);

    bool isProcessAlive(unsigned int nPid, string strProcessName);
    bool isProcessAlive(string strProcessName);
    bool isProcessAliveFull(string strProcessFullPath, string & strMsg);

    string getCpuId();
    string getDiskId();
    string getBiosId();
}

#endif
