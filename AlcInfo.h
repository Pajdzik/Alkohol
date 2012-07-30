#pragma once

using namespace std;

class AlcInfo {
public:
	AlcInfo(void);
	AlcInfo(string name, string sort, int voltage);
	~AlcInfo(void);

	string name;
	string sort;
	int voltage;
};

