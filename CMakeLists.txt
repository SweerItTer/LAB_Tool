# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.5)

# 设置项目名称
project(lab_tool VERSION 0.1 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(/wd4819)

# 是否使用交叉编译工具链的开关
if (1)
    message(STATUS "Using cross-compilation toolchain.")

    # 使用工具链文件
    include(CMakeForceCompiler) # CMake 3.6 版本之前
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

    # 设置库路径
    link_directories(${CMAKE_SYSROOT}/usr/lib)
    
else()
    message(STATUS "Not using cross-compilation toolchain.")
    
endif()


# 查找Qt5包
find_package(Qt5 COMPONENTS Core Quick Qml Multimedia REQUIRED)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # 设置OpenCV库路径
  set(OpenCV_DIR "D:/Program/OpenCV/opencv/build/x64/vc15/lib")
endif()
# 查找OpenCV包
find_package(OpenCV REQUIRED)


file(GLOB_RECURSE ALL_SOURCES 
    "src/core/*.cpp"
    "src/gui/*.cpp"
    "src/gui/controllers/*.cpp"  # 显式包含控制器目录
)

file(GLOB_RECURSE ALL_HEADERS 
    "src/core/*.h"
    "src/gui/*.h"
    "src/gui/controllers/*.h"
)

# 添加可执行文件
set(PROJECT_SOURCES
  
  ${ALL_SOURCES}
  ${ALL_HEADERS}
  src/gui/qml.qrc
  src/main.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
)

# 添加可执行文件
add_executable(lab_tool
  ${PROJECT_SOURCES}
)

# 链接库
target_link_libraries(lab_tool
  Qt5::Quick
  Qt5::Core
  Qt5::Multimedia
  ${OpenCV_LIBS}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # 查找 PulseAudio 主循环库
    find_library(PULSE_MAINLOOP_LIB
        NAMES pulse-mainloop-glib
        PATHS /usr/lib /usr/local/lib
    )

    # # 查找 GLib 库（可选，确保 GLib 依赖）
    # find_package(PkgConfig REQUIRED)
    # pkg_check_modules(GLIB REQUIRED glib-2.0)

    if (PULSE_MAINLOOP_LIB)
        message(STATUS "找到 libpulse-mainloop-glib: ${PULSE_MAINLOOP_LIB}")
        target_link_libraries(lab_tool ${PULSE_MAINLOOP_LIB} ${GLIB_LIBRARIES})
    else()
        message(FATAL_ERROR "未找到 libpulse-mainloop-glib，请安装 libpulse-dev")
    endif()
endif()