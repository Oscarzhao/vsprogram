/********************************************************************
    company:    北京睿智融科控股有限公司
    fileName:    FileWatcher.h
    author:        xujun    
    created:    2013/22/11   14:52    
    purpose:    文件监控
*********************************************************************/
#ifndef FileWatcher_2013_11_22_H
#define FileWatcher_2013_11_22_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace utils
{
    class FileWatcher
    {
    public:
        FileWatcher(const string& filePath);
        ~FileWatcher();

        bool testIsFileChanged();
        void setFilepath(const string& filePath);

    protected:
        void init();

    private:
        string m_strFilePath;
        struct stat m_fileStat;
        unsigned char m_fileDigest[20];
        struct stat m_newFileStat;
        unsigned char m_newfileDigest[20];
    };
}


#endif