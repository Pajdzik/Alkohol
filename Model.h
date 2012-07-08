#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"


using namespace std;
using namespace glm;

class Model {
public:
	Model(ShaderProgram *shaderProgram, string name, vec3 position);
	~Model(void);

	vec3 position;

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

	// uchwyty na VAO i bufory wierzcho�k�w
	GLuint	vao;
	GLuint	verticesBuffer;			// uchwyt na bufor VBO przechowuj�cy tablic� wsp�rz�dnych wierzcho�k�w
	GLuint	uvsBuffer;				// uchwyt na bufor VBO przechowuj�cy tablic� kolor�w
	GLuint	normalsBuffer;			// uchwyt na bufor VBO przechowuj�cy tablic� wektor�w normalnych

	float angle;

	mat4 modelMatrix;
};

