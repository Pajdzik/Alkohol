#pragma once
#include "model.h"
class Bottle :	public Model {
public:
	Bottle(ShaderProgram *shaderProgram, AlcInfo *alcohol, vec3 position);
	~Bottle(void);

	AlcInfo info;
};

