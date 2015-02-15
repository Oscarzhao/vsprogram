/********************************************************************
    created:    2013/11/01
    created:    1:11:2013   17:53
    filename:     f:\server5\ttservice\branches\shoushan_20130828\libs\base\src\utils\RLimit.h
    file path:    f:\server5\ttservice\branches\shoushan_20130828\libs\base\src\utils
    file base:    RLimit
    file ext:    h
    author:        xujun
    
    purpose:    修改打开的最大文件句柄
*********************************************************************/
#ifndef RLimit_2013_11_1_H
#define RLimit_2013_11_1_H

namespace utils
{
    bool setMaxOpenFile(int maxlimit);
}

#endif