package com.waloteam.waloplayer;

import android.content.res.AssetManager;

public class NativeWrapper
{
    static
    {
        System.loadLibrary("waloplayer");
    }

    public static native boolean Create(AssetManager _assetMgr, String _packagePath, String _savePath);
    public static native boolean InitRender(int _width, int _height);

    public static native void HandleTouchBegin(int _id, float _x, float _y);
    public static native void HandleTouchEnd(int _id, float _x, float _y);
    public static native void HandleTouchMove(int[] _idarr, float[] _xarr, float[] _yarr, int _count);

    public static native void Update(float _dt);
    public static native void Pause();
    public static native void Resume();
}
