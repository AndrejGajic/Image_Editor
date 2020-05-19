#pragma once
#include "Pixel.h"
#include "Selection.h"
#include "BasicOperation.h"
#include "CompositeOperation.h"
#include <vector>
#include <string>

using namespace std;


class Layer {
private:
	string name;
	vector<vector<Pixel>> layer;
	vector<Selection> selections;
	bool slct = false;
	int width = 0;
	int height = 0;
	CompositeOperation* comp = nullptr;
public:
	vector<Selection>& getSelections() {
		return selections;
	}

	void setName(string s) {
		name = s;
	}
	string getName() const {
		return name;
	}
	vector<Pixel>& operator[](int index) {
		return layer[index];
	}
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int h) {
		height = h;
	}
	int getWidth() const {
		return width;
	}
	int getHeight() const {
		return height;
	}
	void resize(int newSize) {
		layer.resize(newSize);
	}
	size_t getSize() const {
		return layer.size();
	}
	void writeBMP(ofstream& outFile);
	void writePAM(ofstream& outFile);
	auto begin() {
		return layer.begin();
	}
	auto end() {
		return layer.end();
	}

	void formatHeight(int h);
	void formatWidth(int w);
	void setAlpha(double alpha);


	void execute(Operation* o, int r1 = 1, int g1 = 1, int b1 = 1);
	void printInfo();

	void addSelection(Selection s) {
		this->selections.push_back(s);
	}

	void addSelections(int width, int height);
	void findSelection(string s);
	void deleteSelection(string s);
	bool activateSelection(string s);
	bool hasSelections() const{
		return slct;
	}
	void setSlct(bool x) {
		slct = x;
	}

	void median();
	int calcMediana(int x1, int x2, int x3, int x4 = -1, int x5 = -1);

	CompositeOperation* getCompositeOperation() {
		return this->comp;
	}
	void setCompositeOperation(CompositeOperation* o) {
		comp = o;
	}


};

