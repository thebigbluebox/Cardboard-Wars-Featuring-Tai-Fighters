#include <GLES\gl.h>
#include "controller.h"
#include "keyevent.h"
#include "motionevent.h"
#include "stateevent.h"
#include "example.h"

/******************************************************************************/

class ExamplePlayer
{
public:
	static const float DEFAULT_SCALE = 4.0f;
	static const float DEFAULT_X = 0.0f;
	static const float DEFAULT_Y = 0.0f;

	int mConnection;
	int mControllerVersion;
	int mButtonA;
	int mButtonB;
	int mButtonStart;
	float mAxisX;
	float mAxisY;
	float mAxisZ;
	float mAxisRZ;
	const float mR;
	const float mG;
	const  float mB;
	float mScale;
	float mX;
	float mY;

public:
	ExamplePlayer(float r, float g, float b) : mR(r), mG(g), mB(b)
	{
		mConnection = StateEvent::ACTION_DISCONNECTED;
		mControllerVersion = StateEvent::STATE_SELECTED_VERSION;
		mButtonA = KeyEvent::ACTION_UP;
		mButtonB = KeyEvent::ACTION_UP;
		mButtonStart = KeyEvent::ACTION_UP;
		mAxisX = 0.0f;
		mAxisY = 0.0f;
		mAxisZ = 0.0f;
		mAxisRZ = 0.0f;
		mScale = DEFAULT_SCALE;
		mX = DEFAULT_X;
		mY = DEFAULT_Y;
	}
};

/******************************************************************************/

static jobject mController;

static ExamplePlayer mPlayer(0.0f, 1.0f, 0.0f);

static const float mVertices[] =
{
	+5.0f,  0.0f, 0.0f,
	 0.0f, +5.0f, 0.0f,
	-5.0f,  0.0f, 0.0f,
	 0.0f, -5.0f, 0.0f,
};

/******************************************************************************/

void nativeOnLoad(JNIEnv * env, jobject activity)
{
	Controller::jniOnLoad(env, activity);
	KeyEvent::jniOnLoad(env, activity);
	MotionEvent::jniOnLoad(env, activity);
	StateEvent::jniOnLoad(env, activity);
}

void nativeOnUnload(JNIEnv * env)
{
	Controller::jniOnUnload(env);
	KeyEvent::jniOnUnload(env);
	MotionEvent::jniOnUnload(env);
	StateEvent::jniOnUnload(env);
}

/******************************************************************************/

void nativeOnCreate(JNIEnv * env, jobject activity)
{
	/*
	 * Initialize Controller
	 */
	const jobject controller = Controller::getInstance(env, activity);
	mController = env->NewGlobalRef(controller);

	Controller _controller(mController);
	_controller.init(env);

	/*
	 * Set ControllerListener
	 * Demonstrating how to create a ControllerListener object in native code.
	 * It is perfectly legal to create a ControllerListener object in java and pass it to the JNI instead.
	 */

	// get package-aware ClassLoader
	const jclass nativeActivityCls = env->FindClass("android/app/NativeActivity");
	const jmethodID getClassLoaderMth = env->GetMethodID(nativeActivityCls, "getClassLoader", "()Ljava/lang/ClassLoader;");
	const jobject classLoader = env->CallObjectMethod(activity, getClassLoaderMth);

	// load ExampleControllerListener class
	const jstring classString = env->NewStringUTF("com/bda/controller/example/nativeactivity/listen/ExampleActivity$ExampleControllerListener");
	const jclass classLoaderCls = env->FindClass("java/lang/ClassLoader");
	const jmethodID loadClassMth = env->GetMethodID(classLoaderCls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"); 
	const jclass cls = static_cast<jclass>(env->CallObjectMethod(classLoader, loadClassMth, classString));
	env->DeleteLocalRef(classString);

	// fixup ExampleControllerListener native methods
	static const JNINativeMethod nativeMethods [3] =
	{
		{"nativeOnKeyEvent",	"(Lcom/bda/controller/KeyEvent;)V",		reinterpret_cast<void *>(&Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnKeyEvent)},
		{"nativeOnMotionEvent",	"(Lcom/bda/controller/MotionEvent;)V",	reinterpret_cast<void *>(&Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnMotionEvent)},
		{"nativeOnStateEvent",	"(Lcom/bda/controller/StateEvent;)V",	reinterpret_cast<void *>(&Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnStateEvent)},
	};
	env->RegisterNatives(cls, nativeMethods, sizeof(nativeMethods) / sizeof(*nativeMethods));

	// create ExampleControllerListener object
	const jmethodID method = env->GetMethodID(cls, "<init>", "()V");
	const jobject listener = env->NewObject(cls, method, activity);

	// set ExampleControllerListener object
	_controller.setListener(env, listener, NULL);
	env->DeleteLocalRef(listener);
}

void nativeOnDestroy(JNIEnv * env, jobject activity)
{
	/*
	 * Shutdown Controller
	 */
	Controller _controller(mController);
	_controller.exit(env);

	env->DeleteGlobalRef(mController);
	mController = NULL;
}

void nativeOnPause(JNIEnv * env, jobject activity)
{
	/*
	 * Notify Controller that activity has paused
	 */
	Controller _controller(mController);
	_controller.onPause(env);
}

void nativeOnResume(JNIEnv * env, jobject activity)
{
	/*
	 * Notify Controller that activity has resumed
	 */
	Controller _controller(mController);
	_controller.onResume(env);

	mPlayer.mConnection = _controller.getState(env, Controller::STATE_CONNECTION);
	mPlayer.mControllerVersion = _controller.getState(env, Controller::STATE_SELECTED_VERSION);
	mPlayer.mButtonA = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_A);
	mPlayer.mButtonB = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_B);
	mPlayer.mButtonStart = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_START);
	mPlayer.mAxisX = _controller.getAxisValue(env, Controller::AXIS_X);
	mPlayer.mAxisY = _controller.getAxisValue(env, Controller::AXIS_Y);
	mPlayer.mAxisZ = _controller.getAxisValue(env, Controller::AXIS_Z);
	mPlayer.mAxisRZ = _controller.getAxisValue(env, Controller::AXIS_RZ);
}

