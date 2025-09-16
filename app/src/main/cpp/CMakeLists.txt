cmake_minimum_required(VERSION 3.6)
project(rocket_game)

# فایل‌های سورس
set(SRC_FILES main.cpp)

# اگر فولدر SDL2 دارای CMakeLists است، آن را بعنوان زیرپروژه اضافه کن
if(EXISTS "${CMAKE_SOURCE_DIR}/SDL2/CMakeLists.txt")
  message(STATUS "Using SDL2 subdirectory (SDL2/CMakeLists.txt found)")
  add_subdirectory(SDL2)
  set(SDL2_LIBS SDL2)
elseif(EXISTS "${CMAKE_SOURCE_DIR}/SDL2_src/CMakeLists.txt")
  message(STATUS "Using SDL2_src subdirectory")
  add_subdirectory(SDL2_src)
  set(SDL2_LIBS SDL2)
else()
  # اگر فقط هدرها موجود است، مسیر include را اضافه کن
  message(STATUS "SDL2 CMakeLists not found — add include dir if headers exist")
  include_directories(${CMAKE_SOURCE_DIR}/SDL2/include)
endif()

# ساخت کتابخانه native اپ
add_library(rocket_game SHARED ${SRC_FILES})

# کتابخانه‌های سیستمی اندروید
find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)
find_library(egl-lib EGL)
find_library(m-lib m)

# لینک کردن
target_link_libraries(rocket_game
    ${log-lib}
    ${android-lib}
    ${glesv2-lib}
    ${egl-lib}
    ${m-lib}
    ${SDL2_LIBS}
)
