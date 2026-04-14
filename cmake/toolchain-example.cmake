# 交叉编译工具链示例文件
# 用法: cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-example.cmake

# 设置系统信息
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 指定交叉编译器
set(CMAKE_C_COMPILER /usr/bin/x86_64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-linux-gnu-g++)

# 设置编译器标志
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=x86-64 -mtune=generic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64 -mtune=generic")

# 设置查找路径
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-gnu)

# 调整查找策略
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 设置安装前缀
set(CMAKE_INSTALL_PREFIX /usr/x86_64-linux-gnu)

# 设置依赖库路径
set(ZLIB_ROOT /usr/x86_64-linux-gnu)
set(Iconv_ROOT /usr/x86_64-linux-gnu)

# 设置编译选项
set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS OFF)
set(ENABLE_ZLIB ON)
set(ENABLE_ICONV ON)

# 输出配置信息
message(STATUS "Cross-compiling for ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")