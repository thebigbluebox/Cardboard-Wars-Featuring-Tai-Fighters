package com.example.ryan.mogatests;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import com.bda.controller.Controller;



public class MyActivity extends Activity {

    Controller mController = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        //mController = Controller.getInstance(this);
        //mController.init();
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

    /*@Override
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
    }*/
}
