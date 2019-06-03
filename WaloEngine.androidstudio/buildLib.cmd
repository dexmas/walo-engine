pushd WaloEngine
call %ANDROID_NDK_PATH%\ndk-build.cmd NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./Android.mk  NDK_APPLICATION_MK=./Application.mk %1
popd
pause