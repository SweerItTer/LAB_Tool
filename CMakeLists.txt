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

# 设置OpenCV库路径
set(OpenCV_DIR "D:/Program/OpenCV/opencv/build/x64/vc15/lib")

# 查找Qt5包
find_package(Qt5 COMPONENTS Core Quick Qml Multimedia REQUIRED)

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

# 添加MSVC编译器选项
if(MSVC)
    # 禁用4819警告（文件包含在当前代码页中无法表示的字符）
    add_compile_options(/wd4819)
    
    # 可选：同时设置UTF-8编码（推荐长期解决方案）
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()
