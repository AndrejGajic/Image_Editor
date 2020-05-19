#include "BasicOperation.h"
#include "Layer.h"

void Mediana::setLayer(Layer& l1) {
	l = &l1;
}

Pixel& Mediana::execute(Pixel& p, const int r1, const int g1, const int b1) {
	l->median();
	return (*l)[0][0];
}