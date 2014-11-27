#ifndef COM_BDA_CONTROLLER_KEYEVENT
#define COM_BDA_CONTROLLER_KEYEVENT

/******************************************************************************/

#include <jni.h>

/******************************************************************************/

class KeyEvent
{
public:
	static const jint ACTION_DOWN = 0;
	static const jint ACTION_UP = 1;

	static const jint KEYCODE_DPAD_UP = 19;
	static const jint KEYCODE_DPAD_DOWN = 20;
	static const jint KEYCODE_DPAD_LEFT = 21;
	static const jint KEYCODE_DPAD_RIGHT = 22;
	static const jint KEYCODE_BUTTON_A = 96;
	static const jint KEYCODE_BUTTON_B = 97;
#ifdef BUILD_LEGACY_API
	// SDK 1.2.7b and earlier
	static const jint KEYCODE_BUTTON_X = 98;
	static const jint KEYCODE_BUTTON_Y = 99;
#else
	// SDK 1.2.7c and later
	static const jint KEYCODE_BUTTON_X = 99;
	static const jint KEYCODE_BUTTON_Y = 100;
#endif
	static const jint KEYCODE_BUTTON_L1 = 102;
	static const jint KEYCODE_BUTTON_R1 = 103;
	static const jint KEYCODE_BUTTON_L2 = 104;
	static const jint KEYCODE_BUTTON_R2 = 105;
	static const jint KEYCODE_BUTTON_THUMBL = 106;
	static const jint KEYCODE_BUTTON_THUMBR = 107;
	static const jint KEYCODE_BUTTON_START = 108;
	static const jint KEYCODE_BUTTON_SELECT = 109;

public:
	KeyEvent(jobject event);

	static void jniOnLoad(JNIEnv * env, jobject activity);
	static void jniOnUnload(JNIEnv * env);

	jint getAction(JNIEnv * env);
	jint getControllerId(JNIEnv * env);
	jlong getEventTime(JNIEnv * env);
	jint getKeyCode(JNIEnv * env);

private:
	const jobject mEvent;
};

/******************************************************************************/

#endif // __HEADER
