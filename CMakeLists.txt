cmake_minimum_required(VERSION 3.15)
project(epub_cleaner
    VERSION 1.0.0
    DESCRIPTION "EPUB Ad Cleaner - C++ EPUB ad content cleaning tool"
    HOMEPAGE_URL "https://github.com/yourusername/epub-ad-cleaner"
    LANGUAGES CXX
)

# 设置项目元数据
set(PROJECT_COMPANY "Li")
set(PROJECT_COPYRIGHT "Copyright © 2024 Jianyin Li. All rights reserved.")

# 提取版本组件
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ ${PROJECT_VERSION})
set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})
set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_2})
set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_3})

# 设置SOVERSION（用于共享库）
set(PROJECT_SOVERSION ${PROJECT_VERSION_MAJOR})

# 设置项目选项
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_ZLIB "Enable ZLIB compression support" ON)
option(ENABLE_ICONV "Enable Iconv encoding conversion support" ON)
option(ENABLE_SYSTEM_ZIP "Use system ZIP utilities instead of built-in" ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 现代CMake策略
cmake_policy(SET CMP0077 NEW)  # option()尊重普通变量
cmake_policy(SET CMP0091 NEW)  # MSVC运行时库选择

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译器警告设置
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /EHsc)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
    # 设置运行时库
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    add_compile_options(-Wno-unused-parameter -Wno-unused-variable -Wno-missing-field-initializers -Wno-unknown-pragmas)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-Wduplicated-cond -Wduplicated-branches -Wlogical-op)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wdocumentation -Wthread-safety)
    endif()
endif()

