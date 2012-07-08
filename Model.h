#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"


using namespace std;
using namespace glm;

class Model {
public:
	Model(ShaderProgram *shaderProgram, const char *modelPath, const char* texturePath);
	~Model(void);

	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec2> uvs;
	int verticesCount;

	GLuint Texture;

	void draw(ShaderProgram *shaderProgram);
	bool loadOBJ(const char *path, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals);
	bool loadBMP(const char *imagepath);
	void setupVAO(ShaderProgram *shaderProgram);
	void setupVBO(void);

	// uchwyty na VAO i bufory wierzcho³ków
	GLuint	vao;
	GLuint	verticesBuffer;	// uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
	GLuint	uvsBuffer;		// uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
	GLuint	normalsBuffer;		// uchwyt na bufor VBO przechowuj¹cy tablicê wektorów normalnych

	float angle;

	//"Model" który rysujemy
	/*float *vertices;
	float *colors;
	float *normals;*/
	int vertexCount;

	mat4 modelMatrix;
};

