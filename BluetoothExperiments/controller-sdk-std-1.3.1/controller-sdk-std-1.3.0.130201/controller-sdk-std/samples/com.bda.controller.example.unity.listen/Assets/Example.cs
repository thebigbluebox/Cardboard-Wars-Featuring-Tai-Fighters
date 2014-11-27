using UnityEngine;
using System;

public class Example : MonoBehaviour
{
	/*
	 * Need to track the input focus state.
	 * As Awake() can be called after OnApplicationFocus().
	 */

	private Controller mController;
	private bool mFocused;
	private readonly Vector3 mMaxScale = new Vector3(8.0f, 8.0f, 8.0f);
	private readonly Vector3 mMinScale = new Vector3(0.1f, 0.1f, 0.1f);
	private readonly Vector3 mStepScale = new Vector3(0.1f, 0.1f, 0.1f);
	private GameObject mPlayer;
	private int mConnection = Controller.ACTION_DISCONNECTED;
	private int padVersion = Controller.ACTION_VERSION_MOGA;
	private int mButtonStart = Controller.ACTION_UP;
	private int mButtonA = Controller.ACTION_UP;
	private int mButtonB = Controller.ACTION_UP;
	private float mAxisX = 0.0f;
	private float mAxisY = 0.0f;
	private float mAxisZ = 0.0f;
	private float mAxisRZ = 0.0f;

	void Awake()
	{
		mPlayer = GameObject.Find("Cube");

		AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
		AndroidJavaObject activity = jc.GetStatic<AndroidJavaObject>("currentActivity");

		AndroidJavaObject controller = Controller.getInstance(activity);
		mController = new Controller(controller);
		mController.init();

		AndroidJavaObject listener = new AndroidJavaObject("com.bda.controller.example.unity.listen.plugin.ExampleActivity$ExampleControllerListener");
		mController.setListener(listener, null);

		if(mFocused)
		{
			resumeController();
		}
	}

	void OnApplicationFocus(bool focus)
	{
		mFocused = focus;
		if(mFocused)
		{
			resumeController();
		}
		else
		{
			pauseController();
		}
	}

	void OnDestroy()
	{
		mController.exit();
		mController = null;
	}
	
	void resumeController()
	{
		if(mController != null)
		{
			mController.onResume();
		}
	}
	
	void pauseController()
	{
		if(mController != null)
		{
			mController.onPause();
		}
	}

	/* UnityPlayer.UnitySendMessage callback */
	void onKeyEvent(string message)
	{
		try
		{
			string[] components = message.Split(' ');
			int keyCode = Convert.ToInt32(components[0]);
			int action = Convert.ToInt32(components[1]);
			switch(keyCode)
			{
			case Controller.KEYCODE_BUTTON_A:
				mButtonA = action;
				break;

			case Controller.KEYCODE_BUTTON_B:
				mButtonB = action;
				break;

			case Controller.KEYCODE_BUTTON_START:
				mButtonStart = action;
				break;
			}
		}
		catch(FormatException)
		{
		}
		catch(IndexOutOfRangeException)
		{
		}
		catch(OverflowException)
		{
		}
	}

	/* UnityPlayer.UnitySendMessage callback */
	void onMotionEvent(string message)
	{
		try
		{
			string[] components = message.Split(' ');
			int axis = Convert.ToInt32(components[0]);
			float axisValue = Convert.ToSingle(components[1]);
			switch(axis)
			{
			case Controller.AXIS_X:
				mAxisX = axisValue;
				break;

			case Controller.AXIS_Y:
				mAxisY = axisValue;
				break;

			case Controller.AXIS_Z:
				mAxisZ = axisValue;
				break;

			case Controller.AXIS_RZ:
				mAxisRZ = axisValue;
				break;
			}
		}
		catch(FormatException)
		{
		}
		catch(IndexOutOfRangeException)
		{
		}
		catch(OverflowException)
		{
		}
	}

	/* UnityPlayer.UnitySendMessage callback */
	void onStateEvent(string message)
	{
		try
		{
			string[] components = message.Split(' ');
			int state = Convert.ToInt32(components[0]);
			int action = Convert.ToInt32(components[1]);
			switch(state)
			{
			case Controller.STATE_CONNECTION:
				mConnection = action;
				break;
			case Controller.STATE_SELECTED_VERSION:
				padVersion = action;
				break;
			}
		}
		catch(FormatException)
		{
		}
		catch(IndexOutOfRangeException)
		{
		}
		catch(OverflowException)
		{
		}
	}

	void Update()
	{
		if(mButtonStart == Controller.ACTION_DOWN)
		{
			mPlayer.transform.position = Vector3.zero;
			mPlayer.transform.localScale = Vector3.one;
		}
		else
		{
			if(mButtonA == Controller.ACTION_DOWN)
			{
				mPlayer.transform.localScale -= mStepScale;
				mPlayer.transform.localScale = Vector3.Max(mPlayer.transform.localScale, mMinScale);
			}
			if(mButtonB == Controller.ACTION_DOWN)
			{
				mPlayer.transform.localScale += mStepScale;
				mPlayer.transform.localScale = Vector3.Min(mPlayer.transform.localScale, mMaxScale);
			}
		}

		const float scale = 0.5f;
		mPlayer.transform.position += new Vector3(+(mAxisX + mAxisZ), -(mAxisY + mAxisRZ), 0.0f) * scale;
		mPlayer.transform.localEulerAngles += Vector3.up;

		if(mConnection == Controller.ACTION_CONNECTED)
		{
			if (padVersion == Controller.ACTION_VERSION_MOGA)
			{
				mPlayer.renderer.material.color = Color.blue;
			}
			else if (padVersion == Controller.ACTION_VERSION_MOGAPRO)
			{
				mPlayer.renderer.material.color = Color.green;
			}
			else
			{
				mPlayer.renderer.material.color = Color.red;
			}
		}
		else
		{
			mPlayer.renderer.material.color = Color.grey;
		}

		if(Input.GetKeyDown(KeyCode.Escape))
		{
			Application.Quit();
		}
	}
}
