package com.wlj.bplog;

import java.io.File;

public class FileLogger implements Comparable<FileLogger>{
    /**
     * 这对应一个 native 层的地址
     */
    private long mNativePtr;

    /**
     * 当前所对应的 File log本地文件
     */
    File mLogFile;

    private native long nativeCreate(String logPath, int maxFileSize);

    private native void nWriteData(long nativePtr,byte[] data,int dataLen);

    private native void nRelease(long nativePtr);

    private native int nRealLength(long nativePtr);

    public void release(){
        nRelease(mNativePtr);
    }

    FileLogger(File logFile, int maxFileSize) {
        this.mLogFile = logFile;
        mNativePtr = nativeCreate(logFile.getAbsolutePath(), maxFileSize);
    }
    @Override
    public int compareTo(FileLogger other) {
        return mLogFile.getName().compareToIgnoreCase(other.mLogFile.getName());

    }

    public int length(){
        return nRealLength(mNativePtr);
    }

    public void log(String content) {
        byte[] data = content.getBytes();
        nWriteData(mNativePtr, data, data.length);
    }


}
