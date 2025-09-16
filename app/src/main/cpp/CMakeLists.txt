cmake_minimum_required(VERSION 3.10)
project(rocket_game)

# فایل‌های سورس
set(SRC_FILES main.cpp)

# اضافه کردن مسیرهای include اولیه
include_directories(
    ${CMAKE_SOURCE_DIR}/SDL2
    ${CMAKE_SOURCE_DIR}/SDL2_src/include
)

# کامپایل SDL2 از سورس
if(EXISTS "${CMAKE_SOURCE_DIR}/SDL2_src/CMakeLists.txt")
    message(STATUS "Compiling SDL2 from source")
    add_subdirectory(SDL2_src)
    
    # بررسی هدف‌های SDL2
    if(TARGET SDL2)
        set(SDL2_LIBS SDL2)
    elseif(TARGET SDL2-static)
        set(SDL2_LIBS SDL2-static)
    else()
        message(FATAL_ERROR "SDL2 target not found")
    endif()
else()
    message(FATAL_ERROR "SDL2_src CMakeLists.txt not found")
endif()

# ساخت کتابخانه native
add_library(rocket_game SHARED ${SRC_FILES})

# کتابخانه‌های سیستمی
find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)
find_library(egl-lib EGL)
find_library(m-lib m)

# مسیر include نهایی
target_include_directories(rocket_game PRIVATE
    ${CMAKE_SOURCE_DIR}/SDL2
    ${CMAKE_SOURCE_DIR}/SDL2_src/include
)

# لینک کردن
target_link_libraries(rocket_game
    ${log-lib}
    ${android-lib}
    ${glesv2-lib}
    ${egl-lib}
    ${m-lib}
    ${SDL2_LIBS}
)
