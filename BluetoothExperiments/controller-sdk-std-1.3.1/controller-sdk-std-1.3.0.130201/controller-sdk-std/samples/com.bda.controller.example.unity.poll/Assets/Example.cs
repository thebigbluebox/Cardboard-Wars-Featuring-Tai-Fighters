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

	void Awake()
	{
		mPlayer = GameObject.Find("Cube");

		AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
		AndroidJavaObject activity = jc.GetStatic<AndroidJavaObject>("currentActivity");

		AndroidJavaObject controller = Controller.getInstance(activity);
		mController = new Controller(controller);
		mController.init();
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
	

	void Update()
	{
		int connection = mController.getState(Controller.STATE_CONNECTION);
		int padVersion = mController.getState(Controller.STATE_SELECTED_VERSION);
		int buttonStart = mController.getKeyCode(Controller.KEYCODE_BUTTON_START);
		int buttonA = mController.getKeyCode(Controller.KEYCODE_BUTTON_A);
		int buttonB = mController.getKeyCode(Controller.KEYCODE_BUTTON_B);
		float axisX = mController.getAxisValue(Controller.AXIS_X);
		float axisY = mController.getAxisValue(Controller.AXIS_Y);
		float axisZ = mController.getAxisValue(Controller.AXIS_Z);
		float axisRZ = mController.getAxisValue(Controller.AXIS_RZ);

		if(buttonStart == Controller.ACTION_DOWN)
		{
			mPlayer.transform.position = Vector3.zero;
			mPlayer.transform.localScale = Vector3.one;
		}
		else
		{
			if(buttonA == Controller.ACTION_DOWN)
			{
				mPlayer.transform.localScale -= mStepScale;
				mPlayer.transform.localScale = Vector3.Max(mPlayer.transform.localScale, mMinScale);
			}
			if(buttonB == Controller.ACTION_DOWN)
			{
				mPlayer.transform.localScale += mStepScale;
				mPlayer.transform.localScale = Vector3.Min(mPlayer.transform.localScale, mMaxScale);
			}
		}

		const float scale = 0.5f;
		mPlayer.transform.position += new Vector3(+(axisX + axisZ), -(axisY + axisRZ), 0.0f) * scale;
		mPlayer.transform.localEulerAngles += Vector3.up;

		if(connection == Controller.ACTION_CONNECTED)
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
