cmake_minimum_required(VERSION 3.10)
project(epub_cleaner VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译器选项
if(MSVC)
    add_compile_options(/W4 /EHsc)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-parameter)
endif()

# 手动设置zlib路径（MinGW特定）
set(ZLIB_INCLUDE_DIR "D:/Developenv/Qt/Tools/mingw1310_64/x86_64-w64-mingw32/include")
set(ZLIB_LIBRARY "D:/Developenv/Qt/Tools/mingw1310_64/x86_64-w64-mingw32/lib/libz.a")

# 检查zlib是否存在
if(EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h" AND EXISTS "${ZLIB_LIBRARY}")
    set(ZLIB_FOUND TRUE)
    message(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
else()
    set(ZLIB_FOUND FALSE)
    message(WARNING "ZLIB not found at specified path")
endif()

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
    message(STATUS "Using ZLIB for compression")
    add_definitions(-DHAVE_ZLIB)
else()
    message(STATUS "ZLIB not available, some features may be limited")
    add_definitions(-DNO_ZLIB)
endif()

# 源文件
set(SOURCES
    src/main.cpp
    src/epub_processor.cpp
    src/ad_patterns.cpp
    src/file_utils.cpp
    src/zip_utils_impl.cpp  # 新的ZIP实现
    src/logger.cpp
)

# 如果zlib可用，添加zlib压缩功能
if(ZLIB_FOUND)
    list(APPEND SOURCES src/zlib_utils.cpp)
endif()

# 头文件
set(HEADERS
    include/epub_processor.h
    include/ad_patterns.h
    include/file_utils.h
    include/zip_utils.h
    include/logger.h
)

# 创建可执行文件
add_executable(epub_cleaner ${SOURCES} ${HEADERS})

# 链接库
set(LINK_LIBRARIES)

# 链接zlib（如果找到）
if(ZLIB_FOUND)
    list(APPEND LINK_LIBRARIES "${ZLIB_LIBRARY}")
endif()

# Windows特定设置
if(WIN32)
    target_compile_definitions(epub_cleaner PRIVATE _WIN32)
    list(APPEND LINK_LIBRARIES shlwapi)
endif()

# 链接所有库
target_link_libraries(epub_cleaner PRIVATE ${LINK_LIBRARIES})

# 设置目标属性
set_target_properties(epub_cleaner PROPERTIES
    OUTPUT_NAME "epub_cleaner"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 安装目标
install(TARGETS epub_cleaner
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include/epub_cleaner
    FILES_MATCHING PATTERN "*.h"
)

# 可选：添加测试
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_executable(test_epub_cleaner
        tests/test_main.cpp
        tests/test_file_utils.cpp
        tests/test_ad_patterns.cpp
    )
    target_link_libraries(test_epub_cleaner epub_cleaner)
    add_test(NAME test_epub_cleaner COMMAND test_epub_cleaner)
endif()

# 可选：添加文档生成
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS AND DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
    doxygen_add_docs(docs ${DOXYGEN_INPUT})
endif()

