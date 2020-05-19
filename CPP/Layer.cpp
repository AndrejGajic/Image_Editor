#include "Layer.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <array>

void Layer::writeBMP(ofstream& outFile) {
	
	for (int i = 0; i < layer.size(); i++) {
		for (int j = 0; j < layer[i].size(); j++) {
			byte r = layer[i][j].getR();
			byte g = layer[i][j].getG();
			byte b = layer[i][j].getB();
			byte a = layer[i][j].getA();
			outFile.write((const char*)&b, 1);
			outFile.write((const char*)&g, 1);
			outFile.write((const char*)&r, 1);
			outFile.write((const char*)&a, 1);
		}
	}
}


void Layer::writePAM(ofstream& outFile) {
	for (int i = layer.size() - 1; i > -1; i--) {
		for (int j = 0; j < layer[i].size(); j++) {
			byte r = layer[i][j].getR();
			byte g = layer[i][j].getG();
			byte b = layer[i][j].getB();
			byte a = layer[i][j].getA();
			outFile.write((const char*)&r, 1);
			outFile.write((const char*)&g, 1);
			outFile.write((const char*)&b, 1);
			outFile.write((const char*)&a, 1);
		}
	}
}

void Layer::execute(Operation* o, int r1, int g1, int b1) {
	if (o->mediana) {
		o->execute((*this)[0][0], 0, 0, 0);
	}
	else if (!slct) {
		for (auto& pom : layer) {
			for (Pixel& p : pom) {
				o->execute(p, r1, g1, b1);
			}
		}
	}
	else {
		for (Selection& sel : selections) {
			if (sel.isActive()) {
				for (Rectangle& rect : sel) {
					for (int i = rect.y; i < (rect.h + rect.y); i++) {
						for (int j = rect.x; j < (rect.w + rect.x); j++) {
							o->execute(layer[i][j], r1, g1, b1);
						}
					}
				}
			}
		}
	}
}


void Layer::addSelections(int width, int height) {
	int x = 1;
	while (x) {
		cout << endl << setw(5) << " ";
		printf("\x1B[34mSELEKCIJE \033[0m");
		cout << setw(5) << " " << endl;
		cout << "1. Pravljenje nove selekcije." << endl;
		cout << "2. Dohvatanje informacije o selekciji." << endl;
		cout << "3. Brisanje selekcije." << endl;
		cout << "4. Aktiviranje selekcije." << endl;
		cout << "0. Kraj rada sa selekcijama." << endl;
		printf("\x1B[36mIzaberite jednu od navedenih operacija: \033[0m"); cin >> x;
		if (x == 1) {
			cout << "Sirina slike: " << width << " ; Visina slike: " << height << endl;
			Selection* sel = Selection::createSelection(width, height);
			if (sel == nullptr) {
				printf("\x1B[31mNiste uneli odgovarajuce parametre!\033[0m"); cout << endl;
				continue;
			}
			if (sel->isActive()) slct = true;
			selections.push_back(*sel);
		}
		else if (x == 2) {
			string s;
			printf("\x1B[36mUnesite ime selekcije: \033[0m"); cin >> s;
			this->findSelection(s);
		}
		else if (x == 3) {
			string s;
			printf("\x1B[36mUnesite ime selekcije: \033[0m"); cin >> s;
			this->deleteSelection(s);
			int cnt = count_if(selections.begin(), selections.end(),
				[](const Selection& selection2)->bool {return selection2.isActive(); });
			if (cnt == 0) slct = false;
			else slct = true;
		}
		else if (x == 4) {
			string s;
			printf("\x1B[36mUnesite ime selekcije: \033[0m"); cin >> s;
			bool b = this->activateSelection(s);
			if (b) slct = true;
		}
	}
	
}


void Layer::findSelection(string s) {
	bool x = false;
	for (Selection& sel : selections) {
		if (sel.getName() == s) {
			x = true;
			sel.printInfo();
			break;
		}
	}
	if (!x) printf("\x1B[31mSelekcija sa zadatim imenom ne postoji! \033[0m"); cout << endl;
}

void Layer::deleteSelection(string s) {
	if (selections.size() == 0) {
		printf("\x1B[31mSelekcija sa zadatim imenom ne postoji! \033[0m"); cout << endl;
		return;
	}
	int x = selections.size();
	auto iter = remove_if(selections.begin(), selections.end(),
		[s](const Selection& sel)->bool {return sel.getName() == s; }
	);
	selections.erase(iter, selections.end());
	if (x == selections.size()) { printf("\x1B[31mSelekcija sa zadatim imenom ne postoji! \033[0m"); cout << endl; }
	else {
		printf("\x1B[32mSelekcija uspesno uklonjena! \033[0m"); cout << endl;
		if (selections.size() == 0) this->slct = false;
	}
}


