#pragma once

#include "stdafx.h"
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

class Observer {
public:
	Observer(void);
	~Observer(void);

	vec3 position;
	vec3 center;
	vec3 nose;

	GLuint obsBuffer;
	GLuint vao;

	pair<int, int> mouse;
	float moveStep;
	float strafeStep;
	bool focused;

	mat4 viewMatrix;
	mat4 projectionMatrix;

	float fieldOfView;

	void set(ShaderProgram *shaderProgram);
	void setupProjection(float angle, int width, int height);
	void move(void);
	
private:
	const int windowHeight;
	const int windowWidth;

	void moveForward(float speed);
	void strafe(float speed);
	void mouseMove(void);
	void shift(float speed);
};

