#include "StdAfx.h"
#include "Observer.h"

using namespace std;
using namespace glm;


Observer::Observer(void) {
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 57.0f;

	center.x = 0.0f;
	center.y = 0.0f;
	center.z = 0.0f;

	nose.x = 0.0f;
	nose.y = 1.0f;
	nose.z = 0.0f;

	mouse.first = 300;
	mouse.second = 300;

	mouseStep.first = 0.0f;
	mouseStep.second = 0.0f;

	fieldOfView = 50.0f;
}


Observer::~Observer(void)
{
}

void Observer::set(ShaderProgram *shaderProgram) {
	mouseMove();
	
	viewMatrix = lookAt(vec3(position.x, position.y, position.z),		// wylicz macierz widoku
						vec3(center.x, center.y, center.z),
						vec3(nose.x, nose.y, nose.z)); 

	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, value_ptr(viewMatrix));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, value_ptr(projectionMatrix));
}

void Observer::setupProjection(float angle, int width, int height) {
	projectionMatrix = perspective(angle, (float) width / (float) height, 0.01f, 200.0f);
}

void Observer::move(unsigned char c) {
	const float step = 0.1;

	switch (c) {
	case 'a':	// lewo
	case 'A':
		position.x += step;
		break;

	case 'd':	// prawo
	case 'D':
		position.x -= step;
		break;

	case 's':	// ty³
	case 'S':
		position.z += step;
		break;

	case 'w':	// przód
	case 'W':
		position.z -= step;
		break;
	}
}

void Observer::mouseMove(void) {
	center.x += mouseStep.first;
	center.y += mouseStep.second;
}
