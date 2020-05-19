#pragma once
#include <iostream>
#include <vector>
#include "Formatter.h"
#include "BMP.h"
#include "PAM.h"
#include "XML.h"
#include "BasicOperation.h"
#include "CompositeOperation.h"
#include "Selection.h"
using namespace std;

class Image {
private:
	Formatter* myBMPFormatter;
	Formatter* myPAMFormatter;
	XMLFormatter* myXMLFormatter;
	vector<Layer> layers;
	Layer image;
	int height;
	int width;
	int pom = 0;
	bool inComposite = false;
	bool hasSelections = false;
	CompositeOperation* comp = nullptr;
public:
	Image() {}
	void layersMenu();
	void importImage(); 
	void exportImage();
	void deleteLayer();
	void infoLayer();
	void merge(Layer& l);
	void mergeD(); // merge when deleted
	void operations();
	void advancedOperations(Operation*& o, int x, bool& flag);
	void executeOperation(Operation*& o, int, int, int);
	void allToBinary();
	void setPixelInt();
	void composite();
	void addSelections();
	void setCompositeOperation(CompositeOperation* o) {
		comp = o;
	}
};