#pragma once
#include "Formatter.h"
using byte2 = uint16_t;
using byte4 = uint32_t;
#pragma pack(push, 1)

struct BMPHeader { // File Header
	byte2 ID = 0x4D42; 
	byte4 size = 0;
	byte2 x1 = 0;
	byte2 x2 = 0;
	byte4 offset = 0;
};

struct DIBHeader { // Info Header
	byte4 size = 0;
	int32_t width = 0;
	int32_t height = 0;
	byte2 color = 1; // nebitno
	byte2 bitsPerPixel = 32; // 24B/P ili 32B/P
	byte4 BI_RGB = 3; // kompresija
	byte4 bitmapSize = 0;
	int32_t x1 = 39;
	int32_t x2 = 39;
	byte4 x3 = 0;
	byte4 x4 = 0;
};

struct ColorHeader {
	byte4 redMask = 0x00ff0000;
	byte4 greenMask = 0x0000ff00;
	byte4 blueMask = 0x000000ff;
	byte4 alphaMask = 0xff000000;
	byte4 sRGB = 0x73524742;
	byte4 x[16] = { 0 };
};

#pragma pack(pop)

class BMPFormatter : public Formatter {
private:
	BMPHeader header1;
	DIBHeader header2;
	ColorHeader colorHeader;
	int channels = 0; // 3 (B, G, R) ili 4 (B, G, R, A)
public:
	BMPFormatter(ifstream* input) {
		parse(input);
	}
	BMPFormatter(int x) {

	}
	void parse(ifstream* input) override;
	void convert24to32();
	void write(string outName, Layer L) override;
	void makeFileHeaders(ofstream&);
	void remakeHeader(int width, int height) override;
};