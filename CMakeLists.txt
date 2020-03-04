cmake_minimum_required(VERSION 3.9)
project(cqcppsdk-demo) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include(extern/cqcppsdk/cqcppsdk.cmake OPTIONAL RESULT_VARIABLE CQCPPSDK_PATH) # 包含 SDK 的 CMake 脚本, 必须
if(CQCPPSDK_PATH MATCHES "NOTFOUND") # 找不到cqcppsdk时自动从github获取
    find_package(Git REQUIRED) # 查找Git
    message(STATUS "Fetching cqcppsdk from https://github.com/cqmoe/cqcppsdk")
    execute_process( # 执行一个子进程
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/cqmoe/cqcppsdk
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extern"
            RESULT_VARIABLE CQCPPSDK_CLONE_RESULT
            )
    if(CQCPPSDK_CLONE_RESULT)
        message(FATAL_ERROR "Fail to fetch cqcppsdk")
    endif()
    include(extern/cqcppsdk/cqcppsdk.cmake)
endif()

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)
cq_set_app_id(${APP_ID}) # 设置 app id, 必须

file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

if (CQ_CAN_BUILD_STD_MODE)
    cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
    cq_install_std_app()
endif ()

# 添加 dev 模式的可执行文件构建目标
cq_add_dev_app(${SOURCE_FILES})
