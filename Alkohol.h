#include "Model.h"
#include "Observer.h"
#include "ShaderProgram.h"


int windowPositionX = 100,
	windowPositionY = 100,
	windowHeight = 900,
	windowWidth = 900;

float speed = 120.0f; //120 stopni/s
int lastTime = 0.0f;

Model *models[18];
Observer observer;
ShaderProgram *shaderProgram;
