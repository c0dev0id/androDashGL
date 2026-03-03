cmake_minimum_required(VERSION 3.22.1)
project(androdash LANGUAGES C CXX)

# android_native_app_glue ships with every NDK installation
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue)

# Suppress missing-field-initializers warnings from the glue code
target_compile_options(native_app_glue PRIVATE -Wno-missing-field-initializers)

add_library(androdash SHARED
    main.cpp
    egl_context.cpp
    renderer.cpp
    shader.cpp)

target_include_directories(androdash PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
    ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(androdash PRIVATE -std=c++17 -Wall -Wextra)

target_link_libraries(androdash
    native_app_glue
    android          # ANativeWindow, ALooper, AAssetManager
    EGL              # EGL context / surface
    GLESv2           # OpenGL ES 2.0
    log)             # __android_log_print
