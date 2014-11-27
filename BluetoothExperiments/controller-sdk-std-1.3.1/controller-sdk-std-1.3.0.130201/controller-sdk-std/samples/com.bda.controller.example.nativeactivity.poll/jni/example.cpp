#include <GLES\gl.h>
#include "controller.h"
#include "example.h"

/******************************************************************************/

class ExamplePlayer
{
public:
	static const float DEFAULT_SCALE = 4.0f;
	static const float DEFAULT_X = 0.0f;
	static const float DEFAULT_Y = 0.0f;

	const float mR;
	const float mG;
	const float mB;
	float mScale;
	float mX;
	float mY;

public:
	ExamplePlayer(float r, float g, float b) : mR(r), mG(g), mB(b)
	{
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
}

void nativeOnUnload(JNIEnv * env)
{
	Controller::jniOnUnload(env);
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
	Controller _controller(mController);
	_controller.onResume(env);
}

/******************************************************************************/

void nativeOnDrawFrame(JNIEnv * env)
{
	Controller _controller(mController);
	const jint connection = _controller.getState(env, Controller::STATE_CONNECTION);
	const jint mControllerVersion = _controller.getState(env, Controller::STATE_SELECTED_VERSION);
	const jint buttonA = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_A);
	const jint buttonB = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_B);
	const jint buttonStart = _controller.getKeyCode(env, Controller::KEYCODE_BUTTON_START);
	const jfloat axisX = _controller.getAxisValue(env, Controller::AXIS_X);
	const jfloat axisY = _controller.getAxisValue(env, Controller::AXIS_Y);
	const jfloat axisZ = _controller.getAxisValue(env, Controller::AXIS_Z);
	const jfloat axisRZ = _controller.getAxisValue(env, Controller::AXIS_RZ);

	if(buttonStart == Controller::ACTION_DOWN)
	{
		mPlayer.mScale = ExamplePlayer::DEFAULT_SCALE;
		mPlayer.mX = ExamplePlayer::DEFAULT_X;
		mPlayer.mY = ExamplePlayer::DEFAULT_Y;
	}
	else
	{
		if(buttonA == Controller::ACTION_DOWN)
		{
			mPlayer.mScale -= 0.1f;
			if(mPlayer.mScale < 1.0f)
			{
				mPlayer.mScale = 1.0f;
			}
		}
		if(buttonB == Controller::ACTION_DOWN)
		{
			mPlayer.mScale += 0.1f;
			if(mPlayer.mScale > 8.0f)
			{
				mPlayer.mScale = 8.0f;
			}
		}
	}

	const float scale = 10.0f;
	mPlayer.mX += (axisX + axisZ) * scale;
	mPlayer.mY -= (axisY + axisRZ) * scale;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(mPlayer.mX, mPlayer.mY, 0.0f);
	glScalef(mPlayer.mScale, mPlayer.mScale, mPlayer.mScale);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mVertices);
	if(connection == Controller::ACTION_CONNECTED)
	{
		// Check Moga Version
		// Blue = Moga
		if(mControllerVersion == Controller::ACTION_VERSION_MOGA)
		{
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		}
		// Green = Moga Pro
		else if (mControllerVersion == Controller::ACTION_VERSION_MOGAPRO)
		{
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		}
		// Red = Unknown
		else
		{
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
