#pragma once
#include <string>
#include <vector>
using namespace std;

struct Rectangle {
	int x; // pozicija gornjeg levog ugla
	int y;
	int w; // sirina
	int h; // visina
	Rectangle(int i1, int i2, int i3, int i4) : x(i1), y(i2), w(i3), h(i4) {}
};




class Selection {
private:
	vector<Rectangle> selection;
	string name;
	bool active = false;
public:
	Selection(string name1, bool x) : name(name1), active(x) {}
	void addShape(int x1, int y1, int w1, int h1) {
		selection.push_back(Rectangle(x1, y1, w1, h1));
	}
	auto begin() {
		return selection.begin();
	}
	auto end() {
		return selection.end();
	}
	bool isActive() const {
		return active;
	}
	void setToActive() {
		active = true;
	}
	vector<Rectangle>& getRectangles() {
		return selection;
	}
	string getName() const {
		return name;
	}
	static Selection* createSelection(int, int);
	void printInfo();
};
