#include "StdAfx.h"
#include "Bottle.h"


Bottle::Bottle(ShaderProgram *shaderProgram, AlcInfo *alcohol, vec3 position, float angle) : Model(shaderProgram, alcohol->name, position, angle) {
	this->info = *alcohol;
}


Bottle::~Bottle(void) {
}