# 查找依赖库
if(ENABLE_ZLIB)
    # 首先尝试使用现代CMake方式查找ZLIB
    find_package(ZLIB QUIET)
    
    # 如果没找到，尝试使用备选方法
    if(NOT ZLIB_FOUND)
        message(STATUS "ZLIB not found via find_package, trying alternative methods...")
        
        # 方法1: 使用pkg-config（Linux/macOS）
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(PC_ZLIB QUIET zlib)
            if(PC_ZLIB_FOUND)
                set(ZLIB_INCLUDE_DIRS ${PC_ZLIB_INCLUDE_DIRS})
                set(ZLIB_LIBRARIES ${PC_ZLIB_LIBRARIES})
                set(ZLIB_VERSION ${PC_ZLIB_VERSION})
                set(ZLIB_FOUND TRUE)
                message(STATUS "Found ZLIB via pkg-config: ${ZLIB_VERSION}")
            endif()
        endif()
        
        # 方法2: 手动查找（Windows）
        if(NOT ZLIB_FOUND AND WIN32)
            # 在常见位置查找zlib
            find_path(ZLIB_INCLUDE_DIR zlib.h
                PATHS
                "$ENV{ProgramFiles}/zlib/include"
                "$ENV{ProgramFiles\(x86\)}/zlib/include"
                "C:/zlib/include"
                "C:/Program Files/zlib/include"
                "C:/Program Files (x86)/zlib/include"
            )
            
            find_library(ZLIB_LIBRARY_RELEASE zlibstatic zlib
                PATHS
                "$ENV{ProgramFiles}/zlib/lib"
                "$ENV{ProgramFiles\(x86\)}/zlib/lib"
                "C:/zlib/lib"
                "C:/Program Files/zlib/lib"
                "C:/Program Files (x86)/zlib/lib"
            )
            
            find_library(ZLIB_LIBRARY_DEBUG zlibstaticd zlibd
                PATHS
                "$ENV{ProgramFiles}/zlib/lib"
                "$ENV{ProgramFiles\(x86\)}/zlib/lib"
                "C:/zlib/lib"
                "C:/Program Files/zlib/lib"
                "C:/Program Files (x86)/zlib/lib"
            )
            
            if(ZLIB_INCLUDE_DIR AND (ZLIB_LIBRARY_RELEASE OR ZLIB_LIBRARY_DEBUG))
                set(ZLIB_FOUND TRUE)
                set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
                
                if(ZLIB_LIBRARY_RELEASE AND ZLIB_LIBRARY_DEBUG)
                    set(ZLIB_LIBRARIES optimized ${ZLIB_LIBRARY_RELEASE} debug ${ZLIB_LIBRARY_DEBUG})
                elseif(ZLIB_LIBRARY_RELEASE)
                    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY_RELEASE})
                elseif(ZLIB_LIBRARY_DEBUG)
                    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY_DEBUG})
                endif()
                
                # 尝试获取版本
                if(EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h")
                    file(STRINGS "${ZLIB_INCLUDE_DIR}/zlib.h" ZLIB_VERSION_LINE REGEX "^#define ZLIB_VERSION")
                    if(ZLIB_VERSION_LINE)
                        string(REGEX REPLACE "^#define ZLIB_VERSION \"([^\"]+)\".*$" "\\1" ZLIB_VERSION "${ZLIB_VERSION_LINE}")
                    endif()
                endif()
                
                message(STATUS "Found ZLIB manually: ${ZLIB_VERSION}")
            endif()
        endif()
        
        # 方法3: 使用vcpkg（如果可用）
        if(NOT ZLIB_FOUND AND DEFINED VCPKG_TARGET_TRIPLET)
            find_package(ZLIB CONFIG REQUIRED)
            if(ZLIB_FOUND)
                message(STATUS "Found ZLIB via vcpkg")
            endif()
        endif()
    endif()
    
    # 如果仍然没找到，根据REQUIRED设置决定是否报错
    if(NOT ZLIB_FOUND)
        message(STATUS "ZLIB not found. Some features may be disabled.")
        set(ZLIB_FOUND FALSE)
    else()
        message(STATUS "Found ZLIB: ${ZLIB_VERSION}")
        # 创建导入目标（如果find_package没创建）
        if(NOT TARGET ZLIB::ZLIB)
            add_library(ZLIB::ZLIB INTERFACE IMPORTED)
            target_include_directories(ZLIB::ZLIB INTERFACE ${ZLIB_INCLUDE_DIRS})
            target_link_libraries(ZLIB::ZLIB INTERFACE ${ZLIB_LIBRARIES})
        endif()
    endif()
else()
    message(STATUS "ZLIB support disabled")
    set(ZLIB_FOUND FALSE)
endif()

if(ENABLE_ICONV)
    find_package(Iconv)
    if(Iconv_FOUND)
        message(STATUS "Found Iconv")
    else()
        message(STATUS "Iconv not found, using built-in encoding conversion")
        set(Iconv_FOUND FALSE)
    endif()
else()
    message(STATUS "Iconv support disabled")
    set(Iconv_FOUND FALSE)
endif()

# 设置全局编译定义
# 使用条件语句而不是生成器表达式，因为生成器表达式不能正确处理FALSE变量
if(ENABLE_ZLIB AND ZLIB_FOUND)
    add_compile_definitions(HAVE_ZLIB)
else()
    add_compile_definitions(NO_ZLIB)
endif()

if(ENABLE_ICONV AND Iconv_FOUND)
    add_compile_definitions(HAVE_ICONV)
endif()

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# 创建库目标（如果未来需要）
add_library(epub_cleaner_lib STATIC
    src/epub_processor.cpp
    src/ad_patterns.cpp
    src/file_utils.cpp
    src/zip_utils_impl.cpp
    src/logger.cpp
    src/iconv_wrapper.cpp
)

# 添加zlib压缩功能（如果启用）
if(ENABLE_ZLIB AND ZLIB_FOUND)
    target_sources(epub_cleaner_lib PRIVATE src/zlib_utils.cpp)
endif()

# 设置库目标属性
target_include_directories(epub_cleaner_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 设置编译定义
target_compile_definitions(epub_cleaner_lib PRIVATE
    $<$<BOOL:${ENABLE_SYSTEM_ZIP}>:USE_SYSTEM_ZIP=1>
)

# 链接依赖库
if(ENABLE_ZLIB AND ZLIB_FOUND)
    target_link_libraries(epub_cleaner_lib PUBLIC ZLIB::ZLIB)
endif()

if(ENABLE_ICONV AND Iconv_FOUND)
    target_link_libraries(epub_cleaner_lib PUBLIC Iconv::Iconv)
endif()

# 为库目标设置别名（命名空间）
add_library(epub_cleaner::lib ALIAS epub_cleaner_lib)

# 导出目标（用于安装）
install(TARGETS epub_cleaner_lib EXPORT epub_cleaner-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Windows特定设置
if(WIN32)
    target_compile_definitions(epub_cleaner_lib PRIVATE _WIN32)
    target_link_libraries(epub_cleaner_lib PUBLIC shlwapi)
endif()

# 创建可执行文件
add_executable(epub_cleaner src/main.cpp)
target_link_libraries(epub_cleaner PRIVATE epub_cleaner_lib)

# 设置可执行文件属性
set_target_properties(epub_cleaner PROPERTIES
    OUTPUT_NAME "epub_cleaner"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    MACOSX_BUNDLE FALSE
)

# 添加版本信息
if(WIN32)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_BINARY_DIR}/version.rc
        @ONLY
    )
    target_sources(epub_cleaner PRIVATE ${CMAKE_BINARY_DIR}/version.rc)
endif()

# 安装配置
include(GNUInstallDirs)

# 安装可执行文件
install(TARGETS epub_cleaner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/epub_cleaner
    FILES_MATCHING PATTERN "*.h"
)

# 安装配置文件
install(FILES example_patterns.txt
    DESTINATION ${CMAKE_INSTALL_DATADIR}/epub_cleaner
)

# 生成配置文件
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/epub_cleaner-config.cmake.in
    ${CMAKE_BINARY_DIR}/epub_cleaner-config.cmake
    @ONLY
)

