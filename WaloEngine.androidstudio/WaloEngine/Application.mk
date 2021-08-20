APP_ABI := arm64-v8a armeabi-v7a
APP_STL := c++_static
APP_CPPFLAGS := -std=c++11
APP_MODULES := waloengine


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif