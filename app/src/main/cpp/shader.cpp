#include "shader.h"

#include <android/log.h>
#include <vector>

#define LOG_TAG "androdash/shader"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

ShaderProgram::~ShaderProgram() {
    if (program_) {
        glDeleteProgram(program_);
    }
}

bool ShaderProgram::build(std::string_view vert_src, std::string_view frag_src) {
    GLuint vert = compile_shader(GL_VERTEX_SHADER,   vert_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);

    if (!vert || !frag) {
        glDeleteShader(vert);
        glDeleteShader(frag);
        return false;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    // Shaders are linked into the program — they can be freed now
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint linked = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint len = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(static_cast<size_t>(len));
        glGetProgramInfoLog(prog, len, nullptr, log.data());
        LOGE("Link error: %s", log.data());
        glDeleteProgram(prog);
        return false;
    }

    if (program_) {
        glDeleteProgram(program_);
    }
    program_ = prog;
    return true;
}

void ShaderProgram::use() const {
    glUseProgram(program_);
}

void ShaderProgram::set_int(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(program_, name), value);
}

void ShaderProgram::set_float(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(program_, name), value);
}

void ShaderProgram::set_vec2(const char* name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program_, name), x, y);
}

void ShaderProgram::set_vec4(const char* name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program_, name), x, y, z, w);
}

GLuint ShaderProgram::compile_shader(GLenum type, std::string_view src) {
    GLuint shader = glCreateShader(type);
    const char* data = src.data();
    auto len = static_cast<GLint>(src.size());
    glShaderSource(shader, 1, &data, &len);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(static_cast<size_t>(logLen));
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        LOGE("Shader compile error (%s): %s",
             type == GL_VERTEX_SHADER ? "vert" : "frag",
             log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
