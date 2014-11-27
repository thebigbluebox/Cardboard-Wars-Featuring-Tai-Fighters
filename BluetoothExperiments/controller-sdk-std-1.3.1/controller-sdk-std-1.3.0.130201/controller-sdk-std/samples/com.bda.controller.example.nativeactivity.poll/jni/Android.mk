# Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := example
LOCAL_CFLAGS := -Werror
LOCAL_LDLIBS := -landroid -lEGL -lGLESv1_CM
LOCAL_SRC_FILES := \
	main.cpp \
	example.cpp \
	controller.cpp
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module, android/native_app_glue)
