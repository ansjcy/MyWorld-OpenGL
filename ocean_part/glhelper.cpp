

#include "glhelper.h"
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\glext.h>
#include <GL\glut.h>
#include <stdlib.h>
#include<Windows.h>
#include <stdio.h>
#include <iostream>
//#include <sstream>
//#include <fstream>
#include "misc.h"

void setupTexture(GLuint& texture) {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



void setupTextureFloat(GLuint& texture, int w, int h, const float texturef[]) {
	setupTexture(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, texturef);
}

void deleteTexture(GLuint& texture) {
	glDeleteTextures(1, &texture);
}

void setupCubeMap(GLuint& texture) {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void deleteCubeMap(GLuint& texture) {
	glDeleteTextures(1, &texture);
}

void createProgram(GLuint& glProgram, GLuint& glShaderV, GLuint& glShaderF, const char* vertex_shader, const char* fragment_shader) 
{	
	GLenum err = glewInit();
	if (err != GLEW_OK)
		printf("heheheh"),  getchar(), exit(1); // or handle the error in a nicer way
	if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
		printf("heheheh1"),getchar(), exit(1); // or handle the error in a nicer way
	glShaderV = glCreateShader(GL_VERTEX_SHADER);
	glShaderF = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* vShaderSource = loadFile(vertex_shader);
	const GLchar* fShaderSource = loadFile(fragment_shader);
	glShaderSource(glShaderV, 1, &vShaderSource, NULL);
	glShaderSource(glShaderF, 1, &fShaderSource, NULL);
	delete[] vShaderSource;
	delete[] fShaderSource;
	glCompileShader(glShaderV);
	glCompileShader(glShaderF);
	glProgram = glCreateProgram();
	glAttachShader(glProgram, glShaderV);
	glAttachShader(glProgram, glShaderF);
	glLinkProgram(glProgram);
	glUseProgram(glProgram);

	int  vlength, flength, plength;
	char vlog[2048], flog[2048], plog[2048];
	glGetShaderInfoLog(glShaderV, 2048, &vlength, vlog);
	glGetShaderInfoLog(glShaderF, 2048, &flength, flog);
	glGetProgramInfoLog(glProgram, 2048, &flength, plog);
	std::cout << vlog << std::endl << std::endl << flog << std::endl << std::endl << plog << std::endl << std::endl;
}

void releaseProgram(GLuint& glProgram, GLuint glShaderV, GLuint glShaderF) {
	glDetachShader(glProgram, glShaderF);
	glDetachShader(glProgram, glShaderV);
	glDeleteShader(glShaderF);
	glDeleteShader(glShaderV);
	glDeleteProgram(glProgram);
}

void saveTGA(unsigned char* buffer, int width, int height, bool video) {
	static int i = 0;
	std::stringstream out;
	if (video) {
		if (i < 10)
			out << "video000" << (i++) << ".tga";
		else if (i < 100)
			out << "video00" << (i++) << ".tga";
		else if (i < 1000)
			out << "video0" << (i++) << ".tga";
		else if (i < 10000)
			out << "video" << (i++) << ".tga";
	}
	else {
		out << "capture" << (i++) << ".tga";
	}
	std::string s = out.str();

	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
	std::fstream of(s.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	char header[18] = { 0 };
	header[2] = 2;
	header[12] = width & 0xff;
	header[13] = width >> 8;
	header[14] = height & 0xff;
	header[15] = height >> 8;
	header[16] = 32;
	of.write(header, 18);
	of.write((char *)buffer, width * height * 4);
} 

void glerror(const char* prepend) {
	std::cout << prepend << " -- " << gluErrorString(glGetError()) << std::endl;
}