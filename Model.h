#pragma once

#include "stdafx.h"
#include "ShaderProgram.h"
#include "AlcInfo.h"


using namespace std;
using namespace glm;

class Model {
public:
	Model(ShaderProgram *shaderProgram, string name, vec3 position);
	~Model(void);

	static unsigned char baseColor[3];

	string name;
	unsigned char color[3];		// unikalny kolor do pickingu

	mat4 modelMatrix;

	vec3 position;
	float angle;

	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec2> uvs;

	int verticesCount;

	GLuint texture;
	GLuint vao;
	GLuint verticesBuffer;
	GLuint uvsBuffer;
	GLuint normalsBuffer;


	void draw(ShaderProgram *shaderProgram);
	bool loadOBJ(const char *path, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals);
	bool loadBMP(const char *imagepath);
	bool loadTexture(const char *path);
	void setupVAO(ShaderProgram *shaderProgram);
	void setupVBO(void);
};

