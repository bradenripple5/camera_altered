#include <jni.h>
#include <android/log.h>
#include "GLVideoRendererYUV420Filter.h"

#define LOG_TAG "native"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// This is the only place you define the variable
extern GLVideoRendererYUV420Filter* g_renderer;

extern "C" JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeInitRenderer(JNIEnv* env, jobject thiz) {
    if (!g_renderer) {
        g_renderer = new GLVideoRendererYUV420Filter();
        LOGE("g_renderer created");
    } else {
        LOGE("g_renderer already exists!");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeSetThreshold(JNIEnv *env, jobject thiz,
                                                                     jfloat value) {
    if (g_renderer) {
        g_renderer->setThreshold(value);
        LOGE("Threshold set to %f", value);
    } else {
        LOGE("g_renderer is NULL, cannot set threshold");
    }
}