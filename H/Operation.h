#pragma once
#include <iostream>
#include "Pixel.h"
using namespace std;


class Operation {
protected:
	bool basic; // 1 -> basic operation, 0 -> composite operation
	string name = "";
public:
	virtual Pixel& execute(Pixel& p, const int r1, const int g1, const int b1) = 0;
	bool mediana = false;
	string getName() const {
		return name;
	}
	void setName(string s) {
		name = s;
	}
};