# 安装配置文件
install(FILES
    ${CMAKE_BINARY_DIR}/epub_cleaner-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/epub_cleaner
)

# 测试支持
if(BUILD_TESTS)
    enable_testing()
    
    # 查找测试文件
    file(GLOB TEST_SOURCES "test/*.cpp")
    
    if(TEST_SOURCES)
        add_executable(test_epub_cleaner ${TEST_SOURCES})
        target_link_libraries(test_epub_cleaner PRIVATE epub_cleaner_lib)
        
        # 添加测试
        add_test(NAME epub_cleaner_unit_tests
            COMMAND test_epub_cleaner
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        
        # 设置测试属性
        set_tests_properties(epub_cleaner_unit_tests PROPERTIES
            TIMEOUT 30
            LABELS "unit"
        )
    else()
        message(WARNING "No test sources found in test/ directory")
    endif()
endif()

# 文档生成
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
        set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
        set(DOXYGEN_PROJECT_NAME "EPUB Ad Cleaner")
        set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
        set(DOXYGEN_PROJECT_BRIEF "C++ EPUB ad content cleaning tool")
        
        doxygen_add_docs(docs
            ${DOXYGEN_INPUT}
            COMMENT "Generating API documentation with Doxygen"
        )
        
        # 安装文档
        install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
        )
    else()
        message(WARNING "Doxygen not found, documentation generation disabled")
    endif()
endif()

# 包管理支持
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/epub_cleaner-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_BINARY_DIR}/epub_cleaner-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/epub_cleaner
)

# 导出目标
export(EXPORT epub_cleaner-targets
    FILE ${CMAKE_BINARY_DIR}/epub_cleaner-targets.cmake
)

install(EXPORT epub_cleaner-targets
    FILE epub_cleaner-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/epub_cleaner
)

# 添加卸载目标
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    COMMENT "Uninstalling epub_cleaner"
)

