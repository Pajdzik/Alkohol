#include "StdAfx.h"
#include "Bottle.h"


Bottle::Bottle(ShaderProgram *shaderProgram, AlcInfo *alcohol, vec3 position) : Model(shaderProgram, alcohol->name, position) {
	this->info = *alcohol;
}


Bottle::~Bottle(void) {
}
