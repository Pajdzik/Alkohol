#pragma once

#include "StdAfx.h"
#include "AlcInfo.h"

AlcInfo::AlcInfo(void) {
	
}

AlcInfo::AlcInfo(string name, string sort, int voltage) {
	this->name = name;
	this->sort = sort;
	this->voltage = voltage;
}


AlcInfo::~AlcInfo(void)
{
}
