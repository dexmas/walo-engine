package com.waloteam.waloplayer;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.support.v4.content.ContextCompat;
import android.support.v4.app.ActivityCompat;
import android.Manifest;
import android.content.pm.PackageManager;

public class GameActivity extends Activity
{
    GameView mView;
    int[] idArr = new int[5];
    float[] xArr = new float[5];
    float[] yArr = new float[5];

    private final int REQUEST_PERMISSION_WRITE_INTERNAL_STORAGE=1;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        mView = new GameView(getApplication());
        setContentView(mView);

        String packagePath = getApplicationContext().getPackageResourcePath();
        String scriptPath = "assets/";
        String savePath = getFilesDir().getAbsolutePath() + "/save/";

        NativeWrapper.Create(getAssets(), packagePath, savePath);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();

        NativeWrapper.Pause();
    }

    @Override protected void onResume()
    {
        NativeWrapper.Resume();

        super.onResume();
        mView.onResume();
    }

    @Override public boolean onTouchEvent(final MotionEvent event)
    {
        int pID = event.getPointerId(event.getActionIndex());

        float x = event.getX(event.getActionIndex());
        float y = event.getY(event.getActionIndex());

        if (event.getActionMasked() == MotionEvent.ACTION_DOWN || event.getActionMasked() == MotionEvent.ACTION_POINTER_DOWN)
        {
            NativeWrapper.HandleTouchBegin(pID, x, y);
        }
        else if (event.getActionMasked() == MotionEvent.ACTION_UP || event.getActionMasked() == MotionEvent.ACTION_POINTER_UP)
        {
            NativeWrapper.HandleTouchEnd(pID, x, y);
        }
        else
        if (event.getActionMasked() == MotionEvent.ACTION_MOVE)
        {
            int tcount = 0;

            for (int i = 0; i < event.getPointerCount() && i < 5; i++)
            {
                idArr[i] = event.getPointerId(i);
                xArr[i] = event.getX(i);
                yArr[i] = event.getY(i);

                tcount++;
            }

            NativeWrapper.HandleTouchMove(idArr, xArr, yArr, tcount);
        }

        return true;
    }
}