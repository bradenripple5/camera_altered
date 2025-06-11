#ifndef THRESHOLDBRIDGE_H
#define THRESHOLDBRIDGE_H

#include <jni.h>  // ✅ Required for JNI types

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeSetThreshold(JNIEnv *env, jobject obj, jfloat threshold);

JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeInitRenderer(JNIEnv *env, jobject obj);

#ifdef __cplusplus
}
#endif

#endif // THRESHOLDBRIDGE_H
