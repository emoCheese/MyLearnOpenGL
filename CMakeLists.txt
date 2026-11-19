cmake_minimum_required(VERSION 3.28.0)

# ---------------------------------------------------------------------------
# 依赖来源：能提供 glfw3 的 vcpkg 就优先用 vcpkg，否则用系统路径里的库。
#
# vcpkg 位置按顺序探测：环境变量 VCPKG_ROOT、VCPKG_INSTALLATION_ROOT
# （Visual Studio 会把它们指向自己自带的 vcpkg），最后是 D:/DevTool/vcpkg。
# 只有“已经装了 glfw3 的 vcpkg”才算数，否则 VS 自带的空 vcpkg 会顶掉真正的。
# CMAKE_TOOLCHAIN_FILE 必须在 project() 之前写进 cache 才生效。
# ---------------------------------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    foreach(_vcpkg_root IN ITEMS "$ENV{VCPKG_ROOT}" "$ENV{VCPKG_INSTALLATION_ROOT}" "D:/DevTool/vcpkg")
        file(GLOB _vcpkg_glfw "${_vcpkg_root}/installed/*/share/glfw3/glfw3Config.cmake")
        if(EXISTS "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake" AND _vcpkg_glfw)
            set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
                CACHE FILEPATH "vcpkg toolchain file")
            message(STATUS "[vcpkg] 使用 ${_vcpkg_root}")
            break()
        endif()
    endforeach()
endif()

project(LearnOpenGL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)

# ---------------------------------------------------------------------------
# glfw3：vcpkg 的 config 包优先，找不到再找系统安装（头文件 + 库文件）。
# 两种情况都统一成 glfw 目标，子目录只链接 glfw 即可。
# ---------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG QUIET)

if(TARGET glfw)
    message(STATUS "[glfw3] vcpkg: ${glfw3_DIR}")
else()
    find_path(GLFW3_INCLUDE_DIR NAMES GLFW/glfw3.h)
    find_library(GLFW3_LIBRARY NAMES glfw3 glfw glfw3dll)

    if(NOT GLFW3_INCLUDE_DIR OR NOT GLFW3_LIBRARY)
        message(FATAL_ERROR
            "[glfw3] vcpkg 里没有装 glfw3，系统路径里也没有找到。\n"
            "  vcpkg：执行 vcpkg install glfw3:x64-windows\n"
            "  系统：把 glfw 的头文件和库加入 CMAKE_PREFIX_PATH，"
            "或手动设置 GLFW3_INCLUDE_DIR / GLFW3_LIBRARY")
    endif()

    add_library(glfw INTERFACE IMPORTED)
    set_target_properties(glfw PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${GLFW3_LIBRARY}")
    message(STATUS "[glfw3] 系统路径: ${GLFW3_LIBRARY}")
endif()

add_subdirectory(modules)
add_subdirectory(Chapter)
