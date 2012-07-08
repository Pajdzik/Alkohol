#pragma once

#include "stdafx.h"
#include "GL/glew.h"

using namespace std;
using namespace glm;

class ShaderProgram {
public:
	ShaderProgram(char* vertexShaderFile, char* geometryShaderFile, char* fragmentShaderFile);
	~ShaderProgram(void);
	void use();		// w³¹cza shader
	GLuint getUniformLocation(char* variableName);	// pobiera numer slotu zwi¹zanego z dan¹ zmienn¹ jednorodn¹
	GLuint getAttribLocation(char* variableName);	// pobiera numer slotu zwi¹zanego z danym atrybutem

private:
	char* readFile(char* fileName);							// metoda wczytuj¹ca plik tekstowy do tablicy znaków
	GLuint loadShader(GLenum shaderType, char* fileName);	// metoda wczytuje i kompiluje shader, a nastêpnie zwraca jego uchwyt

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
};
