#include <iostream>
#include <fstream>
#include "BMP.h"
#include "XML.h"
#include "Formatter.h"
#include "Image.h"
#include <iomanip>
#include "Colors.h"
using namespace std;


void Konzola() {
	cout << endl;
	cout << setfill('-') << setw(15) << '-'; printf("\x1B[33mKONZOLA\033[0m");  cout << setfill('-') << setw(15) << '-' << endl;
	cout << "1. Rad sa slojevima." << endl;
	cout << "2. Rad sa selekcijama." << endl;
	cout << "3. Obrada nad slikom." << endl;
	cout << "4. Eksportovanje slike." << endl;
	cout << "0. Kraj programa." << endl;
	printf("\x1B[36mIzaberite jednu od sledecih opcija: \033[0m");
}


bool b = false;
bool change = false;


int main(int argc, char* argv[]) {

	if (argc == 3) {
		// ARGUMENTI: velika.bmp/velika.pam i TEST.fun
		string imagePath = argv[1];
		string operationPath = argv[2];
		Layer image;
		ifstream* myImage = new ifstream(imagePath, ios_base::binary);
		if (imagePath.back() == 'p') {
			BMPFormatter* pom = new BMPFormatter(myImage);
			image = pom->getLayer();
			image.setName(imagePath);
			image.setHeight(image.getSize());
			image.setWidth(image[0].size());
		}
		else if (imagePath.back() == 'm') {
			PAMFormatter* pom = new PAMFormatter(myImage);
			image = pom->getLayer();
			image.setName(imagePath);
			image.setHeight(image.getSize());
			image.setWidth(image[0].size());
		}
		delete myImage;
		
		XMLFormatter* xml = new XMLFormatter();
		image.setCompositeOperation(xml->importOperation(operationPath));
		image.execute(image.getCompositeOperation());
		
		string outFile = "RESULT.bmp";
		BMPFormatter* out = new BMPFormatter(1);
		out->remakeHeader(image.getWidth(), image.getHeight());
		out->write(outFile, image);
		printf("\x1B[32mOperacija izvresna uspesno!\033[0m"); cout << endl;
		return 1;
	}
	

	int x = 0;
	XMLFormatter* pom = new XMLFormatter;
	Image* myImage = new Image();
	cout << setw(15) << " "; printf("\033[3;43;30mPROGRAM ZA OBRADU DIGITALNIH BMP I PAM SLIKA\033[0m"); cout << setw(15) << " " << endl;
	while (1) {
		Konzola();
		cin >> x;
		switch (x) {
		case 1: {
			myImage->layersMenu();
			b = true;
			change = true;
			break;
		}
		case 2: {
			if (!b) {
				printf("\x1B[31mNemate ni jedan sloj!\033[0m"); cout << endl;
				break;
			}
			myImage->addSelections();
			change = true;
			break;
		}
		case 3: {
			if (!b) {
				printf("\x1B[31mNemate ni jedan sloj!\033[0m"); cout << endl;
				break;
			}
			myImage->operations();
			change = true;
			break;
		}
		case 4: {
			if (!b) {
				printf("\x1B[31mNemate ni jedan sloj!\033[0m"); cout << endl;
				break;
			}
			myImage->exportImage();
			change = false;
			break;
		}
		case 0: {
			if (change) printf("\x1B[31mNISTE SACUVALI PROMENE NA SLICI!\033[0m"); cout << endl;
			printf("\x1B[36mDa li ste sigurni da zelite da izadjete iz programa? (1 - DA, 0 - NE): \033[0m"); 
			cin >> x;
			if (x == 1) {
				printf("\x1B[35mProgram je zavrsio sa radom!\033[0m"); cout << endl;
				system("PAUSE");
				exit(1);
			}
			break;
		}


		}

	}

}