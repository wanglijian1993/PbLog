//
// Created by wanglj on 2022/8/29.
//

#ifndef BPLOG_FILELOGGER_H
#define BPLOG_FILELOGGER_H

#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "DZConstDefine.h"
#include <sys/mman.h>
#include <jni.h>

class FileLogger {


public:
    FileLogger(const char *logPath,int maxFileSize);
    ~FileLogger();

public:

    /**
     * 当前log日志文件
     */
    char *logPath;

    /**
     * 每次加载的大小，必须要是页的大小的整数倍
     */
    int increaseSize;

    /**
     * 当前写入数据的位置
     */
    int dataPos=0;

    /**
     * 当前的文件映射大小
     */
    int mmapSize=0;


    /**
     * mmap映射的文件
     */
    char *mmapPtr=NULL;

    /**
     * 文件允许的最大范围
     */
    int maxFileSize;


public:
    void mmapFile(int start,int fileLen);
    void writeData(const char *data,int dateLen);
    int getLogFileSize();
};


#endif //BPLOG_FILELOGGER_H
