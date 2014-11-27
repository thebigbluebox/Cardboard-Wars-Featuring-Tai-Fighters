using UnityEngine;
using System.Collections;

public class Example : MonoBehaviour
{
	private AndroidJavaObject mActivity;
	private readonly Vector3 mMaxScale = new Vector3(8.0f, 8.0f, 8.0f);
	private readonly Vector3 mMinScale = new Vector3(0.1f, 0.1f, 0.1f);
	private readonly Vector3 mStepScale = new Vector3(0.1f, 0.1f, 0.1f);
	private GameObject mPlayer;

	void Awake()
	{
		mPlayer = GameObject.Find("Cube");

		AndroidJavaClass unityPlayerCls = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
		mActivity = unityPlayerCls.GetStatic<AndroidJavaObject>("currentActivity");
	}

	void Update()
	{
		int connection = mActivity.Call<int>("getState", Controller.STATE_CONNECTION);
		int padVersion = mActivity.Call<int>("getState", Controller.STATE_SELECTED_VERSION);
		int buttonA = mActivity.Call<int>("getKeyCode", Controller.KEYCODE_BUTTON_A);
		int buttonB = mActivity.Call<int>("getKeyCode", Controller.KEYCODE_BUTTON_B);
		int buttonStart = mActivity.Call<int>("getKeyCode", Controller.KEYCODE_BUTTON_START);
		float axisX = mActivity.Call<float>("getAxisValue", Controller.AXIS_X);
		float axisY = mActivity.Call<float>("getAxisValue", Controller.AXIS_Y);
		float axisZ = mActivity.Call<float>("getAxisValue", Controller.AXIS_Z);
		float axisRZ = mActivity.Call<float>("getAxisValue", Controller.AXIS_RZ);

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
