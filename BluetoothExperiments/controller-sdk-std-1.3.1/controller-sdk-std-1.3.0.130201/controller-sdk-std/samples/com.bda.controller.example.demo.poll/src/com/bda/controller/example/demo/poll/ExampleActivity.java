package com.bda.controller.example.demo.poll;

/******************************************************************************/

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.bda.controller.Controller;
import com.bda.controller.KeyEvent;

/******************************************************************************/

public class ExampleActivity extends Activity
{
	Controller mController = null;
	final ExamplePlayer mPlayer = new ExamplePlayer(0.0f, 1.0f, 0.0f);
	GLSurfaceView mView = null;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		mController = Controller.getInstance(this);
		mController.init();

		mView = new GLSurfaceView(this);
		mView.setRenderer(new ExampleRenderer());
		mView.setKeepScreenOn(true);
		setContentView(mView);
	}

	@Override
	protected void onDestroy()
	{
		mController.exit();
		super.onDestroy();
	}

	@Override
	protected void onPause()
	{
		mController.onPause();
		mView.onPause();
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		mView.onResume();
		mController.onResume();
	}

	class ExamplePlayer
	{
		static final float DEFAULT_SCALE = 4.0f;
		static final float DEFAULT_X = 0.0f;
		static final float DEFAULT_Y = 0.0f;

		final float mR;
		final float mG;
		final float mB;
		float mScale = DEFAULT_SCALE;
		float mX = DEFAULT_X;
		float mY = DEFAULT_Y;

		public ExamplePlayer(float r, float g, float b)
		{
			mR = r;
			mG = g;
			mB = b;
		}
	}

	class ExampleRenderer implements GLSurfaceView.Renderer
	{
		final FloatBuffer mVertexBuffer;

		public ExampleRenderer()
		{
			final float[] vertices = {5.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, -5.0f, 0.0f};
			final ByteBuffer byteBuffer = ByteBuffer.allocateDirect(vertices.length * 4);
			byteBuffer.order(ByteOrder.nativeOrder());
			mVertexBuffer = byteBuffer.asFloatBuffer();
			mVertexBuffer.put(vertices);
			mVertexBuffer.position(0);
		}

		@Override
		public void onDrawFrame(GL10 gl)
		{
			final int connection = mController.getState(Controller.STATE_CONNECTION);
			final int mControllerVersion = mController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION); // Get the Controller Version
			final int buttonA = mController.getKeyCode(Controller.KEYCODE_BUTTON_A);
			final int buttonB = mController.getKeyCode(Controller.KEYCODE_BUTTON_B);
			final int buttonX = mController.getKeyCode(Controller.KEYCODE_BUTTON_X);
			final int buttonY = mController.getKeyCode(Controller.KEYCODE_BUTTON_Y);
			final int buttonStart = mController.getKeyCode(Controller.KEYCODE_BUTTON_START);
			final float axisX = mController.getAxisValue(Controller.AXIS_X);
			final float axisY = mController.getAxisValue(Controller.AXIS_Y);
			final float axisZ = mController.getAxisValue(Controller.AXIS_Z);
			final float axisRZ = mController.getAxisValue(Controller.AXIS_RZ);


			
			switch (mController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION)) {
			case Controller.ACTION_VERSION_MOGA:
				if(buttonStart == Controller.ACTION_DOWN)
				{
					mPlayer.mScale = ExamplePlayer.DEFAULT_SCALE;
					mPlayer.mX = ExamplePlayer.DEFAULT_X;
					mPlayer.mY = ExamplePlayer.DEFAULT_Y;
				}
				else
				{
					if(buttonA == Controller.ACTION_DOWN)
					{
						mPlayer.mScale -= 0.1f;
						mPlayer.mScale = Math.max(mPlayer.mScale, 1.0f);
					}
					if(buttonB == Controller.ACTION_DOWN)
					{
						mPlayer.mScale += 0.1f;
						mPlayer.mScale = Math.min(mPlayer.mScale, 8.0f);
					}
				}
				break;
			case Controller.ACTION_VERSION_MOGAPRO:
				if(buttonStart == Controller.ACTION_DOWN)
				{
					mPlayer.mScale = ExamplePlayer.DEFAULT_SCALE;
					mPlayer.mX = ExamplePlayer.DEFAULT_X;
					mPlayer.mY = ExamplePlayer.DEFAULT_Y;
				}
				else
				{
					if(buttonX == Controller.ACTION_DOWN)
					{
						mPlayer.mScale -= 0.1f;
						mPlayer.mScale = Math.max(mPlayer.mScale, 1.0f);
					}
					if(buttonY == Controller.ACTION_DOWN)
					{
						mPlayer.mScale += 0.1f;
						mPlayer.mScale = Math.min(mPlayer.mScale, 8.0f);
					}
				}
			}

			final float scale = 10.0f;
			mPlayer.mX += (axisX + axisZ) * scale;
			mPlayer.mY -= (axisY + axisRZ) * scale;

			gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
			gl.glMatrixMode(GL10.GL_MODELVIEW);
			gl.glLoadIdentity();
			gl.glTranslatef(mPlayer.mX, mPlayer.mY, 0.0f);
			gl.glScalef(mPlayer.mScale, mPlayer.mScale, mPlayer.mScale);
			gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
			gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVertexBuffer);
			if(connection == Controller.ACTION_CONNECTED)
			{
				// Check Moga Version
				if(mControllerVersion == Controller.ACTION_VERSION_MOGA)
				{
					// Blue = Moga
					gl.glColor4f(0.0f, 0.0f, 1.0f, 1.0f);		
				}
				else if (mControllerVersion == Controller.ACTION_VERSION_MOGAPRO)
				{
					// Green = Moga Pro
					gl.glColor4f(0.0f, 1.0f, 0.0f, 1.0f);		

				}
				else
				{
					// Red = Unknown
					gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	
				}
				gl.glDrawArrays(GL10.GL_TRIANGLE_FAN, 0, 4);
			}
			else
			{
				gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				gl.glDrawArrays(GL10.GL_LINE_LOOP, 0, 4);
			}
			gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			gl.glViewport(0, 0, width, height);
			gl.glMatrixMode(GL10.GL_PROJECTION);
			gl.glLoadIdentity();
			gl.glOrthof(width / -2.0f, width / +2.0f, height / -2.0f, height / +2.0f, -1.0f, 1.0f);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
		}
	}
}