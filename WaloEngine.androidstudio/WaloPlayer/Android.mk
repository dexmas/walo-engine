abspath_wa = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))

LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := $(call abspath_wa, $(LOCAL_PATH)/src/main/cpp)
PLAYER_PATH := $(call abspath_wa, $(LOCAL_PATH)/../../Player)
ENGINE_PATH := $(call abspath_wa, $(LOCAL_PATH)/../../Engine)
EXTERNS_PATH := $(call abspath_wa, $(LOCAL_PATH)/../../Externs)
ENGINE_LIB_PATH := $(call abspath_wa, $(LOCAL_PATH)/../WaloEngine/obj/local)

include $(CLEAR_VARS)
LOCAL_MODULE := waloengine
LOCAL_SRC_FILES := $(ENGINE_LIB_PATH)/$(TARGET_ARCH_ABI)/libwaloengine.a
LOCAL_EXPORT_C_INCLUDES := $(ENGINE_PATH)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := waloplayer
LOCAL_CFLAGS    := -std=c++11 -O0  -Wno-address-of-temporary
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

LOCAL_CFLAGS += -D_ANDROID -D_DEBUG -fpermissive
LOCAL_STATIC_LIBRARIES := waloengine
LOCAL_LDLIBS := -llog -lGLESv2 -lOpenSLES -landroid

include $(BUILD_SHARED_LIBRARY)