#include "renderer.h"

#include <GLES2/gl2.h>
#include <android/log.h>

#define LOG_TAG "androdash/renderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

// Dark navy — #0D1117 (GitHub dark background, easy on eyes)
static constexpr float kClearR = 0.051f;
static constexpr float kClearG = 0.067f;
static constexpr float kClearB = 0.090f;

void Renderer::init(int width, int height) {
    width_  = width;
    height_ = height;

    glViewport(0, 0, width_, height_);
    glClearColor(kClearR, kClearG, kClearB, 1.0f);

    LOGI("Renderer initialised %dx%d", width_, height_);
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Future: draw quads, text, icons here using shaders from shader.h
}
