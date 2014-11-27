/*
 * Bare-bones wrapper class for "com.bda.controller.Controller".
 * Requires error handling.
 */

/******************************************************************************/

public class Controller
{
	public const int ACTION_DOWN = 0;
	public const int ACTION_UP = 1;
	public const int ACTION_FALSE = 0;
	public const int ACTION_TRUE = 1;
	public const int ACTION_DISCONNECTED = 0;
	public const int ACTION_CONNECTED = 1;
	public const int ACTION_CONNECTING = 2;
	public const int ACTION_VERSION_MOGA = 0;
	public const int ACTION_VERSION_MOGAPRO = 1;
	
	public const int KEYCODE_DPAD_UP = 19;
	public const int KEYCODE_DPAD_DOWN = 20;
	public const int KEYCODE_DPAD_LEFT = 21;
	public const int KEYCODE_DPAD_RIGHT = 22;
	public const int KEYCODE_BUTTON_A = 96;
	public const int KEYCODE_BUTTON_B = 97;
#if BUILD_LEGACY_API
	// SDK 1.2.7b and earlier
	public const int KEYCODE_BUTTON_X = 98;
	public const int KEYCODE_BUTTON_Y = 99;
#else
	// SDK 1.2.7c and later
	public const int KEYCODE_BUTTON_X = 99;
	public const int KEYCODE_BUTTON_Y = 100;
#endif
	public const int KEYCODE_BUTTON_L1 = 102;
	public const int KEYCODE_BUTTON_R1 = 103;
	public const int KEYCODE_BUTTON_L2 = 104;
	public const int KEYCODE_BUTTON_R2 = 105;
	public const int KEYCODE_BUTTON_THUMBL = 106;
	public const int KEYCODE_BUTTON_THUMBR = 107;
	public const int KEYCODE_BUTTON_START = 108;
	public const int KEYCODE_BUTTON_SELECT = 109;

	public const int INFO_KNOWN_DEVICE_COUNT = 1;
	public const int INFO_ACTIVE_DEVICE_COUNT = 2;

	public const int AXIS_X = 0;
	public const int AXIS_Y = 1;
	public const int AXIS_Z = 11;
	public const int AXIS_RZ = 14;
	public const int AXIS_LTRIGGER = 17;
	public const int AXIS_RTRIGGER = 18;

	public const int STATE_CONNECTION = 1;
	public const int STATE_POWER_LOW = 2;
	public const int STATE_SUPPORTED_VERSION = 3;	// Controller Version
	public const int STATE_SELECTED_VERSION = 4;	// Service Controller Version
}
