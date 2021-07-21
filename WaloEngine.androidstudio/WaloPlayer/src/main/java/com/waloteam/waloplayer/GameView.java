package com.waloteam.waloplayer;

import android.content.Context;
import android.util.AttributeSet;
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
        initView(false, 0, 0);
    }

    public GameView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        initView(false, 0, 0);
    }

    private void initView(boolean translucent, int depth, int stencil)
    {
        if (translucent)
        {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }

        this.setEGLContextClientVersion(2);
        this.setFocusableInTouchMode(true);

        setEGLConfigChooser(translucent ? new ConfigChooser(8,8,8,8, depth, stencil, 1) : new ConfigChooser(5,6,5,0, depth, stencil, 1));

        this.mRenderer = new Renderer();
        setRenderer(this.mRenderer);

        setPreserveEGLContextOnPause(true);
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
        super.onPause();
    }

    @Override
    protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        if(!this.isInEditMode()) {
            this.mRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
        }
    }

    private class ConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        private int[] mConfigAttributes;
        private  final int EGL_OPENGL_ES2_BIT = 0x04;
        private  final int EGL_OPENGL_ES3_BIT = 0x40;
        public ConfigChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize, int multisamplingCount)
        {
            mConfigAttributes = new int[] {redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize, multisamplingCount};
        }
        public ConfigChooser(int[] attributes)
        {
            mConfigAttributes = attributes;
        }

        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            int[][] EGLAttributes = {
                    {
                            // GL ES 2 with user set
                            EGL10.EGL_RED_SIZE, mConfigAttributes[0],
                            EGL10.EGL_GREEN_SIZE, mConfigAttributes[1],
                            EGL10.EGL_BLUE_SIZE, mConfigAttributes[2],
                            EGL10.EGL_ALPHA_SIZE, mConfigAttributes[3],
                            EGL10.EGL_DEPTH_SIZE, mConfigAttributes[4],
                            EGL10.EGL_STENCIL_SIZE, mConfigAttributes[5],
                            EGL10.EGL_SAMPLE_BUFFERS, (mConfigAttributes[6] > 0) ? 1 : 0,
                            EGL10.EGL_SAMPLES, mConfigAttributes[6],
                            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL10.EGL_NONE
                    },
                    {
                            // GL ES 2 with user set 16 bit depth buffer
                            EGL10.EGL_RED_SIZE, mConfigAttributes[0],
                            EGL10.EGL_GREEN_SIZE, mConfigAttributes[1],
                            EGL10.EGL_BLUE_SIZE, mConfigAttributes[2],
                            EGL10.EGL_ALPHA_SIZE, mConfigAttributes[3],
                            EGL10.EGL_DEPTH_SIZE, mConfigAttributes[4] >= 24 ? 16 : mConfigAttributes[4],
                            EGL10.EGL_STENCIL_SIZE, mConfigAttributes[5],
                            EGL10.EGL_SAMPLE_BUFFERS, (mConfigAttributes[6] > 0) ? 1 : 0,
                            EGL10.EGL_SAMPLES, mConfigAttributes[6],
                            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL10.EGL_NONE
                    },
                    {
                            // GL ES 2 with user set 16 bit depth buffer without multisampling
                            EGL10.EGL_RED_SIZE, mConfigAttributes[0],
                            EGL10.EGL_GREEN_SIZE, mConfigAttributes[1],
                            EGL10.EGL_BLUE_SIZE, mConfigAttributes[2],
                            EGL10.EGL_ALPHA_SIZE, mConfigAttributes[3],
                            EGL10.EGL_DEPTH_SIZE, mConfigAttributes[4] >= 24 ? 16 : mConfigAttributes[4],
                            EGL10.EGL_STENCIL_SIZE, mConfigAttributes[5],
                            EGL10.EGL_SAMPLE_BUFFERS, 0,
                            EGL10.EGL_SAMPLES, 0,
                            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL10.EGL_NONE
                    },
                    {
                            // GL ES 2 by default
                            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL10.EGL_NONE
                    }
            };

            EGLConfig result = null;
            for (int[] eglAtribute : EGLAttributes) {
                result = this.doChooseConfig(egl, display, eglAtribute);
                if (result != null)
                    return result;
            }

            return null;
        }

        private EGLConfig doChooseConfig(EGL10 egl, EGLDisplay display, int[] attributes) {
            EGLConfig[] configs = new EGLConfig[1];
            int[] matchedConfigNum = new int[1];
            boolean result = egl.eglChooseConfig(display, attributes, configs, 1, matchedConfigNum);
            if (result && matchedConfigNum[0] > 0) {
                return configs[0];
            }
            return null;
        }
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
