#pragma once

// Owns all OpenGL state and issues draw calls each frame.
// Called from the main loop after EGL is set up.
class Renderer {
public:
    Renderer()  = default;
    ~Renderer() = default;

    // Call once after EGL surface is created / resized
    void init(int width, int height);

    // Call every frame — issues GL draw calls, does NOT swap buffers
    void draw();

private:
    int width_  = 0;
    int height_ = 0;
};
