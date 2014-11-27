package com.bda.controller.example.jni.poll;

/******************************************************************************/

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.bda.controller.Controller;

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