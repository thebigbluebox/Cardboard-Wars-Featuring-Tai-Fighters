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

void StateEvent::jniOnLoad(JNIEnv * env)
{
	const jclass cls = env->FindClass("com/bda/controller/StateEvent");
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
