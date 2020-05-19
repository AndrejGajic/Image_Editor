#pragma once
#include "Formatter.h"

struct PAMHeader {
	string magicNum = "P7";
	int width;
	int height;
	int channels;
	int maxval;
	string tupltype;
	string end = "ENDHDR";
};

class PAMFormatter : public Formatter {
private:
	PAMHeader pamHeader;
public:
	PAMFormatter(ifstream* input) {
		parse(input);
	}
	PAMFormatter(int x) {

	}
	void parse(ifstream* input) override;
	void convert24to32();
	void write(string outName, Layer L) override;
	void remakeHeader(int width, int height) override;
	void writePAMHeader(ofstream&);
};