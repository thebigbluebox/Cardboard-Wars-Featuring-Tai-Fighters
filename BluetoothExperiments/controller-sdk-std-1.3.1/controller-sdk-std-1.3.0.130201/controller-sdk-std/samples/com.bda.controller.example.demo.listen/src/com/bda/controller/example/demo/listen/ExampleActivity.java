package com.bda.controller.example.demo.listen;

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
import com.bda.controller.ControllerListener;
import com.bda.controller.KeyEvent;
import com.bda.controller.MotionEvent;
import com.bda.controller.StateEvent;

/******************************************************************************/

public class ExampleActivity extends Activity {
	Controller mController = null;
	final ExampleControllerListener mListener = new ExampleControllerListener();
	final ExamplePlayer mPlayer = new ExamplePlayer(0.0f, 1.0f, 0.0f);
	GLSurfaceView mView = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mController = Controller.getInstance(this);
		mController.init();
		mController.setListener(mListener, null);

		mView = new GLSurfaceView(this);
		mView.setRenderer(new ExampleRenderer());
		mView.setKeepScreenOn(true);
		setContentView(mView);
	}

	@Override
	protected void onDestroy() {
		mController.exit();
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		mController.onPause();
		mView.onPause();
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mView.onResume();
		mController.onResume();

		mPlayer.mConnection = mController.getState(Controller.STATE_CONNECTION);
		mPlayer.mControllerVersion = mController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION); // Get
																										// the
																										// Controller
																										// Version
		mPlayer.mButtonA = mController.getKeyCode(Controller.KEYCODE_BUTTON_A);
		mPlayer.mButtonB = mController.getKeyCode(Controller.KEYCODE_BUTTON_B);
		mPlayer.mButtonStart = mController.getKeyCode(Controller.KEYCODE_BUTTON_START);
		mPlayer.mAxisX = mController.getAxisValue(Controller.AXIS_X);
		mPlayer.mAxisY = mController.getAxisValue(Controller.AXIS_Y);
		mPlayer.mAxisZ = mController.getAxisValue(Controller.AXIS_Z);
		mPlayer.mAxisRZ = mController.getAxisValue(Controller.AXIS_RZ);
	}

	class ExampleControllerListener implements ControllerListener {

		@Override
		public void onKeyEvent(KeyEvent event) {

			switch (mController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION)) {
			case Controller.ACTION_VERSION_MOGA:
				switch (event.getKeyCode()) {
				case KeyEvent.KEYCODE_BUTTON_A:
					mPlayer.mButtonA = event.getAction();
					break;

				case KeyEvent.KEYCODE_BUTTON_B:
					mPlayer.mButtonB = event.getAction();
					break;

				case KeyEvent.KEYCODE_BUTTON_START:
					mPlayer.mButtonStart = event.getAction();
					break;
				}
				break;
			case Controller.ACTION_VERSION_MOGAPRO:
				switch (event.getKeyCode()) {
				case KeyEvent.KEYCODE_BUTTON_X:
					mPlayer.mButtonA = event.getAction();
					break;

				case KeyEvent.KEYCODE_BUTTON_Y:
					mPlayer.mButtonB = event.getAction();
					break;

				case KeyEvent.KEYCODE_BUTTON_START:
					mPlayer.mButtonStart = event.getAction();
					break;
				}
			}
		}

		@Override
		public void onMotionEvent(MotionEvent event) {
			mPlayer.mAxisX = event.getAxisValue(MotionEvent.AXIS_X);
			mPlayer.mAxisY = event.getAxisValue(MotionEvent.AXIS_Y);
			mPlayer.mAxisZ = event.getAxisValue(MotionEvent.AXIS_Z);
			mPlayer.mAxisRZ = event.getAxisValue(MotionEvent.AXIS_RZ);
		}

		@Override
		public void onStateEvent(StateEvent event) {
			switch (event.getState()) {
			case StateEvent.STATE_CONNECTION:
				mPlayer.mConnection = event.getAction();
				break;
			case StateEvent.STATE_CURRENT_PRODUCT_VERSION:
				mPlayer.mControllerVersion = event.getAction();
				break;
			}
		}
	}

	class ExamplePlayer {
		static final float DEFAULT_SCALE = 4.0f;
		static final float DEFAULT_X = 0.0f;
		static final float DEFAULT_Y = 0.0f;

		boolean gotPadVersion = false;

		int mConnection = StateEvent.ACTION_DISCONNECTED;
		int mControllerVersion = StateEvent.STATE_UNKNOWN;
		int mButtonA = KeyEvent.ACTION_UP;
		int mButtonB = KeyEvent.ACTION_UP;
		int mButtonStart = KeyEvent.ACTION_UP;
		float mAxisX = 0.0f;
		float mAxisY = 0.0f;
		float mAxisZ = 0.0f;
		float mAxisRZ = 0.0f;
		final float mR;
		final float mG;
		final float mB;
		float mScale = DEFAULT_SCALE;
		float mX = DEFAULT_X;
		float mY = DEFAULT_Y;

		public ExamplePlayer(float r, float g, float b) {
			mR = r;
			mG = g;
			mB = b;
		}
	}

	class ExampleRenderer implements GLSurfaceView.Renderer {
		final FloatBuffer mVertexBuffer;

		public ExampleRenderer() {
			final float[] vertices = { 5.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, -5.0f, 0.0f };
			final ByteBuffer byteBuffer = ByteBuffer.allocateDirect(vertices.length * 4);
			byteBuffer.order(ByteOrder.nativeOrder());
			mVertexBuffer = byteBuffer.asFloatBuffer();
			mVertexBuffer.put(vertices);
			mVertexBuffer.position(0);
		}

		@Override
		public void onDrawFrame(GL10 gl) {
			if (mPlayer.mButtonStart == Controller.ACTION_DOWN) {
				mPlayer.mScale = ExamplePlayer.DEFAULT_SCALE;
				mPlayer.mX = ExamplePlayer.DEFAULT_X;
				mPlayer.mY = ExamplePlayer.DEFAULT_Y;
			} else {
				if (mPlayer.mButtonA == Controller.ACTION_DOWN) {
					mPlayer.mScale -= 0.1f;
					mPlayer.mScale = Math.max(mPlayer.mScale, 1.0f);
				}
				if (mPlayer.mButtonB == Controller.ACTION_DOWN) {
					mPlayer.mScale += 0.1f;
					mPlayer.mScale = Math.min(mPlayer.mScale, 8.0f);
				}
			}

			final float scale = 10.0f;
			mPlayer.mX += (mPlayer.mAxisX + mPlayer.mAxisZ) * scale;
			mPlayer.mY -= (mPlayer.mAxisY + mPlayer.mAxisRZ) * scale;

			gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
			gl.glMatrixMode(GL10.GL_MODELVIEW);
			gl.glLoadIdentity();
			gl.glTranslatef(mPlayer.mX, mPlayer.mY, 0.0f);
			gl.glScalef(mPlayer.mScale, mPlayer.mScale, mPlayer.mScale);
			gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
			gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVertexBuffer);
			if (mPlayer.mConnection == Controller.ACTION_CONNECTED) {
				// Check Moga Version
				if (!mPlayer.gotPadVersion) {
					mPlayer.mControllerVersion = mController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION);

					// Blue = Moga
					if (mPlayer.mControllerVersion == Controller.ACTION_VERSION_MOGA) {
						gl.glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
						mPlayer.gotPadVersion = true;
					}
					// Green = Moga Pro
					else if (mPlayer.mControllerVersion == Controller.ACTION_VERSION_MOGAPRO) {
						gl.glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
						mPlayer.gotPadVersion = true;
					}
					// Red = Unknown
					else {
						gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
						mPlayer.gotPadVersion = true;
					}
				}

				gl.glDrawArrays(GL10.GL_TRIANGLE_FAN, 0, 4);
			} else {
				gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				gl.glDrawArrays(GL10.GL_LINE_LOOP, 0, 4);
				mPlayer.gotPadVersion = false; // Disconnected
			}
			gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			gl.glViewport(0, 0, width, height);
			gl.glMatrixMode(GL10.GL_PROJECTION);
			gl.glLoadIdentity();
			gl.glOrthof(width / -2.0f, width / +2.0f, height / -2.0f, height / +2.0f, -1.0f, 1.0f);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		}
	}
}
