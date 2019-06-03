#include "Device/Android/DeviceAndroid.hpp"
#include "WaloPlayer.hpp"

#include <jni.h>
#include <sys/stat.h>
#include <errno.h>

CPlayer* pGame = 0;

extern "C"
{
	JNIEXPORT jboolean JNICALL Java_com_waloteam_waloplayer_NativeWrapper_Create(JNIEnv* _env, jobject _caller, jobject _assetMgr, jstring _packageName, jstring _savePath)
	{
		const char* packagePathBytes = _env->GetStringUTFChars(_packageName, 0);
		const char* savePathBytes = _env->GetStringUTFChars(_savePath, 0);

        int result_code = mkdir(savePathBytes, 0770);

        int err = errno;

        CDeviceAndroid::InitFromJava(_env, _assetMgr, packagePathBytes, savePathBytes);

        if(!CPlayer::RunScript("main.nut"))
        {
            return -1;
        }

        pGame = (CPlayer*)CPlayer::Instance();

        if(!pGame)
        {
            LOG("WaloPlayer: Game instance not created in script.\n");
            return -1;
        }

        pGame->Configure();

		return true;
	}

	JNIEXPORT jboolean JNICALL Java_com_waloteam_waloplayer_NativeWrapper_InitRender(JNIEnv* _env, jobject _caller, jint _width, jint _height)
    {
        pGame->GetDevice()->SetScreenSize(CVector2(_width, _height));

        if(!pGame->Init())
        {
            LOG("WaloPlayer: Game initialization failed.\n");
            return -1;
        }

		return true;
	}

    JNIEXPORT jboolean JNICALL Java_com_waloteam_waloplayer_NativeWrapper_HandleTouchBegin(JNIEnv* _env, jobject _caller, int _id, jfloat _x, jfloat _y)
    {
        CTouchEvent event;

        event.TouchCount = 1;
        event.Touches[0].TouchType = CTouchEvent::ETouchEventType::ETE_BEGIN;
        event.Touches[0].ID = _id;
        event.Touches[0].X = _x;
        event.Touches[0].Y = _y;

        pGame->GetSystem<CInputSystem>()->PushInput(&event);

        return true;
    }

    JNIEXPORT jboolean JNICALL Java_com_waloteam_waloplayer_NativeWrapper_HandleTouchEnd(JNIEnv* _env, jobject _caller, int _id, jfloat _x, jfloat _y)
    {
        CTouchEvent event;

        event.TouchCount = 1;
        event.Touches[0].TouchType = CTouchEvent::ETouchEventType::ETE_END;
        event.Touches[0].ID = _id;
        event.Touches[0].X = _x;
        event.Touches[0].Y = _y;

        pGame->GetSystem<CInputSystem>()->PushInput(&event);

        return true;
    }

    JNIEXPORT jboolean JNICALL Java_com_waloteam_waloplayer_NativeWrapper_HandleTouchMove(JNIEnv* _env, jobject _caller, jintArray _idarr, jfloatArray _xarr, jfloatArray _yarr, int _count)
    {
        CTouchEvent event;

        event.TouchCount = _count;

        for(int i=0; i< _count; i++)
        {
            jint* idArr = _env->GetIntArrayElements(_idarr, 0);
            jfloat* xArr = _env->GetFloatArrayElements(_xarr, 0);
            jfloat* yArr = _env->GetFloatArrayElements(_yarr, 0);

            event.Touches[i].TouchType = CTouchEvent::ETouchEventType::ETE_MOVE;
            event.Touches[i].ID = idArr[i];
            event.Touches[i].X = xArr[i];
            event.Touches[i].Y = yArr[i];

            _env->ReleaseIntArrayElements(_idarr, idArr, 0);
            _env->ReleaseFloatArrayElements(_xarr, xArr, 0);
            _env->ReleaseFloatArrayElements(_yarr, yArr, 0);
        }

        pGame->GetSystem<CInputSystem>()->PushInput(&event);

        return true;
    }

	JNIEXPORT void JNICALL Java_com_waloteam_waloplayer_NativeWrapper_Update()
	{
        pGame->Run(0.04f);
	}

	JNIEXPORT void JNICALL Java_com_waloteam_waloplayer_NativeWrapper_Pause()
	{
        pGame->Activate(false);
	}

	JNIEXPORT void JNICALL Java_com_waloteam_waloplayer_NativeWrapper_Resume()
	{
        pGame->Activate(true);
	}
}

