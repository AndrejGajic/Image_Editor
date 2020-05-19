#include "Image.h"
#include <string>
#include <iomanip>

void Image::layersMenu() {
	cout << endl << setw(5) << " ";
	printf("\x1B[34mSLOJEVI \033[0m");
	cout << setw(5) << " " << endl;
	int x = 1;
	while (x) {
		cout << "1. Kreiranje novog sloja." << endl;
		cout << "2. Brisanje postojeceg sloja." << endl;
		cout << "3. Dobijanje informacija o sloju." << endl;
		cout << "0. Kraj rada sa slojevima." << endl;
		printf("\x1B[36mIzaberite jednu od navedenih operacija: \033[0m"); cin >> x;
		switch (x) {
		case 1:
			importImage();
			break;
		case 2:
			deleteLayer();
			break;
		case 3:
			infoLayer();
			break;
		case 0:
			break;
		}

	}
}





void Image::importImage() {
	int choice = 0;
	while (!choice) {
	printf("\x1B[36mUnesite 1 za BMP format, 2 za PAM format, 3 za XML format: \033[0m");
	cin >> choice;
		switch (choice) {
		case 1: {
			string s;
			printf("\x1B[36mUnesite put do slike u BMP formatu: \033[0m");
			cin >> s;
			ifstream* myImage = new ifstream(s, ios_base::binary);
			if (!myImage->is_open()) {
				printf("\x1b[31mNiste uneli ispravan put do slike!\033[0m"); cout << endl;
				return;
			}
			myBMPFormatter = new BMPFormatter(myImage);
			Layer l = myBMPFormatter->getLayer();
			l.setName(s);
			l.setHeight(l.getSize());
			l.setWidth(l[0].size());
			if (l.getHeight() > this->height) this->height = l.getHeight();
			if (l.getWidth() > this->width) this->width = l.getWidth();
			int rr;
			printf("\x1B[36mUnesite 1 ako zelite da promenite opacity(neprozirnost) sloja: \033[0m"); cin >> rr;
			if (rr == 1) {
				double al = 0;
				printf("\x1B[36mUnesite sopstveni opacity(neprozirnost) slike (u procentima): \033[0m"); cin >> al;
				l.setAlpha(al);
			}
			layers.push_back(l);
			Layer& ll = layers[layers.size() - 1];
			this->merge(ll);
			delete myImage;
			printf("\x1B[32mUspesno napravljen BMP sloj!\033[0m"); cout << endl;
			break;
		}
		case 2: {
			string s;
			printf("\x1B[36mUnesite put do slike u PAM\ formatu: \033[0m");
			cin >> s;
			ifstream* myImage = new ifstream(s, ios_base::binary);
			if (!myImage->is_open()) {
				printf("\x1b[31mNiste uneli ispravan put do slike!\033[0m"); cout << endl;
				return;
			}
			myPAMFormatter = new PAMFormatter(myImage);
			Layer l = myPAMFormatter->getLayer();
			l.setName(s);
			l.setHeight(l.getSize());
			l.setWidth(l[0].size());
			if (l.getHeight() > this->height) this->height = l.getHeight();
			if (l.getWidth() > this->width) this->width = l.getWidth();
			int rr;
			printf("\x1B[36mUnesite 1 ako zelite da promenite opacity(neprozirnost) sloja: \033[0m"); cin >> rr;
			if (rr == 1) {
				double al = 0;
				printf("\x1B[36mUnesite sopstveni opacity(neprozirnost) slike (u procentima): \033[0m"); cin >> al;
				l.setAlpha(al);
			}
			layers.push_back(l);
			Layer& ll = layers[layers.size() - 1];
			this->merge(ll);
			delete myImage;
			printf("\x1B[32mUspesno napravljen PAM sloj!\033[0m"); cout << endl;
			break;
		}
		case 3: {
			string s;
			printf("\x1B[36mUnesite put do XML fajla (ekstenzija .proj): \033[0m");
			cin >> s;
			XMLFormatter* xml = new XMLFormatter();
			image = xml->importProject(s);
			if (image.getName() == "GRESKA") return;
			this->setCompositeOperation(image.getCompositeOperation());
			this->hasSelections = image.hasSelections();
			this->height = image.getHeight();
			this->width = image.getWidth();
			printf("\x1B[32mUspesno importovan XML fajl!\033[0m"); cout << endl;
			break;
		}
		default: {
			printf("\x1B[31mNiste uneli odgovarajuci broj! \033[0m"); cout << endl;
			choice = 0;
		}
		}
	}
}

