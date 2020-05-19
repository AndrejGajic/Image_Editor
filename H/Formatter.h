#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Layer.h"
using namespace std;

class Formatter {
protected:
	Layer myLayer;
public:
	Formatter() {}
	virtual void parse(ifstream* input) = 0;
	Layer getLayer() const {
		return myLayer;
	}
	virtual void write(string outName, Layer L) = 0;
	virtual void remakeHeader(int, int) = 0;
};