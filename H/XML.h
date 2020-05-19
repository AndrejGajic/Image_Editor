#pragma once
#include "Formatter.h"
#include "CompositeOperation.h"


class XMLFormatter : public Formatter {
private:
	CompositeOperation* o = nullptr;
public:
	XMLFormatter() {}
	XMLFormatter(CompositeOperation* c) {
		o = c;
	}
	void parse(ifstream* input) {}
	void write(string outName, Layer L) {}
	void remakeHeader(int, int) {}

	void exportOperation(string outName);
	CompositeOperation* importOperation(string inName);
	Operation* makeOperation(string name);

	void exportProject(string outName, Layer& image, CompositeOperation* comp);
	Layer importProject(string inName);
};