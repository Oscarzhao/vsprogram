#include "common/Stdafx.h"
#include "utils/FileWatcher.h"
#include "utils/sha1.h"
#include <sys/stat.h>

namespace utils
{
    FileWatcher::FileWatcher(const string& filePath)
        :m_strFilePath(filePath)
    {
        if (!filePath.empty())
        {
            init();
        }
    }

    FileWatcher::~FileWatcher()
    {

    }

    void FileWatcher::init()
    {
        int ret = ::stat(m_strFilePath.c_str(), &m_fileStat);
        if (ret != 0)
        {
            TT_STDLOG(LLV_ERROR, "read get task key file error: %s", m_strFilePath.c_str());
        }
        sha1_checksum(m_fileDigest, m_strFilePath.c_str(), m_strFilePath.size());
    }

    bool FileWatcher::testIsFileChanged()
    {
        bool ret = false;
        int nRet = ::stat(m_strFilePath.c_str(), &m_newFileStat);
        if (nRet != 0)
        {
            TT_STDLOG(LLV_ERROR, "read get task key file error: %s", m_strFilePath.c_str());            
        } else {
            if (memcmp(&m_fileStat, &m_newFileStat, sizeof(struct stat)) == 0)
            {
                ret = false;              
            } else {
                sha1_checksum(m_newfileDigest, m_strFilePath.c_str(), m_strFilePath.size());
                if (memcmp(m_fileDigest, m_newfileDigest, sizeof(m_fileDigest)) == 0)
                {
                    ret = false;
                } else {
                    ret = true;
                }
                m_fileStat = m_newFileStat;
                memcpy(&m_fileStat, &m_newFileStat, sizeof(m_newFileStat));
            }
        }
        return ret;
    }


    void FileWatcher::setFilepath(const string& filePath)
    {
        m_strFilePath = filePath;
        init();
    }
}