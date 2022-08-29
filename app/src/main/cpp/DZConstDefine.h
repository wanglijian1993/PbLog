//
// Created by 曾辉 on 2019-08-12.
//

#ifndef NDK_DAY05_DZCONSTDEFINE_H
#define NDK_DAY05_DZCONSTDEFINE_H

#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

/**
 * 默认分四次加载，如果是 4M ，每次加载 1M
 */
#define REMMAP_TIMES 4;

#endif //NDK_DAY05_DZCONSTDEFINE_H
