/*
 * Bare-bones wrapper class for "com.bda.controller.KeyEvent".
 * Requires error handling.
 */

/******************************************************************************/

#include "keyevent.h"

/******************************************************************************/

static jclass mClass;
static jmethodID mMethodGetAction;
static jmethodID mMethodGetControllerId;
static jmethodID mMethodGetEventTime;
static jmethodID mMethodGetKeyCode;

/******************************************************************************/

KeyEvent::KeyEvent(jobject event) : mEvent(event)
{
}

void KeyEvent::jniOnLoad(JNIEnv * env)
{
	const jclass cls = env->FindClass("com/bda/controller/KeyEvent");
	mClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));

	mMethodGetAction = env->GetMethodID(cls, "getAction", "()I");
	mMethodGetControllerId = env->GetMethodID(cls, "getControllerId", "()I");
	mMethodGetEventTime = env->GetMethodID(cls, "getEventTime", "()J");
	mMethodGetKeyCode = env->GetMethodID(cls, "getKeyCode", "()I");
}

void KeyEvent::jniOnUnload(JNIEnv * env)
{
	env->DeleteWeakGlobalRef(mClass);
}

jint KeyEvent::getAction(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetAction);
}

jint KeyEvent::getControllerId(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetControllerId);
}

jlong KeyEvent::getEventTime(JNIEnv * env)
{
	return env->CallLongMethod(mEvent, mMethodGetEventTime);
}

jint KeyEvent::getKeyCode(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetKeyCode);
}
