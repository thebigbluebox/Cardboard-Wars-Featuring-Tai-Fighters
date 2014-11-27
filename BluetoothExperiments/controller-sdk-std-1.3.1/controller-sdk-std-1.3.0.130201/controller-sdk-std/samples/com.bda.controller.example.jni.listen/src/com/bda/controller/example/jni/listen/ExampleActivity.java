package com.bda.controller.example.jni.listen;

/******************************************************************************/

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;
import com.bda.controller.KeyEvent;
import com.bda.controller.MotionEvent;
import com.bda.controller.StateEvent;

/******************************************************************************/

public class ExampleActivity extends Activity
{
	Controller mController = null;
	GLSurfaceView mView = null;

	static
	{
		System.loadLibrary("example");
	}

	native void nativeOnCreate();

	native void nativeOnDestroy();

	native void nativeOnPause();

	native void nativeOnResume();

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		nativeOnCreate();

		mView = new GLSurfaceView(this);
		mView.setRenderer(new ExampleRenderer());
		mView.setKeepScreenOn(true);
		setContentView(mView);
	}

	@Override
	protected void onDestroy()
	{
		nativeOnDestroy();
		super.onDestroy();
	}

	@Override
	protected void onPause()
	{
		nativeOnPause();
		mView.onPause();
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		mView.onResume();
		nativeOnResume();
	}

	public static class ExampleControllerListener implements ControllerListener
	{
		native void nativeOnKeyEvent(KeyEvent event);

		native void nativeOnMotionEvent(MotionEvent event);

		native void nativeOnStateEvent(StateEvent event);

		@Override
		public void onKeyEvent(KeyEvent event)
		{
			nativeOnKeyEvent(event);
		}

		@Override
		public void onMotionEvent(MotionEvent event)
		{
			nativeOnMotionEvent(event);
		}

		@Override
		public void onStateEvent(StateEvent event)
		{
			nativeOnStateEvent(event);
		}
	}

	class ExampleRenderer implements GLSurfaceView.Renderer
	{
		native void nativeOnDrawFrame(GL10 gl);

		native void nativeOnSurfaceChanged(GL10 gl, int width, int height);

		native void nativeOnSurfaceCreated(GL10 gl, EGLConfig config);

		@Override
		public void onDrawFrame(GL10 gl)
		{
			nativeOnDrawFrame(gl);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			nativeOnSurfaceChanged(gl, width, height);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			nativeOnSurfaceCreated(gl, config);
		}
	}
}