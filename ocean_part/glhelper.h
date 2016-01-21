#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "misc.h"


void setupTexture(GLuint& texture);

void setupTextureFloat(GLuint& texture, int w, int h, const float texturef[]);
void deleteTexture(GLuint& texture);

void setupCubeMap(GLuint& texture);

void deleteCubeMap(GLuint& texture);

void createProgram(GLuint& glProgram, GLuint& glShaderV, GLuint& glShaderF, const char* vertex_shader, const char* fragment_shader);
void releaseProgram(GLuint& glProgram, GLuint glShaderV, GLuint glShaderF);

void saveTGA(unsigned char* buffer, int width, int height, bool video = false);

void glerror(const char* prepend);

