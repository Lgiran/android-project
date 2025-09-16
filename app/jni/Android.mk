LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := rocket_game_native
LOCAL_SRC_FILES := ../src/main/cpp/main.cpp
LOCAL_CPPFLAGS := -std=c++17 -fexceptions -I$(LOCAL_PATH)/../src/main/cpp/SDL2/include
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lm
include $(BUILD_SHARED_LIBRARY)
