#pragma once
#include "Operation.h"
#include <vector>

struct colors {
	int red = 0;
	int green = 0;
	int blue = 0;
};



class CompositeOperation : public Operation {
public:
	vector<pair<Operation*, colors>> operations;
	CompositeOperation() {
		basic = false;
	}
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override;
	void addOperation(Operation*, const int r1 = 0, const int g1 = 0, const int b1 = 0);
	Operation* getLastOperation();
	Operation* getFirstOperation();
	int numOfOperation() const;
};