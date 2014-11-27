#ifndef COM_BDA_CONTROLLER_EXAMPLE_JNI_POLL_EXAMPLE
#define COM_BDA_CONTROLLER_EXAMPLE_JNI_POLL_EXAMPLE

/******************************************************************************/

#include <jni.h>

/******************************************************************************/

extern "C"
{
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * jvm, void * reserved);
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * jvm, void * reserved);

	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_nativeOnCreate(JNIEnv * env, jobject activity);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_nativeOnDestroy(JNIEnv * env, jobject activity);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_nativeOnPause(JNIEnv * env, jobject activity);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_nativeOnResume(JNIEnv * env, jobject activity);

	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_00024ExampleRenderer_nativeOnDrawFrame(JNIEnv * env, jobject renderer, jobject gl);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_00024ExampleRenderer_nativeOnSurfaceChanged(JNIEnv * env, jobject renderer, jobject gl, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_bda_controller_example_jni_poll_ExampleActivity_00024ExampleRenderer_nativeOnSurfaceCreated(JNIEnv * env, jobject renderer, jobject gl, jobject config);
};

/******************************************************************************/

#endif