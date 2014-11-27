package com.bda.controller.example.test.listen;

/******************************************************************************/

import java.util.Map.Entry;
import java.util.TreeMap;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;
import com.bda.controller.KeyEvent;
import com.bda.controller.MotionEvent;
import com.bda.controller.StateEvent;

/******************************************************************************/

public class ExampleActivity extends Activity
{
	static final int DELAY = 1000 / 50; // 50 Hz
	
	public boolean toastShown = false;
	
	static final int ACTION_CONNECTED = Controller.ACTION_CONNECTED;
	static final int ACTION_DISCONNECTED = Controller.ACTION_DISCONNECTED;
	static final int ACTION_VERSION_MOGA = Controller.ACTION_VERSION_MOGA;
	static final int ACTION_VERSION_MOGAPRO = Controller.ACTION_VERSION_MOGAPRO;

	Controller mController = null;
	final ExampleRunnable mRunnable = new ExampleRunnable();
	final ExampleThread mThread = new ExampleThread();

	final TreeMap<Integer, ExampleInteger> mStates = new TreeMap<Integer, ExampleInteger>();
	final TreeMap<Integer, ExampleInteger> mKeys = new TreeMap<Integer, ExampleInteger>();
	final TreeMap<Integer, ExampleFloat> mMotions = new TreeMap<Integer, ExampleFloat>();

