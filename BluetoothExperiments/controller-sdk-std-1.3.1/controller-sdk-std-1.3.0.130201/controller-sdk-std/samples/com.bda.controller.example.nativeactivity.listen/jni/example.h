#ifndef COM_BDA_CONTROLLER_EXAMPLE_NATIVEACTIVITY_LISTEN_EXAMPLE
#define COM_BDA_CONTROLLER_EXAMPLE_NATIVEACTIVITY_LISTEN_EXAMPLE

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

extern "C"
{
	JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnKeyEvent(JNIEnv * env, jobject listener, jobject event);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnMotionEvent(JNIEnv * env, jobject listener, jobject event);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnStateEvent(JNIEnv * env, jobject listener, jobject event);
};

/******************************************************************************/

#endif