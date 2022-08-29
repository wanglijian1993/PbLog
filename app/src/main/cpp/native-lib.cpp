#include <jni.h>
#include <string>
#include <android/log.h>
#include "FileLogger.h"
extern "C"
JNIEXPORT jlong JNICALL
Java_com_wlj_bplog_FileLogger_nativeCreate(JNIEnv *env, jobject thiz, jstring logPath_,
                                           jint max_file_size) {

    const char *logPath= env->GetStringUTFChars(logPath_,0);

    FileLogger *fileLogger=new FileLogger(logPath,max_file_size);

    env->ReleaseStringUTFChars(logPath_,logPath);

    return reinterpret_cast<jlong>(fileLogger);


}
extern "C"
JNIEXPORT void JNICALL
Java_com_wlj_bplog_FileLogger_nWriteData(JNIEnv *env, jobject thiz, jlong nativePtr,
                                         jbyteArray data_, jint dataLen) {

    jbyte *data= env->GetByteArrayElements(data_,NULL);
    FileLogger *fileLogger=reinterpret_cast<FileLogger*>(nativePtr);
    if(fileLogger!=NULL) {
        fileLogger->writeData((char *) data, dataLen);
    }
    env ->ReleaseByteArrayElements(data_,data,0);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_wlj_bplog_FileLogger_nRealLength(JNIEnv *env, jobject thiz, jlong nativePtr) {

    FileLogger *fileLogger=reinterpret_cast<FileLogger*>(nativePtr);

    if(fileLogger!=NULL){
       return fileLogger -> dataPos;
    }
    return 0;

}


extern "C"
JNIEXPORT void JNICALL
Java_com_wlj_bplog_FileLogger_nRelease(JNIEnv *env, jobject thiz, jlong nativePtr) {

    FileLogger *fileLogger=reinterpret_cast<FileLogger *>(nativePtr);
    if(fileLogger!=NULL){
        delete(fileLogger);
    }
}