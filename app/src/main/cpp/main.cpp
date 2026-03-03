#include <android_native_app_glue.h>
#include <android/log.h>

#include "egl_context.h"
#include "renderer.h"

#define LOG_TAG "androdash"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

struct AppState {
    EglContext* egl      = nullptr;
    Renderer*   renderer = nullptr;
};

static void handle_cmd(android_app* app, int32_t cmd) {
    auto* state = reinterpret_cast<AppState*>(app->userData);

    switch (cmd) {
    case APP_CMD_INIT_WINDOW:
        if (app->window != nullptr) {
            state->egl = new EglContext(app->window);
            if (!state->egl->valid()) {
                LOGE("Failed to create EGL context");
                delete state->egl;
                state->egl = nullptr;
                break;
            }
            state->renderer = new Renderer();
            state->renderer->init(state->egl->width(), state->egl->height());
            LOGI("Surface %dx%d, EGL ready", state->egl->width(), state->egl->height());
        }
        break;

    case APP_CMD_TERM_WINDOW:
        delete state->renderer;
        state->renderer = nullptr;
        delete state->egl;
        state->egl = nullptr;
        LOGI("Surface destroyed");
        break;

    case APP_CMD_GAINED_FOCUS:
        LOGI("Focus gained");
        break;

    case APP_CMD_LOST_FOCUS:
        LOGI("Focus lost");
        break;

    default:
        break;
    }
}

static int32_t handle_input(android_app* /*app*/, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        // Touch input — will be routed to renderer in a future step
        return 1;
    }
    return 0;
}

void android_main(android_app* app) {
    AppState state;
    app->userData    = &state;
    app->onAppCmd    = handle_cmd;
    app->onInputEvent = handle_input;

    // Main loop
    while (true) {
        int                  events;
        android_poll_source* source;

        // Block if no surface, poll immediately if we have one to render
        const int timeout = (state.egl != nullptr) ? 0 : -1;

        while (ALooper_pollAll(timeout, nullptr, &events,
                               reinterpret_cast<void**>(&source)) >= 0) {
            if (source != nullptr) {
                source->process(app, source);
            }
            if (app->destroyRequested) {
                LOGI("Destroy requested, exiting");
                return;
            }
        }

        if (state.egl != nullptr && state.renderer != nullptr) {
            state.renderer->draw();
            state.egl->swap();
        }
    }
}
