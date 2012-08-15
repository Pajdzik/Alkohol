#pragma once

#include "AlcInfo.h"
#include "Bottle.h"
#include "Model.h"
#include "Observer.h"
#include "ShaderProgram.h"


const int bottleCount = 18;

int windowPositionX = 100,
	windowPositionY = 100,
	windowHeight = 900,
	windowWidth = 900;

float speed = 120.0f; //120 stopni/s
int lastTime = 0.0f;

Bottle *bottles[bottleCount];
Model	*barrel,
		*chair[4],
		*counter,
		*desk,
		*ground,
		*table[2],
		*room;
Observer observer;
ShaderProgram *shaderProgram;

enum Brands {
	absolut,
	baileys,
	becherovka,
	beefeater,
	chivasregal,
	cointreau,
	curacao,
	havanaclub,
	jackdaniels,
	jagermeister,
	jameson,
	kahlua,
	malibu,
	martell,
	molinari,
	olmeca,
	pernod,
	ramazzotti
};
