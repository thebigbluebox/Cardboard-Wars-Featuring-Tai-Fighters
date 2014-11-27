# Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := example
LOCAL_CFLAGS := -Werror
LOCAL_LDLIBS := -lGLESv1_CM
LOCAL_SRC_FILES := \
	example.cpp \
	controller.cpp

include $(BUILD_SHARED_LIBRARY)
