#ifndef COM_BDA_CONTROLLER_STATEEVENT
#define COM_BDA_CONTROLLER_STATEEVENT

/******************************************************************************/

#include <jni.h>

/******************************************************************************/

class StateEvent
{
public:
	static const jint ACTION_FALSE = 0;
	static const jint ACTION_TRUE = 1;
	static const jint ACTION_DISCONNECTED = 0;
	static const jint ACTION_CONNECTED = 1;
	static const jint ACTION_CONNECTING = 2;
	static const jint ACTION_VERSION_MOGA = 0;
	static const jint ACTION_VERSION_MOGAPRO = 1;

	static const jint STATE_CONNECTION = 1;
	static const jint STATE_POWER_LOW = 2;
	static const jint STATE_SUPPORTED_VERSION = 3;	// Controller Version
	static const jint STATE_SELECTED_VERSION = 4;	// Service Controller Version

public:
	StateEvent(jobject event);

	static void jniOnLoad(JNIEnv * env);
	static void jniOnUnload(JNIEnv * env);

	jint getAction(JNIEnv * env);
	jint getControllerId(JNIEnv * env);
	jlong getEventTime(JNIEnv * env);
	jint getState(JNIEnv * env);

private:
	const jobject mEvent;
};

/******************************************************************************/

#endif // __HEADER