void Image::exportImage() {
	int choice = 0;
	while (!choice) {
	cout << endl << "Unesite 1 za BMP format, 2 za PAM format, 3 za XML format: ";
	cin >> choice;
		switch (choice) {
		case 1: {
			string s;
			printf("\x1B[36mUnesite naziv izlaznog fajla: \033[0m");
			cin >> s;
			if (!myBMPFormatter) myBMPFormatter = new BMPFormatter(1);
			myBMPFormatter->remakeHeader(width, height);
			myBMPFormatter->write(s, image);
			printf("\x1B[32mUspesno eksportovana BMP slika!\033[0m"); cout << endl;
			break;
		}
		case 2: {
			string s;
			printf("\x1B[36mUnesite naziv izlaznog fajla: \033[0m");
			cin >> s;
			if (!myPAMFormatter) myPAMFormatter = new PAMFormatter(1);
			myPAMFormatter->remakeHeader(width, height);
			myPAMFormatter->write(s, image);
			printf("\x1B[32mUspesno eksportovana PAM slika!\033[0m"); cout << endl;
			break;
		}
		case 3: {
			string s;
			printf("\x1B[36mUnesite naziv izlaznog fajla: \033[0m");
			cin >> s;
			myXMLFormatter = new XMLFormatter();
			myXMLFormatter->exportProject(s, image, this->comp);
			printf("\x1B[32mUspesno eksportovan XML fajl!\033[0m"); cout << endl;
			break;
		}
		default: {
			cout << "Niste uneli odgovarajuci broj, unesite ponovo!" << endl;
			choice = 0;
		}
		}
	}
}


void Image::merge(Layer&l) {
	if (layers.size() == 1) {
		image = layers[0];
	}
	else {
		if (image.getHeight() > l.getHeight()) {
			l.formatHeight(image.getHeight());
		}
		else if (image.getHeight() < l.getHeight()) {
			image.formatHeight(l.getHeight());
		}

		if (image.getWidth() > l.getWidth()) {
			l.formatWidth(image.getWidth());
		}
		else if (image.getWidth() < l.getWidth()) {
			image.formatWidth(l.getWidth());
		}
		for (int i = 0; i < image.getHeight(); i++) {
			for (int j = 0; j < image.getWidth(); j++) {
				double A0 = (double)image[i][j].getA() / 255;
				double A1 = (double)l[i][j].getA() / 255;
				double A = A0 + (1 - A0) * A1;
				double R = (double)image[i][j].getR() * A0 / A + (double)l[i][j].getR() * (1 - A0) * A1 / A;
				double G = (double)image[i][j].getG() * A0 / A + (double)l[i][j].getG() * (1 - A0) * A1 / A;
				double B = (double)image[i][j].getB() * A0 / A + (double)l[i][j].getB() * (1 - A0) * A1 / A;
				A = A * 255;
				image[i][j].setAllA((int)R, (int)G, (int)B, (int)A);
			}
		}

	}
	this->width = image.getWidth();
	this->height = image.getHeight();
}

void Image::deleteLayer() {
	int rb;
	printf("\x1B[36mUnesite redni broj sloja koji zelite da uklonite: \033[0m"); cin >> rb;
	if (rb >= layers.size()) {
		printf("\x1B[31mNe postoji zadati sloj!\033[0m"); cout << endl;
	}
	else {
		layers.erase(layers.begin() + rb);
		mergeD();
		printf("\x1B[32mUspesno obrisan sloj!\033[0m"); cout << endl;
	}




}

void Image::infoLayer() {
	int rb;
	printf("\x1B[36mUnesite redni broj sloja: \033[0m"); cin >> rb;
	if (rb >= layers.size()) {
		printf("\x1B[31mNe postoji zadati sloj!\033[0m"); cout << endl;
	}
	else {
		layers[rb].printInfo();
	}
}