bool Layer::activateSelection(string s) {
	bool x = false;
	for (Selection& sel : selections) {
		if (sel.getName() == s) {
			x = true;
			sel.setToActive();
			break;
		}
	}
	if (!x) {
		printf("\x1B[31mSelekcija sa zadatim imenom ne postoji! \033[0m"); cout << endl;
	}
	return x;
}


void Layer::formatHeight(int h) {
	layer.resize(h);
	setHeight(h);
}

void Layer::formatWidth(int w) {
	for (auto& pom : layer) {
		int x = this->getWidth();
		pom.resize(w);
		while (x < w) {
			pom[x].setAlpha(0);
			x++;
		}
	}
	this->setWidth(w);
}

void Layer::setAlpha(double alpha) {
	
	for (auto& pom : layer) {
		for (Pixel& p : pom) {
			double a = (double)p.getA() * (alpha / 100);
			p.setAlpha((int)a);
		}
	}
}

void Layer::printInfo() {
	cout << "Slika na ovom sloju: " << name << endl;
	cout << "Sirina sloja: " << width << endl;
	cout << "Visina sloja: " << height << endl;
}




void Layer::median() {
	int r1 = 0, g1 = 0, b1 = 0;
	Layer image2 = *this; // kopija
	if (!slct) {
		for (int i = 0; i < this->height; i++) {
			for (int j = 0; j < this->width; j++) {
				if ((i == 0) && (j == 0)) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j + 1].getR(), image2[i + 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j + 1].getG(), image2[i + 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j + 1].getB(), image2[i + 1][j].getB());
				}
				else if ((i == 0) && (j == this->width - 1)) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i + 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i + 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i + 1][j].getB());
				}
				else if ((i == this->height - 1) && (j == 0)) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB());
				}
				else if ((i == this->height - 1) && (j == this->width - 1)) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i - 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i - 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i - 1][j].getB());
				}
				else if (i == 0) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i + 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i + 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i + 1][j].getB());
				}
				else if (j == 0) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR(), image2[i][j + 1].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG(), image2[i][j + 1].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB(), image2[i][j + 1].getB());
				}
				else if (i == this->height - 1) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB());
				}
				else if (j == this->width - 1) {
					r1 = calcMediana((*this)[i][j].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR(), image2[i][j - 1].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG(), image2[i][j - 1].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB(), image2[i][j - 1].getB());
				}
				else {
					r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR());
					g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG());
					b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB());
				}
				(*this)[i][j].setAllInt(r1, g1, b1);
			}
		}
	}
	else {
		for (Selection& sel : selections) {
			if (sel.isActive()) {
				for (Rectangle& rect : sel) {
					for (int i = rect.y; i < (rect.h + rect.y); i++) {
						for (int j = rect.x; j < (rect.w + rect.x); j++) {
							if ((i == 0) && (j == 0)) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j + 1].getR(), image2[i + 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j + 1].getG(), image2[i + 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j + 1].getB(), image2[i + 1][j].getB());
							}
							else if ((i == 0) && (j == this->width - 1)) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i + 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i + 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i + 1][j].getB());
							}
							else if ((i == this->height - 1) && (j == 0)) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB());
							}
							else if ((i == this->height - 1) && (j == this->width - 1)) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i - 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i - 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i - 1][j].getB());
							}
							else if (i == 0) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i + 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i + 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i + 1][j].getB());
							}
							else if (j == 0) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR(), image2[i][j + 1].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG(), image2[i][j + 1].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB(), image2[i][j + 1].getB());
							}
							else if (i == this->height - 1) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB());
							}
							else if (j == this->width - 1) {
								r1 = calcMediana((*this)[i][j].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR(), image2[i][j - 1].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG(), image2[i][j - 1].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB(), image2[i][j - 1].getB());
							}
							else {
								r1 = calcMediana((*this)[i][j].getR(), image2[i][j - 1].getR(), image2[i][j + 1].getR(), image2[i - 1][j].getR(), image2[i + 1][j].getR());
								g1 = calcMediana((*this)[i][j].getG(), image2[i][j - 1].getG(), image2[i][j + 1].getG(), image2[i - 1][j].getG(), image2[i + 1][j].getG());
								b1 = calcMediana((*this)[i][j].getB(), image2[i][j - 1].getB(), image2[i][j + 1].getB(), image2[i - 1][j].getB(), image2[i + 1][j].getB());
							}
							(*this)[i][j].setAllInt(r1, g1, b1);
						}
					}
				}
			}
		}







	}
}

int Layer::calcMediana(int x1, int x2, int x3, int x4, int x5) {
	if ((x4 == -1) && (x5 == -1)) {
		std::array<int, 3> arr = { x1, x2, x3 };
		std::sort(arr.begin(), arr.end());
		return arr[1];
	}
	else if (x5 == -1) {
		std::array<int, 4> arr = { x1, x2, x3, x4 };
		std::sort(arr.begin(), arr.end());
		return (int)((arr[1] + arr[2]) / 2);
	}
	else {
		std::array<int, 5> arr = { x1, x2, x3, x4, x5 };
		std::sort(arr.begin(), arr.end());
		return arr[2];
	}
}