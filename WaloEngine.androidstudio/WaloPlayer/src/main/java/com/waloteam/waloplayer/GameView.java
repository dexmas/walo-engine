package com.waloteam.waloplayer;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GameView extends GLSurfaceView
{
    private Renderer mRenderer;

    public GameView(Context context)
    {
        super(context);
        init(false, 0, 0);
    }

    public GameView(Context context, boolean translucent, int depth, int stencil)
    {
        super(context);
        init(translucent, depth, stencil);
    }

    private void init(boolean translucent, int depth, int stencil)
    {
        if (translucent)
        {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }

        setEGLContextFactory(new ContextFactory());

        setEGLConfigChooser( translucent ? new ConfigChooser(8,8,8,8, depth, stencil) : new ConfigChooser(5,6,5,0, depth, stencil) );

        this.mRenderer = new Renderer();
        setRenderer(this.mRenderer);
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory
    {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
        {
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };

            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);

            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
        {
            egl.eglDestroyContext(display, context);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        this.setRenderMode(RENDERMODE_CONTINUOUSLY);
        this.queueEvent(new Runnable() {
            @Override
            public void run() {
                GameView.this.mRenderer.handleOnResume();
            }
        });
    }

    @Override
    public void onPause() {
        this.queueEvent(new Runnable() {
            @Override
            public void run() {
                GameView.this.mRenderer.handleOnPause();
            }
        });
        this.setRenderMode(RENDERMODE_WHEN_DIRTY);
        //super.onPause();
    }

    @Override
    protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        if(!this.isInEditMode()) {
            this.mRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
        }
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
        {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
                EGL10.EGL_RED_SIZE, 4,
                EGL10.EGL_GREEN_SIZE, 4,
                EGL10.EGL_BLUE_SIZE, 4,
                EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
        {
            for (EGLConfig config : configs)
            {
                int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
        {
            if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
            {
                return mValue[0];
            }
            return defaultValue;
        }

        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        private final static long NANOSECONDSPERSECOND = 1000000000L;
        private final static long NANOSECONDSPERMICROSECOND = 1000000L;

        private static long sAnimationInterval = (long) (1.0 / 60 * Renderer.NANOSECONDSPERSECOND);

        private long mLastTickInNanoSeconds;

        private int mScreenWidth;
        private int mScreenHeight;

        private boolean mNativeInitCompleted = false;

        public void setScreenWidthAndHeight(final int surfaceWidth, final int surfaceHeight) {
            mScreenWidth = surfaceWidth;
            mScreenHeight = surfaceHeight;
        }

        public void onDrawFrame(GL10 gl)
        {
            if (sAnimationInterval <= 1.0 / 60 * Renderer.NANOSECONDSPERSECOND) {
                double dt = (double)sAnimationInterval / (double)Renderer.NANOSECONDSPERSECOND;
                NativeWrapper.Update((float)dt);
            } else {
                final long now = System.nanoTime();
                final long interval = now - mLastTickInNanoSeconds;

                if (interval < Renderer.sAnimationInterval) {
                    try {
                        long ms = (Renderer.sAnimationInterval - interval) / (long)Renderer.NANOSECONDSPERSECOND;
                        Thread.sleep(ms);
                    } catch (final Exception e) {
                    }
                }

                mLastTickInNanoSeconds = System.nanoTime();
                double dt = (double)interval / (double)Renderer.NANOSECONDSPERSECOND;
                NativeWrapper.Update((float)dt);
            }
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            int i = 0; //Do nothing
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
            NativeWrapper.InitRender(this.mScreenWidth, this.mScreenHeight);
            this.mLastTickInNanoSeconds = System.nanoTime();
            this.mNativeInitCompleted = true;
        }

        public void handleOnPause() {
            if (this.mNativeInitCompleted)
                NativeWrapper.Pause();
        }

        public void handleOnResume() {
            if (this.mNativeInitCompleted)
                NativeWrapper.Resume();
        }
    }
}