/******************************************************************************/

JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnKeyEvent(JNIEnv * env, jobject listener, jobject event)
{
	KeyEvent _event(event);

	switch(_event.getKeyCode(env))
	{
	case KeyEvent::KEYCODE_BUTTON_A:
		mPlayer.mButtonA = _event.getAction(env);
		break;

	case KeyEvent::KEYCODE_BUTTON_B:
		mPlayer.mButtonB = _event.getAction(env);
		break;

	case KeyEvent::KEYCODE_BUTTON_START:
		mPlayer.mButtonStart = _event.getAction(env);
		break;
	}
}

JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnMotionEvent(JNIEnv * env, jobject listener, jobject event)
{
	MotionEvent _event(event);

	mPlayer.mAxisX = _event.getAxisValue(env, MotionEvent::AXIS_X);
	mPlayer.mAxisY = _event.getAxisValue(env, MotionEvent::AXIS_Y);
	mPlayer.mAxisZ = _event.getAxisValue(env, MotionEvent::AXIS_Z);
	mPlayer.mAxisRZ = _event.getAxisValue(env, MotionEvent::AXIS_RZ);
}

JNIEXPORT void JNICALL Java_com_bda_controller_example_nativeactivity_listen_ExampleActivity_00024ExampleControllerListener_nativeOnStateEvent(JNIEnv * env, jobject listener, jobject event)
{
	StateEvent _event(event);

	switch(_event.getState(env))
	{
	case StateEvent::STATE_CONNECTION:
		mPlayer.mConnection = _event.getAction(env);
		break;
	case StateEvent::STATE_SELECTED_VERSION:
		mPlayer.mControllerVersion = _event.getAction(env);
		break;
	}
}

/******************************************************************************/

void nativeOnDrawFrame(JNIEnv * env)
{
	if(mPlayer.mButtonStart == Controller::ACTION_DOWN)
	{
		mPlayer.mScale = ExamplePlayer::DEFAULT_SCALE;
		mPlayer.mX = ExamplePlayer::DEFAULT_X;
		mPlayer.mY = ExamplePlayer::DEFAULT_Y;
	}
	else
	{
		if(mPlayer.mButtonA == Controller::ACTION_DOWN)
		{
			mPlayer.mScale -= 0.1f;
			if(mPlayer.mScale < 1.0f)
			{
				mPlayer.mScale = 1.0f;
			}
		}
		if(mPlayer.mButtonB  == Controller::ACTION_DOWN)
		{
			mPlayer.mScale += 0.1f;
			if(mPlayer.mScale > 8.0f)
			{
				mPlayer.mScale = 8.0f;
			}
		}
	}

	const float scale = 10.0f;
	mPlayer.mX += (mPlayer.mAxisX + mPlayer.mAxisZ) * scale;
	mPlayer.mY -= (mPlayer.mAxisY + mPlayer.mAxisRZ) * scale;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(mPlayer.mX, mPlayer.mY, 0.0f);
	glScalef(mPlayer.mScale, mPlayer.mScale, mPlayer.mScale);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(mPlayer.mR, mPlayer.mG, mPlayer.mB, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, mVertices);
	if(mPlayer.mConnection == Controller::ACTION_CONNECTED)
	{
		// Check Moga Version
		if(mPlayer.mControllerVersion == Controller::ACTION_VERSION_MOGA)
		{
			// Blue = Moga
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (mPlayer.mControllerVersion == Controller::ACTION_VERSION_MOGAPRO)
		{
			// Green = Moga Pro
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			// Red = Unknown
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	else
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}

void nativeOnSurfaceChanged(JNIEnv * env, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(width / -2.0f, width / +2.0f, height / -2.0f, height / +2.0f, -1.0f, 1.0f);
}

void nativeOnSurfaceCreated(JNIEnv * env)
{
}
