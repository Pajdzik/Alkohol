#include "StdAfx.h"
#include "Observer.h"

using namespace std;
using namespace glm;


Observer::Observer(void) {
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 80.0f;

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

void Observer::move(float speed) {
	vec3 diff = center - position;

	position.x = position.x + diff.x * speed;
	position.z = position.z + diff.z * speed;

	center.x = center.x + diff.x * speed;
	center.z = center.z + diff.z * speed;
}

void Observer::strafe(float speed) {
	vec3 diff = center - position;

	center.x = (float) (position.x + cos(speed) * diff.x - sin(speed) * diff.z);
	center.z = (float) (position.z + sin(speed) * diff.x + cos(speed) * diff.z);
}

void Observer::mouseMove(void) {
	center.x += mouseStep.first;
	center.y += mouseStep.second;
}
