//
// Created by wanglj on 2022/8/29.
//

#include "FileLogger.h"

/**
 * 默认最小的增长大小
 */
static int DEFAULT_INCREASE_SIZE=getpagesize() * 128;

/**
 * 文件真实大小所占字节数
 */
static int DEFAULT_POSITION_SIZE=sizeof(int);


FileLogger::FileLogger(const char *logPath,int maxFileSize){

    this->logPath=(char *) malloc(strlen(logPath)+1);

    memcpy(this -> logPath,logPath, strlen(logPath)+1);

    this->maxFileSize=maxFileSize;
    //计算每次映射多大数据
    increaseSize=maxFileSize/REMMAP_TIMES;
    //最小加载0.5M
    if(mmapSize<DEFAULT_INCREASE_SIZE){
        increaseSize=DEFAULT_INCREASE_SIZE;
    }else if(increaseSize%getpagesize()!=0){
        //不是页大小的整数倍，计算到页大小的整数倍
        increaseSize=(increaseSize/getpagesize()+1)*getpagesize();
    }
    //读取到整个文件的大小
    int fileSize=getLogFileSize();

    if(fileSize<=0){
        //是一个空文件，当前映射大小
        mmapSize=increaseSize;
        //打开映射文件
        mmapFile(0,mmapSize);
    }else{
        //不是空文件，里面有数据
        mmapSize=fileSize;
        mmapFile(0,fileSize);
        //读取文件大小
        memcpy(&dataPos,mmapPtr+(mmapSize-DEFAULT_POSITION_SIZE-1),DEFAULT_POSITION_SIZE);
        if(dataPos>=maxFileSize){
            //释放之前映射过的内存
            // LOGE("释放->路径：%s，文件大小：%d,允许大小：%d, 真实大小：%d", logPath, fileSize, maxFileSize, dataPos);
            if(mmapPtr!=NULL){
                munmap(mmapPtr,mmapSize);
            }

        }

    }

}

void FileLogger::mmapFile(int start,int end){

    //首先打开文件

    long logFd=open(logPath, O_RDWR| O_CREAT,S_IRWXU);
    if(logFd==-1){
        return;
    }
    //给文件新增大小控件
    ftruncate(logFd, mmapSize);
    //用mmap映射文件区域大小
    int size=end-start;
    mmapPtr=(char *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,logFd,start);
    //关闭文件
    close(logFd);

}
void FileLogger::writeData(const char *data,int dataLen){
    //映射空间不足 扩展空间
    if(dataPos+dataLen+DEFAULT_POSITION_SIZE>mmapSize){
        if(dataPos+dataLen+DEFAULT_POSITION_SIZE>maxFileSize) {
            //找一个最大的页大小的倍数，但是要能放满当前数据
            int maxMmapSize = dataPos + dataLen + DEFAULT_POSITION_SIZE;
            if (maxMmapSize % getpagesize() != 0) {
                maxMmapSize = ((maxMmapSize / getpagesize()) + 1) * getpagesize();
            }
            increaseSize = maxMmapSize - mmapSize;
        }
        if(mmapPtr!=NULL){
            munmap(mmapPtr,mmapSize);
        }

        mmapSize+=increaseSize;
        mmapFile(0,mmapSize);
    }
    //把数据写入映射区域(文件)
    memcpy(mmapPtr + dataPos,data,dataLen);

    //当前大小位置增加
    dataPos+=dataLen;
    //把文件的真实大小写入最后四个字节(最后面四个字节永远存放真实内容大小)
    memcpy(mmapPtr+(mmapSize-DEFAULT_POSITION_SIZE-1),&dataPos,DEFAULT_POSITION_SIZE);

}
int FileLogger::getLogFileSize(){

    FILE  *fp= fopen(logPath,"r");
    if(fp==NULL){
        return 0;
    }
    fseek(fp,0L,SEEK_END);
    //得到开始位置到结束位置的偏移量
    int size= ftell(fp);
    fclose(fp);
    return size;

}

FileLogger::~FileLogger(){

    //释放内存
    if(logPath!=NULL){
        free(logPath);
    }
    //释放映射过的内存
    if(mmapPtr!=NULL){
        munmap(mmapPtr,mmapSize);
    }

}