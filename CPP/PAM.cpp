#include "PAM.h"
#include <regex>

void PAMFormatter::parse(ifstream* input) {
	regex myRegex("([^0-9]*)([0-9]*)"); // I -> citaj sve do brojeva; II -> procitaj brojeve
	string s;
	smatch mySmatch;
	getline(*input, s);
	if (s != "P7") {
		cout << "Niste uneli ispravan PAM fajl!" << endl;
		return;
	}
	getline(*input, s);
	if (regex_match(s, mySmatch, myRegex)) pamHeader.width = atoi(mySmatch.str(2).c_str());
	getline(*input, s);
	if (regex_match(s, mySmatch, myRegex)) pamHeader.height = atoi(mySmatch.str(2).c_str());
	getline(*input, s);
	if (regex_match(s, mySmatch, myRegex)) pamHeader.channels = atoi(mySmatch.str(2).c_str());
	getline(*input, s);
	if (regex_match(s, mySmatch, myRegex)) pamHeader.maxval = atoi(mySmatch.str(2).c_str());
	getline(*input, s);
	pamHeader.tupltype = s.substr(9);
	getline(*input, s); // ENDHDR


	myLayer.resize(pamHeader.height);
	if (pamHeader.width % 4 == 0) {
		for (int i = pamHeader.height - 1; i > -1; i--) {
			myLayer[i].resize(pamHeader.width);
			for (int j = 0; j < pamHeader.width; j++) {
				byte b, g, r, a;
				a = 255;
				if ((pamHeader.tupltype == "RGB") || (pamHeader.tupltype == "RGB_ALPHA")) {
					input->read((char*)&r, 1);
					input->read((char*)&g, 1);
					input->read((char*)&b, 1);
					if (pamHeader.channels == 4) input->read((char*)&a, 1);
				}
				else if ((pamHeader.tupltype == "BLACKWHITE") || (pamHeader.tupltype == "BLACKWHITE_ALPHA")) {
					input->read((char*)&r, 1);
					if (r == 0) g = b = 0;
					else r = g = b = 255;
					if (pamHeader.tupltype == "BLACKWHITE_ALPHA") input->read((char*)&a, 1);
				}
				else if ((pamHeader.tupltype == "GRAYSCALE") || (pamHeader.tupltype == "GRAYSCALE_ALPHA")) {
					input->read((char*)&r, 1);
					b = r;
					g = r;
					if (pamHeader.tupltype == "GRAYSCALE_ALPHA") input->read((char*)&a, 1);
				}
				Pixel p(r, g, b, a);
				myLayer[i][j] = p;
			}
		}
	}
	else {
		int temp = pamHeader.width * pamHeader.channels;
		while (temp % 4 != 0) temp++;
		vector<byte> empty_bytes(temp - pamHeader.width * pamHeader.channels);
		for (int i = pamHeader.height - 1; i > -1; i--) {
			myLayer[i].resize(pamHeader.width);
			for (int j = 0; j < pamHeader.width; j++) {
				byte b, g, r, a;
				a = 255;
				if ((pamHeader.tupltype == "RGB") || (pamHeader.tupltype == "RGB_ALPHA")) {
					input->read((char*)&r, 1);
					input->read((char*)&g, 1);
					input->read((char*)&b, 1);
					if (pamHeader.channels == 4) input->read((char*)&a, 1);
				}
				else if ((pamHeader.tupltype == "BLACKWHITE") || (pamHeader.tupltype == "BLACKWHITE_ALPHA")) {
					input->read((char*)&r, 1);
					if (r == 0) g = b = 0;
					else r = g = b = 255;
					if (pamHeader.tupltype == "BLACKWHITE_ALPHA") input->read((char*)&a, 1);
				}
				else if ((pamHeader.tupltype == "GRAYSCALE") || (pamHeader.tupltype == "GRAYSCALE_ALPHA")) {
					input->read((char*)&r, 1);
					b = r;
					g = r;
					if (pamHeader.tupltype == "GRAYSCALE_ALPHA") input->read((char*)&a, 1);
				}
				Pixel p(r, g, b, a);
				myLayer[i][j] = p;
			}
			input->read((char*)empty_bytes.data(), empty_bytes.size());
		}
	}
	if (pamHeader.channels == 3) convert24to32();

}

void PAMFormatter::convert24to32() {
	pamHeader.tupltype = "TUPLTYPE RGB_ALPHA";
	pamHeader.channels = 4;

}


void PAMFormatter::write(string outName, Layer L) {
	ofstream out(outName, ios_base::binary);
	writePAMHeader(out);
	L.writePAM(out);
}

void PAMFormatter::writePAMHeader(ofstream& out) {
	out << pamHeader.magicNum << "\n";
	out << "WIDTH " << pamHeader.width << "\n";
	out << "HEIGHT " << pamHeader.height << "\n";
	out << "DEPTH " << pamHeader.channels << "\n";
	out << "MAXVAL " << pamHeader.maxval << "\n";
	out << "TUPLTYPE " << pamHeader.tupltype << "\n";
	out << pamHeader.end << "\n";
}

void PAMFormatter::remakeHeader(int width, int height) {
	pamHeader.width = width;
	pamHeader.height = height;
	pamHeader.maxval = 255;
	pamHeader.channels = 4;
	pamHeader.tupltype = "RGB_ALPHA";
}