cmake_minimum_required(VERSION 3.10)
project(rocket_game)

# فایل‌های سورس اپلیکیشن
set(SRC_FILES main.cpp)

# مسیرهای include برای هدرهای SDL2
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL2
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_src/include
)

# اگر سورس SDL2 وجود داشت، اضافه کن
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_src/CMakeLists.txt")
    message(STATUS "Compiling SDL2 from source")
    add_subdirectory(SDL2_src)
endif()

# ساخت کتابخانه native اپ
add_library(rocket_game SHARED ${SRC_FILES})

# کتابخانه‌های سیستمی اندروید
find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)
find_library(egl-lib EGL)
find_library(m-lib m)

# لینک کردن کتابخانه‌ها
target_link_libraries(rocket_game
    ${log-lib}
    ${android-lib}
    ${glesv2-lib}
    ${egl-lib}
    ${m-lib}
)
