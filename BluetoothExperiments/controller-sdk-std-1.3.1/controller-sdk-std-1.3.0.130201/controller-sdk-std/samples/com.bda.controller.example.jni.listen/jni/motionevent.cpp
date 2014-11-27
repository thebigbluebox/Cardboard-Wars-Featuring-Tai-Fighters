/*
 * Bare-bones wrapper class for "com.bda.controller.MotionEvent".
 * Requires error handling.
 */

/******************************************************************************/

#include "motionevent.h"

/******************************************************************************/

static jclass mClass;
static jmethodID mMethodFindPointerIndex;
static jmethodID mMethodGetAxisValue;
static jmethodID mMethodGetAxisValue2;
static jmethodID mMethodGetControllerId;
static jmethodID mMethodGetEventTime;
static jmethodID mMethodGetPointerCount;
static jmethodID mMethodGetPointerId;
static jmethodID mMethodGetRawX;
static jmethodID mMethodGetRawY;
static jmethodID mMethodGetX;
static jmethodID mMethodGetX2;
static jmethodID mMethodGetXPrecision;
static jmethodID mMethodGetY;
static jmethodID mMethodGetY2;
static jmethodID mMethodGetYPrecision;

/******************************************************************************/

MotionEvent::MotionEvent(jobject event) : mEvent(event)
{
}

void MotionEvent::jniOnLoad(JNIEnv * env)
{
	const jclass cls = env->FindClass("com/bda/controller/MotionEvent");
	mClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));

	mMethodFindPointerIndex = env->GetMethodID(cls, "findPointerIndex", "(I)I");
	mMethodGetAxisValue = env->GetMethodID(cls, "getAxisValue", "(I)F");
	mMethodGetAxisValue2 = env->GetMethodID(cls, "getAxisValue", "(II)F");
	mMethodGetControllerId = env->GetMethodID(cls, "getControllerId", "()I");
	mMethodGetEventTime = env->GetMethodID(cls, "getEventTime", "()J");
	mMethodGetPointerCount = env->GetMethodID(cls, "getPointerCount", "()I");
	mMethodGetPointerId = env->GetMethodID(cls, "getPointerId", "(I)I");
	mMethodGetRawX = env->GetMethodID(cls, "getRawX", "()F");
	mMethodGetRawY = env->GetMethodID(cls, "getRawY", "()F");
	mMethodGetX = env->GetMethodID(cls, "getX", "()F");
	mMethodGetX2 = env->GetMethodID(cls, "getX", "(I)F");
	mMethodGetXPrecision = env->GetMethodID(cls, "getXPrecision", "()F");
	mMethodGetY = env->GetMethodID(cls, "getY", "()F");
	mMethodGetY2 = env->GetMethodID(cls, "getY", "(I)F");
	mMethodGetYPrecision = env->GetMethodID(cls, "getYPrecision", "()F");
}

void MotionEvent::jniOnUnload(JNIEnv * env)
{
	env->DeleteWeakGlobalRef(mClass);
}

jint MotionEvent::findPointerIndex(JNIEnv * env, jint pointerId)
{
	return env->CallFloatMethod(mEvent, mMethodFindPointerIndex, pointerId);
}

jfloat MotionEvent::getAxisValue(JNIEnv * env, jint axis)
{
	return env->CallFloatMethod(mEvent, mMethodGetAxisValue, axis);
}

jfloat MotionEvent::getAxisValue(JNIEnv * env, jint axis, jint pointerIndex)
{
	return env->CallFloatMethod(mEvent, mMethodGetAxisValue2, axis, pointerIndex);
}

jint MotionEvent::getControllerId(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetControllerId);
}

jlong MotionEvent::getEventTime(JNIEnv * env)
{
	return env->CallLongMethod(mEvent, mMethodGetEventTime);
}

jint MotionEvent::getPointerCount(JNIEnv * env)
{
	return env->CallIntMethod(mEvent, mMethodGetPointerCount);
}

jint MotionEvent::getPointerId(JNIEnv * env, jint pointerIndex)
{
	return env->CallIntMethod(mEvent, mMethodGetPointerId, pointerIndex);
}

jfloat MotionEvent::getRawX(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetRawX);
}

jfloat MotionEvent::getRawY(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetRawY);
}

jfloat MotionEvent::getX(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetX);
}

jfloat MotionEvent::getX(JNIEnv * env, jint pointerIndex)
{
	return env->CallFloatMethod(mEvent, mMethodGetX2, pointerIndex);
}

jfloat MotionEvent::getXPrecision(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetXPrecision);
}

jfloat MotionEvent::getY(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetY);
}

jfloat MotionEvent::getY(JNIEnv * env, jint pointerIndex)
{
	return env->CallFloatMethod(mEvent, mMethodGetY2, pointerIndex);
}

jfloat MotionEvent::getYPrecision(JNIEnv * env)
{
	return env->CallFloatMethod(mEvent, mMethodGetYPrecision);
}
