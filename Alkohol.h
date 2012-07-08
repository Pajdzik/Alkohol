#include "Model.h"
#include "Observer.h"
#include "ShaderProgram.h"


int windowPositionX = 100,
	windowPositionY = 100,
	windowHeight = 600,
	windowWidth = 600;

float speed = 120.0f; //120 stopni/s
int lastTime = 0.0f;
//float angle = 0.0f;

Model *model;
Observer observer;
ShaderProgram *shaderProgram;