	public ExampleActivity()
	{
		mStates.put(StateEvent.STATE_CONNECTION, new ExampleInteger("STATE_CONNECTION......"));
		mStates.put(StateEvent.STATE_POWER_LOW, new ExampleInteger("STATE_POWER_LOW......"));
		mStates.put(StateEvent.STATE_CURRENT_PRODUCT_VERSION, new ExampleInteger("STATE_CURRENT_PRODUCT_VERSION"));
		mStates.put(StateEvent.STATE_SUPPORTED_PRODUCT_VERSION, new ExampleInteger("STATE_SUPPORTED_PRODUCT_VERSION"));

		mKeys.put(KeyEvent.KEYCODE_DPAD_UP, new ExampleInteger("KEYCODE_DPAD_UP......"));
		mKeys.put(KeyEvent.KEYCODE_DPAD_DOWN, new ExampleInteger("KEYCODE_DPAD_DOWN......"));
		mKeys.put(KeyEvent.KEYCODE_DPAD_LEFT, new ExampleInteger("KEYCODE_DPAD_LEFT......"));
		mKeys.put(KeyEvent.KEYCODE_DPAD_RIGHT, new ExampleInteger("KEYCODE_DPAD_RIGHT......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_A, new ExampleInteger("KEYCODE_BUTTON_A......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_B, new ExampleInteger("KEYCODE_BUTTON_B......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_X, new ExampleInteger("KEYCODE_BUTTON_X......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_Y, new ExampleInteger("KEYCODE_BUTTON_Y......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_L1, new ExampleInteger("KEYCODE_BUTTON_L1......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_R1, new ExampleInteger("KEYCODE_BUTTON_R1......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_L2, new ExampleInteger("KEYCODE_BUTTON_L2......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_R2, new ExampleInteger("KEYCODE_BUTTON_R2......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_THUMBL, new ExampleInteger("KEYCODE_BUTTON_THUMBL......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_THUMBR, new ExampleInteger("KEYCODE_BUTTON_THUMBR......"));		
		mKeys.put(KeyEvent.KEYCODE_BUTTON_START, new ExampleInteger("KEYCODE_BUTTON_START......"));
		mKeys.put(KeyEvent.KEYCODE_BUTTON_SELECT, new ExampleInteger("KEYCODE_BUTTON_SELECT......"));

		mMotions.put(MotionEvent.AXIS_X, new ExampleFloat("AXIS_X........."));
		mMotions.put(MotionEvent.AXIS_Y, new ExampleFloat("AXIS_Y........."));
		mMotions.put(MotionEvent.AXIS_Z, new ExampleFloat("AXIS_Z........."));
		mMotions.put(MotionEvent.AXIS_RZ, new ExampleFloat("AXIS_RZ......."));
		mMotions.put(MotionEvent.AXIS_LTRIGGER, new ExampleFloat("AXIS_LTRIGGER........."));
		mMotions.put(MotionEvent.AXIS_RTRIGGER, new ExampleFloat("AXIS_RTRIGGER........."));
	}

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.example);

		// final View view = findViewById(android.R.id.content);
		// view.setKeepScreenOn(true);

		final LayoutInflater layoutInflater = getLayoutInflater();
		final TableLayout tableLayout = (TableLayout) findViewById(R.id.table);

		final View viewStates = tableLayout.findViewById(R.id.textview_states);
		int indexStates = tableLayout.indexOfChild(viewStates);
		for(final ExampleInteger value : mStates.values())
		{
			final View view = layoutInflater.inflate(R.layout.example_item, null);

			final TextView textView = (TextView) view.findViewById(R.id.textview_key);
			textView.setText(value.mName);

			value.mTextView = (TextView) view.findViewById(R.id.textview_value);

			tableLayout.addView(view, ++indexStates);
		}

		final View viewKeys = tableLayout.findViewById(R.id.textview_keys);
		int indexKeys = tableLayout.indexOfChild(viewKeys);
		for(final ExampleInteger value : mKeys.values())
		{
			final View view = layoutInflater.inflate(R.layout.example_item, null);

			final TextView textView = (TextView) view.findViewById(R.id.textview_key);
			textView.setText(value.mName);

			value.mTextView = (TextView) view.findViewById(R.id.textview_value);

			tableLayout.addView(view, ++indexKeys);
		}

		final View viewMotions = tableLayout.findViewById(R.id.textview_motions);
		int indexMotions = tableLayout.indexOfChild(viewMotions);
		for(final ExampleFloat value : mMotions.values())
		{
			final View view = layoutInflater.inflate(R.layout.example_item, null);

			final TextView textView = (TextView) view.findViewById(R.id.textview_key);
			textView.setText(value.mName);

			value.mTextView = (TextView) view.findViewById(R.id.textview_value);

			tableLayout.addView(view, ++indexMotions);
		}

		mController = Controller.getInstance(this);
		mController.init();
		mController.setListener(new ExampleControllerListener(), new Handler());

		// start game thread
		mThread.start();
	}

	@Override
	protected void onDestroy()
	{
		// stop game thread
		while(mThread.isAlive())
		{
			mThread.interrupt();
			try
			{
				mThread.join();
			}
			catch(final InterruptedException e)
			{
			}
		}

		mController.exit();
		super.onDestroy();
	}

	@Override
	protected void onPause()
	{
		mController.onPause();
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		mController.onResume();

		toastShown = false;
		
		for(final Entry<Integer, ExampleInteger> entry : mStates.entrySet())
		{
			final int key = entry.getKey();
			final ExampleInteger value = entry.getValue();
			value.mValue = mController.getState(key);
		}
		
		for(final Entry<Integer, ExampleInteger> entry : mKeys.entrySet())
		{
			final int key = entry.getKey();
			final ExampleInteger value = entry.getValue();
			value.mValue = mController.getKeyCode(key);
		}

		for(final Entry<Integer, ExampleFloat> entry : mMotions.entrySet())
		{
			final int key = entry.getKey();
			final ExampleFloat value = entry.getValue();
			value.mValue = mController.getAxisValue(key);
		}
	}
	public void onClickItem(View view){
		// do nothing.
	}
	
	public void showToast()
	{
	    runOnUiThread(new Runnable() {
	        public void run()
	        {
	    		int padVersion = mStates.get(StateEvent.STATE_CURRENT_PRODUCT_VERSION).mValue;

	    		if (padVersion == ACTION_VERSION_MOGA)
	    		{
	    			Toast.makeText(ExampleActivity.this, "Moga Controller Connected", Toast.LENGTH_SHORT).show();
	    		}
	    		else if (padVersion == ACTION_VERSION_MOGAPRO)
	    		{
	    			Toast.makeText(ExampleActivity.this, "Moga Pro Controller Connected", Toast.LENGTH_SHORT).show();
	    		}
	    		else
	    		{
	    			Toast.makeText(ExampleActivity.this, "Unknown Controller Connected", Toast.LENGTH_SHORT).show();
	    		}
	            
	        }
	    });
	}

	class ExampleControllerListener implements ControllerListener
	{
		@Override
		public void onKeyEvent(KeyEvent event)
		{
			final ExampleInteger value = mKeys.get(event.getKeyCode());
			if(value != null)
			{
				value.mValue = event.getAction();
			}
		}

		@Override
		public void onMotionEvent(MotionEvent event)
		{
			for(final Entry<Integer, ExampleFloat> entry : mMotions.entrySet())
			{
				final int key = entry.getKey();
				final ExampleFloat value = entry.getValue();
				value.mValue = event.getAxisValue(key);
			}
		}

		@Override
		public void onStateEvent(StateEvent event)
		{
			final ExampleInteger value = mStates.get(event.getState());
			if(value != null)
			{
				value.mValue = event.getAction();
			}
		}
	}

	class ExampleFloat
	{
		final String mName;
		float mValue;
		TextView mTextView;

		ExampleFloat(String name)
		{
			mName = name;
		}
	}

	class ExampleInteger
	{
		final String mName;
		int mValue;
		TextView mTextView;

		ExampleInteger(String name)
		{
			mName = name;
		}
	}

	class ExampleRunnable implements Runnable
	{
		@Override
		public void run()
		{
			for(final ExampleInteger value : mStates.values())
			{
				value.mTextView.setText(String.valueOf(value.mValue));
			}

			for(final ExampleInteger value : mKeys.values())
			{
				value.mTextView.setText(String.valueOf(value.mValue));
			}

			for(final ExampleFloat value : mMotions.values())
			{
				value.mTextView.setText(String.valueOf(value.mValue));
			}
		}
	}

	class ExampleThread extends Thread
	{

		@Override
		public void run()
		{
			while(!isInterrupted())
			{
				final long time = SystemClock.uptimeMillis();

				// update world
				if (!toastShown)
				{
					if (mStates.get(StateEvent.STATE_CONNECTION).mValue == ACTION_CONNECTED)
					{
						showToast();

						// Update States Display on Reconnection because Pad Version doesn't reset.
						for(final Entry<Integer, ExampleInteger> entry : mStates.entrySet())
						{
							final int key = entry.getKey();
							final ExampleInteger value = entry.getValue();
							value.mValue = mController.getState(key);
						}
						
						toastShown = true;
					}
				}
				
				if (mStates.get(StateEvent.STATE_CONNECTION).mValue == ACTION_DISCONNECTED)
				{
					toastShown = false;
				}
				
				// render display
				runOnUiThread(mRunnable);

				// limit frame rate
				final long elapsedTime = SystemClock.uptimeMillis() - time;
				final long sleepTime = DELAY - elapsedTime;
				if(sleepTime > 0)
				{
					try
					{
						sleep(sleepTime);
					}
					catch(final InterruptedException e)
					{
						interrupt();
					}
				}
			}
		}
	}
}