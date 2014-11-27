package com.example.ryan.mogatests;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import com.bda.controller.Controller;



public class MyActivity extends Activity {

    Controller mController = null;
    float mX = 0;
    float mY = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        mController = Controller.getInstance(this);
        mController.init();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.my, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        if(mController != null)
        {
            mController.onPause();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        if(mController != null)
        {
            mController.onResume();
        }
    }

    @Override
    protected void onDestroy()
    {
        if(mController != null)
        {
            mController.exit();
        }
        super.onDestroy();
    }


    void update()
    {
        if(mController != null)
        {
            // test if controller is connected
            if(mController.getState(Controller.STATE_CONNECTION) == Controller.ACTION_CONNECTED)
            {
                //test if button a is pressed
                if(mController.getKeyCode(Controller.KEYCODE_BUTTON_A) == Controller.ACTION_DOWN)
                {
                    //button a is pressed
                    Log.d("Label", "a is pushed");
                }
                else
                {
                    //button A is released
                }

                //test if d-pad left is pressed
                if(mController.getKeyCode(Controller.KEYCODE_DPAD_LEFT) == Controller.ACTION_DOWN)
                {
                    //d pad left is pressed
                    Log.d("Label", "dpad left is pushed");
                }
                else
                {
                    //dpad left is released
                }

                //read left analog stick
                mX = mController.getAxisValue(Controller.AXIS_X);
                mY = mController.getAxisValue(Controller.AXIS_Y);

                //test if controller is in low power state
                if(mController.getState(Controller.STATE_POWER_LOW) == Controller.ACTION_TRUE)
                {
                    //low power state
                }
                else
                {
                    //normal power state
                }
            }
        }
    }


}
