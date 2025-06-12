#include <jni.h>
#include "GLVideoRendererYUV420Filter.h"
#include "Log.h"

GLVideoRendererYUV420Filter* g_renderer = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeSetFilterIntensity(JNIEnv* env, jobject thiz, jfloat value) {
    LOGE("calling jni Java_com_media_camera_preview_activity_GLActivity_nativeSetFilterIntensity ");

    if (g_renderer) {
        LOGE("g render is real yall and the value we're trying to pass is %f",value);
        g_renderer->setEffectIntensity(value);
    }
    else{
        LOGE("g render is NOT real yall");

    }
}

// Define the global renderer instance

extern "C"
JNIEXPORT void JNICALL
Java_com_media_camera_preview_activity_GLActivity_nativeInitRenderer(JNIEnv* env, jobject thiz) {
    if (!g_renderer) {
        g_renderer = static_cast<GLVideoRendererYUV420Filter*>(
                VideoRenderer::create(tYUV420_FILTER).release()
        );
        LOGE("g_renderer created");
    } else {
        LOGE("g_renderer already exists");
    }
}