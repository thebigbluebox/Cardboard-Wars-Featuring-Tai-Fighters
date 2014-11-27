/*
 * Bare-bones wrapper class for "com.bda.controller.StateEvent".
 * Requires error handling.
 */

/******************************************************************************/

#include "stateevent.h"

/******************************************************************************/

static jclass mClass;
static jmethodID mMethodGetAction;
static jmethodID mMethodGetControllerId;
static jmethodID mMethodGetEventTime;
static jmethodID mMethodGetState;

/******************************************************************************/

StateEvent::StateEvent(jobject event) : mEvent(event)
{
}

void StateEvent::jniOnLoad(JNIEnv * env, jobject activity)
{
	// get package-aware ClassLoader
	const jclass nativeActivityCls = env->FindClass("android/app/NativeActivity");
	const jmethodID getClassLoaderMth = env->GetMethodID(nativeActivityCls, "getClassLoader", "()Ljava/lang/ClassLoader;");
	const jobject classLoader = env->CallObjectMethod(activity, getClassLoaderMth);

	// load class
	const jstring classString = env->NewStringUTF("com/bda/controller/StateEvent");
	const jclass classLoaderCls = env->FindClass("java/lang/ClassLoader");
	const jmethodID loadClassMth = env->GetMethodID(classLoaderCls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"); 
	const jclass cls = static_cast<jclass>(env->CallObjectMethod(classLoader, loadClassMth, classString));
	env->DeleteLocalRef(classString);

	mClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));

	mMethodGetAction = env->GetMethodID(cls, "getAction", "()I");
	mMethodGetControllerId = env->GetMethodID(cls, "getControllerId", "()I");
	mMethodGetEventTime = env->GetMethodID(cls, "getEventTime", "()J");
	mMethodGetState = env->GetMethodID(cls, "getState", "()I");
}

void StateEvent::jniOnUnload(JNIEnv * env)
{
	env->DeleteWeakGlobalRef(mClass);
}

jint StateEvent::getAction(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetAction);
}

jint StateEvent::getControllerId(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetControllerId);
}

jlong StateEvent::getEventTime(JNIEnv * env)
{
	return env->CallLongMethod(mEvent, mMethodGetEventTime);
}

jint StateEvent::getState(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetState);
}
