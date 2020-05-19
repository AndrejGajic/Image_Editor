#include "XML.h"
#include "tinyxml2.h"
#include "Operation.h"
#include "BasicOperation.h"
#include "BMP.h"
#include "PAM.h"
#include "Selection.h"

using namespace tinyxml2;


void XMLFormatter::exportOperation(string outName) {

	XMLDocument xmlOperation;
	XMLNode* root = xmlOperation.NewElement("ROOT");
	xmlOperation.InsertFirstChild(root);
	XMLElement* oper = xmlOperation.NewElement("Operations");
	for (auto& a : o->operations) {
		XMLElement* el = xmlOperation.NewElement("Operation");
		el->SetAttribute("Name", a.first->getName().c_str());
		el->SetAttribute("R", a.second.red);
		el->SetAttribute("G", a.second.green);
		el->SetAttribute("B", a.second.blue);
		oper->InsertEndChild(el);
	}
	root->InsertEndChild(oper);
	xmlOperation.SaveFile(outName.c_str());

}


CompositeOperation* XMLFormatter::importOperation(string inName) {
	XMLDocument xmlOperation;
	CompositeOperation* comp = new CompositeOperation();
	xmlOperation.LoadFile(inName.c_str());
	XMLNode* root = xmlOperation.FirstChild();
	if (!root) {
		printf("\x1B[31mNiste uneli odgovarajuce ime XML fajla!\033[0m"); cout << endl;
		return nullptr;
	}
	XMLElement* oper = root->FirstChildElement("Operations");
	XMLElement* el = oper->FirstChildElement("Operation");
	Operation* op = nullptr;
	colors clr;
	const char* name;
	string nameS;
	while (el) {
		name = el->Attribute("Name");
		nameS = name;
		el->QueryIntAttribute("R", &clr.red);
		el->QueryIntAttribute("G", &clr.green);
		el->QueryIntAttribute("B", &clr.blue);
		op = makeOperation(nameS);
		
		comp->addOperation(op, clr.red, clr.green, clr.blue);

		el = el->NextSiblingElement("Operation");
	}

	return comp;
}




void XMLFormatter::exportProject(string outName, Layer& image, CompositeOperation* comp) {
	XMLDocument xmlProject;
	XMLNode* root = xmlProject.NewElement("ROOT");
	xmlProject.InsertFirstChild(root);
	printf("\x1B[36mUnesite naziv BMP ili PAM fajla u koji zelite da eksportujete sliku: \033[0m");
	string s;
	cin >> s;
	if (s.back() == 'p') {
		BMPFormatter* pom = new BMPFormatter(1);
		pom->remakeHeader(image.getWidth(), image.getHeight());
		pom->write(s, image);
	}
	else if (s.back() == 'm') {
		PAMFormatter* pom = new PAMFormatter(1);
		pom->remakeHeader(image.getWidth(), image.getHeight());
		pom->write(s, image);
	}
	else {
		printf("\x1B[31mNiste uneli odgovarajuci naziv fajla!\033[0m"); cout << endl;
		return;
	}
	XMLElement* elem = xmlProject.NewElement("Layer");
	elem->SetAttribute("Name", s.c_str());
	root->InsertEndChild(elem);
	elem = xmlProject.NewElement("Selections");
	if (!image.hasSelections()) elem->SetAttribute("HasSelections", 0);
	else {
		elem->SetAttribute("HasSelections", 1);
		for (Selection& s : image.getSelections()) {
			XMLElement* elemS = xmlProject.NewElement("Selection");
			elemS->SetAttribute("Name", s.getName().c_str());
			int x = s.isActive();
			elemS->SetAttribute("Active", x);
			elemS->SetAttribute("CountR", s.getRectangles().capacity());
			for (Rectangle& r : s.getRectangles()) {
				XMLElement* elemR = xmlProject.NewElement("Rectangle");
				elemR->SetAttribute("Height", r.h);
				elemR->SetAttribute("Width", r.w);
				elemR->SetAttribute("X", r.x);
				elemR->SetAttribute("Y", r.y);
				elemS->InsertEndChild(elemR);
			}
			elem->InsertEndChild(elemS);
		}
	}
	root->InsertEndChild(elem);
	elem = xmlProject.NewElement("Operations");
	if (!comp) elem->SetAttribute("HasComposite", 0);
	else {
		elem->SetAttribute("HasComposite", 1);
		for (auto& a : comp->operations) {
			XMLElement* el = xmlProject.NewElement("Operation");
			el->SetAttribute("Name", a.first->getName().c_str());
			el->SetAttribute("R", a.second.red);
			el->SetAttribute("G", a.second.green);
			el->SetAttribute("B", a.second.blue);
			elem->InsertEndChild(el);
		}
	}
	root->InsertEndChild(elem);
	xmlProject.SaveFile(outName.c_str());
}


