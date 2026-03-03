#pragma once

#include <GLES2/gl2.h>
#include <string_view>

// Compiles and links a vertex + fragment shader pair into a GL program.
// On failure, id() returns 0.
class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram();

    // Non-copyable, movable
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    // Compile and link from GLSL source strings
    bool build(std::string_view vert_src, std::string_view frag_src);

    // Activate this program for subsequent draw calls
    void use() const;

    GLuint id() const { return program_; }

    // Uniform helpers
    void set_int  (const char* name, int value)         const;
    void set_float(const char* name, float value)       const;
    void set_vec2 (const char* name, float x, float y)  const;
    void set_vec4 (const char* name, float x, float y,
                                     float z, float w)  const;

private:
    GLuint program_ = 0;

    static GLuint compile_shader(GLenum type, std::string_view src);
};
