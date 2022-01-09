#include "Shader.h"
#include"../FileHelper/FileHelper.hpp"
#include <Windows.h>
#include <string>
void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success && ShaderDebug)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::string df = infoLog;
            df += "\n -- --------------------------------------------------- -- \n";
            LPWSTR filename = new wchar_t[df.length()];
            MultiByteToWideChar(0, 0, df.c_str(), df.length(), filename, df.length());
            MessageBox(nullptr, filename, L"ERROR::SHADER_COMPILATION_ERROR", MB_ICONERROR);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success && ShaderDebug)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::string df = infoLog;
            df  += "\n -- --------------------------------------------------- -- \n";
            LPWSTR filename = new wchar_t[df.length()];
            MultiByteToWideChar(0, 0, df.c_str(), df.length(), filename, df.length());
            MessageBox(nullptr, filename, L"ERROR::PROGRAM_LINKING_ERROR", MB_ICONERROR);
        }
    }
}

//shader//
void shader::init(const char* path, GLuint type) {
    this->type = type;
    std::string s = rdFile(path);
    const char* t = s.c_str();
#if ShaderDebug
        size_t gh = s.length();
        LPWSTR fff = new wchar_t[gh];
        MultiByteToWideChar(0, 0, t, gh, fff, gh);
        MessageBox(nullptr, fff, L"Shader code", MB_OK);
#endif
    id = glCreateShader(type);
    glShaderSource(id, 1, &t, NULL);
    glCompileShader(id);
    checkCompileErrors(id, "");
}
void shader::finit() {
    glDeleteShader(id);
}
//shader//

//program//
void program::setShaderFVG(shader shad, GLuint index) { fvg[index] = shad; }
void program::setShaderFVG(const char* path, GLuint index) { fvg[index].init(path, (index == 0)?GL_VERTEX_SHADER:(index == 1)?GL_FRAGMENT_SHADER:GL_GEOMETRY_SHADER); }
void program::create() {
    id = glCreateProgram();
    glAttachShader(id, fvg[0].id);
    glAttachShader(id, fvg[1].id);
    if (fvg[2].id) glAttachShader(id, fvg[2].id);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    fvg[0].finit();
    fvg[1].finit();
    if (fvg[2].id) fvg[2].finit();
}
void program::use() { glUseProgram(id); }
GLuint program::getUnigorm(const char* name) { return glGetUniformLocation(id, name); }
GLuint program::getAtribut(const char* name) { return glGetAttribLocation( id, name); }
//program//

//error//
void errorGLshader(const wchar_t* name, GLuint id) {
    char* error_log = new char[4096];
    glGetProgramInfoLog(id, 4096, NULL, error_log);
    if (error_log[0] != 0) {
        LPWSTR ww = new wchar_t[4096];
        MultiByteToWideChar(0, 0, error_log, 4096, ww, 4096);
        MessageBox(nullptr, ww, name, MB_OK);
    }
}
//error//

//pipeprog//
void pipeprog::create(GLuint type, GLuint typeBIT, const char* path, GLuint num) {
    std::string code = rdFile(path);
    const GLchar* t[] = { code.c_str() };
    progs[num] = glCreateShaderProgramv(type, sizeof(t) / sizeof(void*), t);
    errorGLshader(L"shader", progs[num]);
    glUseProgramStages(id, typeBIT, progs[num]);
}
void pipeprog::gen(GLuint shads) { glGenProgramPipelines(1, &id); progs = new GLuint[shads]; }
void pipeprog::bind() { glBindProgramPipeline(id); }
//pipeprog//