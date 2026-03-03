#pragma once

#include <EGL/egl.h>
#include <android/native_window.h>

// Owns the EGL display, surface and context for an ANativeWindow.
// Call valid() after construction to check for errors.
class EglContext {
public:
    explicit EglContext(ANativeWindow* window);
    ~EglContext();

    // Non-copyable
    EglContext(const EglContext&)            = delete;
    EglContext& operator=(const EglContext&) = delete;

    bool valid()  const { return valid_; }
    int  width()  const { return width_; }
    int  height() const { return height_; }

    // Present the current frame (eglSwapBuffers)
    void swap();

private:
    EGLDisplay display_ = EGL_NO_DISPLAY;
    EGLSurface surface_ = EGL_NO_SURFACE;
    EGLContext context_ = EGL_NO_CONTEXT;
    int        width_   = 0;
    int        height_  = 0;
    bool       valid_   = false;

    void destroy();
};
