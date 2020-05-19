#include "CompositeOperation.h"

Pixel& CompositeOperation::execute(Pixel& p, const int r1, const int g1, const int b1) {
	for (auto& par : operations) {
		par.first->execute(p, par.second.red, par.second.green, par.second.blue);
	}
	return p;
}

void CompositeOperation::addOperation(Operation* o, const int r1, const int g1, const int b1) {
	colors clr;
	clr.red = r1;
	clr.green = g1;
	clr.blue = b1;
	pair<Operation*, colors> par(o, clr);
	if (o != nullptr) operations.push_back(par);
}
/*
Operation* CompositeOperation::getFirstOperation() {
	if (operations.size() != 0) {
		Operation* o = operations.front().first;
		operations.erase(operations.begin());
		operations.pop_back(); // mozda greska!
		return o;
	}
	return nullptr;
}

Operation* CompositeOperation::getLastOperation() {
	if (operations.size() != 0) {
		Operation* o = operations.back().first;
		operations.pop_back();
		return o;
	}
	return nullptr;
}
*/
int CompositeOperation::numOfOperation() const {
	return operations.size();
}