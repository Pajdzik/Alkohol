#pragma once
#include "model.h"
class Bottle :	public Model {
public:
	Bottle(ShaderProgram *shaderProgram, AlcInfo *alcohol, vec3 position, float angle);
	~Bottle(void);

	AlcInfo info;
};

