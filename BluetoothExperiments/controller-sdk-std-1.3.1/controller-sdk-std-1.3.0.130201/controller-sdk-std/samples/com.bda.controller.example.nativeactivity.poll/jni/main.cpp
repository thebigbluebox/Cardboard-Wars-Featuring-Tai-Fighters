#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "example.h"

/******************************************************************************/

static void onAppCmd(struct android_app * app, int32_t cmd);
static int32_t onInputEvent(struct android_app * app, AInputEvent * event);

static void onInitWindow(struct android_app * app);
static void onTermWindow(void);

/******************************************************************************/

static JNIEnv * mEnv = NULL;
static EGLDisplay mDisplay = EGL_NO_DISPLAY;
static EGLSurface mSurface = EGL_NO_SURFACE;
static EGLContext mContext = EGL_NO_CONTEXT;
static bool mDrawFrame = false;

/******************************************************************************/

void android_main(struct android_app * app)
{
	app_dummy();

	app->activity->vm->AttachCurrentThread(&mEnv, NULL);
	app->onAppCmd = onAppCmd;
	app->onInputEvent = onInputEvent;

	nativeOnLoad(mEnv, app->activity->clazz);
	nativeOnCreate(mEnv, app->activity->clazz);

	for( ; ; )
	{
		int events;
		struct android_poll_source * source;
		const int timeoutMillis = mDrawFrame ? 0 : -1;
		if(ALooper_pollAll(timeoutMillis, NULL, &events, reinterpret_cast<void **>(&source)) >= 0)
		{
			if(source != NULL)
			{
				source->process(app, source);
			}
			
			if(app->destroyRequested)
			{
				break;
			}
		}
		else
		{
			if(mDrawFrame)
			{
				if(mDisplay != EGL_NO_DISPLAY)
				{
					nativeOnDrawFrame(mEnv);
					eglSwapBuffers(mDisplay, mSurface);
				}
			}
		}
	}

	nativeOnUnload(mEnv);
	app->activity->vm->DetachCurrentThread();
}

/******************************************************************************/

void onAppCmd(struct android_app * app, int32_t cmd)
{
	switch(cmd)
	{
	case APP_CMD_INIT_WINDOW:
		onInitWindow(app);
		break;

	case APP_CMD_TERM_WINDOW:
		onTermWindow();
		break;

	case APP_CMD_GAINED_FOCUS:
		mDrawFrame = true;
		break;

	case APP_CMD_LOST_FOCUS:
		mDrawFrame = false;
		break;

	case APP_CMD_RESUME:
		nativeOnResume(mEnv, app->activity->clazz);
		break;

	case APP_CMD_PAUSE:
		nativeOnPause(mEnv, app->activity->clazz);
		break;

	case APP_CMD_DESTROY:
		nativeOnDestroy(mEnv, app->activity->clazz);
		break;
	}
}

int32_t onInputEvent(struct android_app * app, AInputEvent * event)
{
	return 0;
}

/******************************************************************************/

void onInitWindow(struct android_app * app)
{
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	const EGLint attribs[] = 
	{
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

	EGLSurface surface = eglCreateWindowSurface(display, config, app->window, NULL);
	EGLContext context = eglCreateContext(display, config, NULL, NULL);

	if(eglMakeCurrent(display, surface, surface, context) != EGL_FALSE)
	{
		mDisplay = display;
		mContext = context;
		mSurface = surface;

		EGLint width;
		EGLint height;
		eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &width);
		eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &height);

		nativeOnSurfaceCreated(app->activity->env);
		nativeOnSurfaceChanged(app->activity->env, width, height);
	}
}

void onTermWindow(void)
{
	if(mDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(mContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(mDisplay, mContext);
		}
		if(mSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(mDisplay, mSurface);
		}
		eglTerminate(mDisplay);
	}
	mDisplay = EGL_NO_DISPLAY;
	mContext = EGL_NO_CONTEXT;
	mSurface = EGL_NO_SURFACE;
}