void Image::mergeD() {
	int cnt = 0;
	Layer pom = layers[0];
	image = pom;
	for (Layer& l : layers) {
		if (cnt == 0) {
			cnt++;
			continue;
		}
		merge(l);
	}
	this->width = image.getWidth();
	this->height = image.getHeight();
}



void consoleOperations() {
	cout << endl << setw(5) << " ";
	printf("\x1B[34mOPERACIJE \033[0m");
	cout << setw(5) << " " << endl;
	cout << "1. Sabiranje." << endl;
	cout << "2. Oduzimanje." << endl;
	cout << "3. Inverzno oduzimanje." << endl;
	cout << "4. Mnozenje." << endl;
	cout << "5. Deljenje." << endl;
	cout << "6. Inverzno deljenje." << endl;
	cout << "7. Stepen." << endl;
	cout << "8. Logaritam." << endl;
	cout << "9. Apsolutna vrednost." << endl;
	cout << "10. MIN (sve komponente vece od zadate konstante se postavljaju na tu vrednost)." << endl;
	cout << "11. MAX (sve komponente manje od zadate komponente se postavljaju na tu vrednost)." << endl;
	cout << "12. Bojenje slike(selekcija) u odredjenu boju." << endl;
	printf("\x1B[31m13. Naprednije funkcije. \033[0m"); cout << endl;
	printf("\x1B[35m14. Rad sa kompozitnim operacijama. \033[0m"); cout << endl;
	cout << "0. Kraj rada sa operacijama." << endl << endl;
	printf("\x1B[36mIzaberite jednu od operacija: \033[0m");
}

void consoleOperarationsAdvanced() {
	cout << "1. Invertovanje boja slike (255 - trenutna boja)." << endl;
	cout << "2. Pretvaranje slike u nijanse sive (grayscale)." << endl;
	cout << "3. Pretvaranje slike u crno-belu (black and white)." << endl;
	cout << "4. Pravljenje slike pomocu medijane susednih piksela." << endl;
	printf("\x1B[36mIzaberite jednu od navedenih operacija: \033[0m");
}


void Image::operations() {
	if (pom == 0)this->setPixelInt();
	else pom++;
	int choice = 0;
	bool flag = false;
	while (!flag) {
		Operation* o = nullptr;
		int r1 = 0, g1 = 0, b1 = 0;
		if (!inComposite) consoleOperations();
		else printf("\x1B[36mIzaberite jednu od operacija: \033[0m");
		cin >> choice;
		if ((choice == 1) || (choice == 2) || (choice == 3) || (choice == 4) || (choice == 5) || (choice == 6)) {
			printf("\x1B[36mUnesite 3 konstante (R, G, B) ili 0 ukoliko ne zelite da se operacija primenjuje nad tom komponentom: \033[0m");
			cin >> r1 >> g1 >> b1;
		}
		else if ((choice == 7) || (choice == 10) || (choice == 11)) {
			printf("\x1B[36mUnesite konstantu sa kojom zelite da uradite operaciju: \033[0m");
			cin >> r1;
		}
		switch (choice) {
		case 1: {
			o = new Addition();
			o->setName("Addition");
			break;
		}
		case 2: {
			o = new Subtraction();
			o->setName("Subtraction");
			break;
		}

		case 3: {
			o = new InverseSubtraction();
			o->setName("InverseSubtraction");
			break;
		}
		case 4: {
			o = new Multiplication();
			o->setName("Multiplication");
			break;
		}
		case 5: {
			o = new Division();
			o->setName("Division");
			break;
		}
		case 6: {
			o = new InverseDivision();
			o->setName("InverseDivision");
			break;
		}
		case 7: {
			o = new Power();
			o->setName("Power");
			break;
		}
		case 8: {
			o = new Logarithm();
			o->setName("Logarithm");
			break;
		}

		case 9: {
			o = new AbsoluteValue();
			o->setName("AbsoluteValue");
			break;
		}
		case 10: {
			o = new Minimum();
			o->setName("Minimum");
			break;
		}
		case 11: {
			o = new Maximum();
			o->setName("Maximum");
			break;
		}
		case 12: {
			printf("\x1B[36mUnesite 1 za bojenje u crveno, 2 za bojenje u zeleno, ili 3 za bojenje u plavo \033[0m");
			int clr; cin >> clr;
			if (clr == 1) {
				o = new AllRed(); o->setName("AllRed");
			}
			else if (clr == 2) {
				o = new AllGreen(); o->setName("AllGreen");
			}

			else if (clr == 3) {
				o = new AllBlue(); o->setName("AllBlue");
			}
			break;
		}
		case 13: {
			advancedOperations(o, r1, flag);
			break;
		}
		case 14: {
			if (!inComposite) {
				inComposite = true;
				composite();
				allToBinary();
				flag = true;
			}
			break;
		}
		case 0: {
			flag = true;
			if(!inComposite) allToBinary();
			break;
		}
		default: {
			cout << "Niste uneli odgovarajuci broj, unesite ponovo!" << endl;
			choice = 0;
			break;
		}
			
		}
		if (!inComposite) {
			if (!flag) this->executeOperation(o, r1, g1, b1);
		}
		else {
			if (!flag) this->comp->addOperation(o, r1, g1, b1);
		}
	}
}

