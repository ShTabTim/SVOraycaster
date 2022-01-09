#pragma once
#include <glad/glad.h>
#include <string>
#define ShaderDebug false

void checkCompileErrors(unsigned int shader, std::string type);

typedef	struct shader {
	GLuint id;
	GLuint type;
	void init(const char* path, GLuint type);
	void finit();
}shader;


typedef struct program {
	GLuint id;
	shader fvg[3];
	void setShaderFVG(shader shad, GLuint index);
	void setShaderFVG(const char* path, GLuint index);
	void create();
	void use();
	GLuint getUnigorm(const char* name);
	GLuint getAtribut(const char* name);
}program;
void errorGLshader(const wchar_t* name, GLuint id);
typedef struct pipeprog {
	GLuint id;
	GLuint* progs;
	void create(GLuint type, GLuint typeBIT, const char* path, GLuint num);
	void gen(GLuint shads);
	void bind();
};