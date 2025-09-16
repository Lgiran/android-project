LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_SRC_FILES := ../src/main/cpp/main.cpp
LOCAL_CPPFLAGS := -std=c++17
LOCAL_LDLIBS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
