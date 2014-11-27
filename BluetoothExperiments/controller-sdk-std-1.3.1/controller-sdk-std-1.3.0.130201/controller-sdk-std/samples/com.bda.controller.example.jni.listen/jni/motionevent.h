#ifndef COM_BDA_CONTROLLER_MOTIONEVENT
#define COM_BDA_CONTROLLER_MOTIONEVENT

/******************************************************************************/

#include <jni.h>

/******************************************************************************/

class MotionEvent
{
public:
	static const jint AXIS_X = 0;
	static const jint AXIS_Y = 1;
	static const jint AXIS_Z = 11;
	static const jint AXIS_RZ = 14;
	static const jint AXIS_LTRIGGER = 17;
	static const jint AXIS_RTRIGGER = 18;

public:
	MotionEvent(jobject event);

	static void jniOnLoad(JNIEnv * env);
	static void jniOnUnload(JNIEnv * env);

	jint findPointerIndex(JNIEnv * env, jint pointerId);
	jfloat getAxisValue(JNIEnv * env, jint axis);
	jfloat getAxisValue(JNIEnv * env, jint axis, jint pointerIndex);
	jint getControllerId(JNIEnv * env);
	jlong getEventTime(JNIEnv * env);
	jint getPointerCount(JNIEnv * env);
	jint getPointerId(JNIEnv * env, jint pointerIndex);
	jfloat getRawX(JNIEnv * env);
	jfloat getRawY(JNIEnv * env);
	jfloat getX(JNIEnv * env);
	jfloat getX(JNIEnv * env, jint pointerIndex);
	jfloat getXPrecision(JNIEnv * env);
	jfloat getY(JNIEnv * env);
	jfloat getY(JNIEnv * env, jint pointerIndex);
	jfloat getYPrecision(JNIEnv * env);

private:
	const jobject mEvent;
};

/******************************************************************************/

#endif // __HEADER
