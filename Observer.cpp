#pragma once

#include "StdAfx.h"
#include "Observer.h"


using namespace std;
using namespace glm;


Observer::Observer(void) : windowHeight(900), windowWidth(900) {
	position.x = 0.0f;
	position.y = 30.0f;
	position.z = 0.0f;

	center.x = 50.0f;
	center.y = 0.0f;
	center.z = 50.0f;

	nose.x = 0.0f;
	nose.y = 5.0f;
	nose.z = 0.0f;

	mouse.first = 300;
	mouse.second = 300;

	fieldOfView = 50.0f;

	focused = true;
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



	GLuint observerLocation	= shaderProgram->getAttribLocation("observer");	
	glVertexAttrib4f(observerLocation, position.x, position.y, position.z, 1.0);
}

void Observer::setupProjection(float angle, int width, int height) {
	projectionMatrix = perspective(angle, (float) width / (float) height, 0.2f, 3000.0f);
}

void Observer::move(void) {
	moveForward(moveStep);
	strafe(strafeStep);
	mouseMove();
}

void Observer::moveForward(float speed) {
	vec3 diff = center - position;
	float px = position.x + diff.x * speed,
		  pz = position.z + diff.z * speed;

	if ((px < 400) && (px > -430)) {
		position.x = px;
		center.x = center.x + diff.x * speed;
	}

	if ((pz < 250) && (pz > -300)) {
		position.z = pz;
		center.z = center.z + diff.z * speed;
	}
}

void Observer::strafe(float speed) {
	vec3 diff = cross(center - position, nose);
	float px = position.x + diff.x * speed,
		  pz = position.z + diff.z * speed;

	if ((px < 400) && (px > -430)) {
		position.x = px;
		center.x = center.x + diff.x * speed;
	}

	if ((pz < 250) && (pz > -300)) {
		position.z = pz;
		center.z = center.z + diff.z * speed;
	}
}

void Observer::mouseMove(void) {
	const unsigned char strip = 180;
	const float speed = 0.4f;

	if (focused == true) {
		int x = mouse.first;
		int y = mouse.second;

		if (x < strip) {							// lewa strona ekranu
			shift(-speed);
		} else if (x > (windowWidth - strip)) {		// prawa strona ekranu
			shift(speed);
		} 

		if (y < strip) {							// góra ekranu
			center.y += speed;
		} else if (y > (windowHeight - strip)) {	// dó³ ekrau
			center.y -= speed;
		}
	}
}

void Observer::shift(float speed) {
	speed /= 100;
	vec3 diff = center - position;

	center.x = (float) (position.x + cos(speed) * diff.x - sin(speed) * diff.z);
	center.z = (float) (position.z + sin(speed) * diff.x + cos(speed) * diff.z);
}