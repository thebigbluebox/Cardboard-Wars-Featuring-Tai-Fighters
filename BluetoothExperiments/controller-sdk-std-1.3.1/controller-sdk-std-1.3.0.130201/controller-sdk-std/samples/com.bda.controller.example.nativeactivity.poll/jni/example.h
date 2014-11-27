#ifndef COM_BDA_CONTROLLER_EXAMPLE_NATIVEACTIVITY_POLL_EXAMPLE
#define COM_BDA_CONTROLLER_EXAMPLE_NATIVEACTIVITY_POLL_EXAMPLE

/******************************************************************************/

#include <jni.h>

/******************************************************************************/

void nativeOnLoad(JNIEnv * env, jobject activity);
void nativeOnUnload(JNIEnv * env);

void nativeOnCreate(JNIEnv * env, jobject activity);
void nativeOnDestroy(JNIEnv * env, jobject activity);
void nativeOnPause(JNIEnv * env, jobject activity);
void nativeOnResume(JNIEnv * env, jobject activity);

void nativeOnDrawFrame(JNIEnv * env);
void nativeOnSurfaceChanged(JNIEnv * env, int width, int height);
void nativeOnSurfaceCreated(JNIEnv * env);

/******************************************************************************/

#endif