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

	pair<int, int> mouse;
	pair<float, float> mouseStep;

	//float angle;

	mat4 viewMatrix;
	mat4 projectionMatrix;

	float fieldOfView;

	void set(ShaderProgram *shaderProgram);
	void setupProjection(float angle, int width, int height);
	void move(float speed);
	void strafe(float speed);
	
private:
	void mouseMove(void);
};

