cmake_minimum_required(VERSION 3.9)
project(cqcppsdk-demo) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改
include_directories(extern/cqcppsdk/include) # 设置 SDK 的 include 目录为包含目录, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须
file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)

cq_set_app_id(${APP_ID}) # 设置 app id, 必须

set(LIB_NAME "app") # 酷Q要求开发阶段的应用名为 app.dll

if (MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4)) # MSVC x86
    cq_add_app(${LIB_NAME} ${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
    cq_add_install_script(${LIB_NAME} "${PROJECT_SOURCE_DIR}/scripts/install.ps1") # 添加安装脚本(目前只支持 PowerShell 脚本, 建议脚本文件使用 UTF-16 LE 编码)
endif ()

# 添加 dev 模式的可执行文件构建目标
set(CQCPPSDK_DEV_MODE ON)
cq_add_app(${LIB_NAME}_dev ${SOURCE_FILES})

# 添加单元测试，如果无需测试，可删除
enable_testing()
add_subdirectory(extern/cqcppsdk/tests)