Layer XMLFormatter::importProject(string inName) {
	Layer image;
	XMLDocument xmlProject;
	xmlProject.LoadFile(inName.c_str());
	XMLNode* root = xmlProject.FirstChild();
	if (!root) {
		printf("\x1B[31mNiste uneli ispravno ime XML projekta!\033[0m"); cout << endl;
		image.setName("GRESKA");
		return image;
	}
	XMLElement* elem = root->FirstChildElement("Layer");
	const char* name = elem->Attribute("Name");
	string nameS = name;
	ifstream* myImage = new ifstream(nameS, ios_base::binary);
	if (nameS.back() == 'p') {
		BMPFormatter* pom = new BMPFormatter(myImage);
		image = pom->getLayer();
		image.setName(nameS);
		image.setHeight(image.getSize());
		image.setWidth(image[0].size());
	}
	else if (nameS.back() == 'm') {
		PAMFormatter* pom = new PAMFormatter(myImage);
		image = pom->getLayer();
		image.setName(nameS);
		image.setHeight(image.getSize());
		image.setWidth(image[0].size());
	}
	delete myImage;

	elem = root->FirstChildElement("Selections");
	int x = 0;
	elem->QueryIntAttribute("HasSelections", &x);
	if (x == 1) {
		image.setSlct(true);
		XMLElement* sel = elem->FirstChildElement("Selection");
		while (sel) {
			int a = 0, cnt = 0;
			const char* nameSel;
			string nameSelS;
			nameSel = sel->Attribute("Name");
			nameSelS = nameSel;
			sel->QueryIntAttribute("Active", &a);
			sel->QueryIntAttribute("CountR", &cnt);
			Selection s(nameSelS, a);
			if (a == 1) s.setToActive();
			XMLElement* rect = sel->FirstChildElement("Rectangle");
			for (int i = 0; i < cnt; i++) {
				int h1 = 0, w1 = 0, x1 = 0, y1 = 0;
				rect->QueryIntAttribute("Height", &h1);
				rect->QueryIntAttribute("Width", &w1);
				rect->QueryIntAttribute("X", &x1);
				rect->QueryIntAttribute("Y", &y1);
				s.addShape(x1, y1, w1, h1);
				rect = rect->NextSiblingElement("Rectangle");
			}
			image.addSelection(s);
			sel = sel->NextSiblingElement("Selection");
		}
	}

	elem = root->FirstChildElement("Operations");
	elem->QueryIntAttribute("HasComposite", &x);
	if (x == 1) {
		CompositeOperation* comp = new CompositeOperation();
		XMLElement* el = elem->FirstChildElement("Operation");
		Operation* op = nullptr;
		colors clr;
		const char* name;
		string nameS;
		while (el) {
			name = el->Attribute("Name");
			nameS = name;
			el->QueryIntAttribute("R", &clr.red);
			el->QueryIntAttribute("G", &clr.green);
			el->QueryIntAttribute("B", &clr.blue);
			op = makeOperation(nameS);

			comp->addOperation(op, clr.red, clr.green, clr.blue);

			el = el->NextSiblingElement("Operation");
		}
		image.setCompositeOperation(comp);
	}


	return image;
}





Operation* XMLFormatter::makeOperation(string name) {

	Operation* o = nullptr;
	if (name == "Addition") {
		o = new Addition();
		o->setName("Addition");
	}
	else if (name == "Subtraction") {
		o = new Subtraction();
		o->setName("Subtraction");
	}
	else if (name == "InverseSubtraction") {
		o = new InverseSubtraction();
		o->setName("InverseSubtraction");
	}
	else if (name == "Multiplication") {
		o = new Multiplication();
		o->setName("Multiplication");
	}
	else if (name == "Division") {
		o = new Division();
		o->setName("Division");
	}
	else if (name == "InverseDivision") {
		o = new InverseDivision();
		o->setName("InverseDivision");
	}
	else if (name == "Power") {
		o = new Power();
		o->setName("Power");
	}
	else if (name == "Logarithm") {
		o = new Logarithm();
		o->setName("Logarithm");
	}
	else if (name == "AbsoluteValue") {
		o = new AbsoluteValue();
		o->setName("AbsoluteValue");
	}
	else if (name == "Minimum") {
		o = new Minimum();
		o->setName("Minimum");
	}
	else if (name == "Maximum") {
		o = new Maximum();
		o->setName("Maximum");
	}
	else if (name == "Grayscale") {
		o = new Grayscale();
		o->setName("Grayscale");
	}
	else if (name == "BlackWhite") {
		o = new BlackWhite();
		o->setName("BlackWhite");
	}
	else if (name == "Inversion") {
		o = new Inversion();
		o->setName("Inversion");
	}
	else if (name == "Median") {
		o = new Mediana();
		o->setName("Median");
	}
	else if (name == "AllRed") {
		o = new AllRed();
		o->setName("AllRed");
	}
	else if (name == "AllGreen") {
		o = new AllGreen();
		o->setName("AllGreen");
	}
	else if (name == "AllBlue") {
		o = new AllBlue();
		o->setName("AllBlue");
	}
	return o;
}