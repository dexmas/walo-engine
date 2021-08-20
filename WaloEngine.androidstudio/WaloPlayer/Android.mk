
LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := src/main/cpp
PLAYER_PATH  := ../../Player
ENGINE_PATH  := ../../Engine
EXTERNS_PATH := ../../Externs

include $(CLEAR_VARS)

LOCAL_MODULE    := waloplayer

LOCAL_CFLAGS := -O0 -w -D_ANDROID -fpermissive -Wno-address-of-temporary

LOCAL_SRC_FILES := \
    $(JNI_SRC_PATH)/jni_interface.cpp \
	$(PLAYER_PATH)/WaloPlayer.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdaux.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdblob.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdio.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdmath.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdrex.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdstream.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdstring.cpp \
	$(EXTERNS_PATH)/squirrel3/sqstdlib/sqstdsystem.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqapi.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqbaselib.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqclass.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqcompiler.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqdebug.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqfuncstate.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqlexer.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqmem.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqobject.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqstate.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqtable.cpp \
	$(EXTERNS_PATH)/squirrel3/squirrel/sqvm.cpp \
	$(EXTERNS_PATH)/sqrat/sqratimport.cpp
	
LOCAL_C_INCLUDES :=	\
    $(PLAYER_PATH) \
    $(ENGINE_PATH) \
	$(EXTERNS_PATH)/squirrel3 \
	$(EXTERNS_PATH)/sqrat

LOCAL_LDLIBS := -llog -lGLESv2 -lOpenSLES -landroid

LOCAL_WHOLE_STATIC_LIBRARIES := waloengine

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ..)
$(call import-module, WaloEngine)