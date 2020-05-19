#include "Selection.h"
#include <iostream>
using namespace std;

Selection* Selection::createSelection(int width, int height) {
	int x1 = 0, y1 = 0, w = 0, h = 0;
	bool a = false;
	string s;
	cout << "Unesite naziv selekcije: ";
	cin >> s;
	cout << "Unesite 1 ako je selekcija aktivna, ili 0 ako nije: ";
	cin >> a;
	Selection* sel = new Selection(s, a);
	while (1) {
		cout << "Unesite koordinate oblika(x - sirina, y - visina): ";
		cin >> x1 >> y1;
		if ((x1 < 0) || (y1 < 0) || (x1 > width) || (y1 > height)) return nullptr;
		cout << "Unesite sirinu i visinu oblika: ";
		cin >> w >> h;
		if (((x1 + w) > width) || ((x1 + w) < 0) || ((y1 + h) > height) || ((y1 + h) < 0)) return nullptr;
		sel->addShape(x1, y1, w, h);
		cout << "Unesite 1 ako zelite da nastavite sa pravljenjem oblika: ";
		cin >> x1;
		if (x1 != 1) break;
	}
	return sel;
}

void Selection::printInfo() {
	int cnt = 1;
	if (isActive()) cout << "AKTIVNA SELEKCIJA"; else cout << "NEAKTIVNA SELEKCIJA";
	cout << endl;
	for (Rectangle& r : selection) {
		cout << "Oblik " << cnt++ << ":" << endl;
		cout << "Koordinate pocetka: " << r.x << ", " << r.y << endl;
		cout << "Sirina: " << r.w << endl;
		cout << "Visina: " << r.h << endl;
	}
}