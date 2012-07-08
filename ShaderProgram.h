#pragma once

#include "stdafx.h"
#include "GL/glew.h"

using namespace std;
using namespace glm;

class ShaderProgram {
public:
	ShaderProgram(char* vertexShaderFile, char* geometryShaderFile, char* fragmentShaderFile);
	~ShaderProgram(void);
	void use();		// w��cza shader
	GLuint getUniformLocation(char* variableName);	// pobiera numer slotu zwi�zanego z dan� zmienn� jednorodn�
	GLuint getAttribLocation(char* variableName);	// pobiera numer slotu zwi�zanego z danym atrybutem

private:
	char* readFile(char* fileName);							// metoda wczytuj�ca plik tekstowy do tablicy znak�w
	GLuint loadShader(GLenum shaderType, char* fileName);	// metoda wczytuje i kompiluje shader, a nast�pnie zwraca jego uchwyt

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
};