void Image::advancedOperations(Operation*& o, int x, bool& flag) {
	int choice = 0;
	consoleOperarationsAdvanced();
	string pomS;
	cin >> choice;
	switch (choice) {
	case 1: {
		o = new Inversion();
		o->setName("Inversion");
		break;
	}
	case 2: {
		o = new Grayscale();
		o->setName("Grayscale");
		break;
	}
	case 3: {
		o = new BlackWhite();
		o->setName("BlackWhite");
		break;
	}
	case 4: {
		if (inComposite) break;
		Mediana* m = new Mediana();
		m->setLayer(image);
		o = m;
		o->setName("Median");
		o->mediana = true;
		break;
	}
	default: flag = true;
	}
}


void Image::executeOperation(Operation*& o, int r1, int g1, int b1) {
	
	image.execute(o, r1, g1, b1);
	printf("\x1B[32mOperacija uspesno izvrsena!\033[0m"); cout << endl;
	//delete o;
	
}



void Image::allToBinary() {
	int cnt = 0;
		for (auto& pom : image) {
			for (Pixel& p : pom) {
				p.intToBin();
			}
		}
	
}

void Image::setPixelInt() {
	for (auto& pom : image) {
		for (Pixel& p : pom) {
			p.setInt();
		}
	}
}


void Image::composite() {
	this->comp = new CompositeOperation();
	int choice = 0;
	cout << "Unesite 1 za kreiranje nove kompozitne operacije, ili 2 za uvoz postojece: ";
	cin >> choice;
	switch (choice) {
	case 1: {
		operations();
		image.execute(comp);
		printf("\x1B[32mKompozitna operacija uspesno izvrsena!\033[0m"); cout << endl;
		printf("\x1B[36mUnesite 1 ukoliko zelite da eksportujete kompozitnu operaciju: \033[0m");
		cin >> choice;
		if (choice == 1) {
			string s;
			printf("\x1B[36mUnesite naziv izlaznog XML fajla (ekstenzija .fun): \033[0m");
			cin >> s;
			myXMLFormatter = new XMLFormatter(comp);
			myXMLFormatter->exportOperation(s);
			printf("\x1B[32mKompozitna operacija uspesno eksportovana!\033[0m"); cout << endl;
		}
		break;
	}
	case 2: {
		myXMLFormatter = new XMLFormatter();
		string s;
		printf("\x1B[36mUnesite naziv ulaznog XML fajla (ekstenzija .fun): \033[0m");
		cin >> s;
		this->comp = myXMLFormatter->importOperation(s);
		if (!this->comp) break;
		image.execute(comp);
		printf("\x1B[32mKompozitna operacija uspesno izvrsena!\033[0m"); cout << endl;
		break;
	}
	default: break;
	}

	//delete comp;
	//comp = nullptr;
	inComposite = false;
}


void Image::addSelections() {
	image.addSelections(width, height);
	hasSelections = image.hasSelections();
}




