#include "BMP.h"


void BMPFormatter::parse(ifstream* input) {
	input->read((char*)&header1, sizeof(header1));
	input->read((char*)&header2, sizeof(header2));
	if (header2.bitsPerPixel == 32) {
		input->read((char*)&colorHeader, sizeof(colorHeader));
	}
	channels = header2.bitsPerPixel / 8;
	input->seekg(header1.offset, input->beg); // beg = begin
	myLayer.resize(header2.height);
	if (header2.width % 4 == 0) {
		for (int i = 0; i < header2.height; i++) {
			myLayer[i].resize(header2.width);
			for (int j = 0; j < header2.width; j++) {
				byte b, g, r, a;
				a = 255;
				if ((channels == 1) || (channels == 2)) {
					input->read((char*)&b, 1);
					r = b;
					g = b;
					if (channels == 2) input->read((char*)&a, 1);
				}
				else {
					input->read((char*)&b, 1);
					input->read((char*)&g, 1);
					input->read((char*)&r, 1);
					if (channels == 4) input->read((char*)&a, 1);
				}
				Pixel p(r, g, b, a);
				myLayer[i][j] = p;
			}
		}
	}
	else {
		int temp = header2.width * channels;
		
		while (temp % 4 != 0) temp++;
		vector<byte> empty_bytes(temp - header2.width * channels);
		for (int i = 0; i < header2.height; i++) {
			myLayer[i].resize(header2.width);
			for (int j = 0; j < header2.width; j++) {
				byte b, g, r, a;
				a = 255;
				if ((channels == 1) || (channels == 2)) {
					input->read((char*)&b, 1);
					r = b;
					g = b;
					if (channels == 2) input->read((char*)&a, 1);
				}
				else {
					input->read((char*)&b, 1);
					input->read((char*)&g, 1);
					input->read((char*)&r, 1);
					if (channels == 4) input->read((char*)&a, 1);
				}
				myLayer[i][j] = Pixel(r, g, b, a);
			}
			input->read((char*)empty_bytes.data(), empty_bytes.size());
		}
	}

	if (channels != 4) convert24to32();

} 


void BMPFormatter::convert24to32() {
	header2.size = sizeof(header2) + sizeof(colorHeader);
	header1.offset = sizeof(header1) + sizeof(header2) + sizeof(colorHeader);
	header1.size = header1.offset + myLayer.getSize() * myLayer[0].size();
	header2.bitsPerPixel = 32;
	header2.BI_RGB = 3;
	channels = 4;
}

void BMPFormatter::write(string outName, Layer L) {
	ofstream outFile(outName, ios_base::binary);
	makeFileHeaders(outFile);
	L.writeBMP(outFile);
}

void BMPFormatter::makeFileHeaders(ofstream& outFile) {
	outFile.write((const char*)&header1, sizeof(header1));
	outFile.write((const char*)&header2, sizeof(header2));
	outFile.write((const char*)&colorHeader, sizeof(colorHeader));
}

void BMPFormatter::remakeHeader(int width, int height) {
	header2.width = width;
	header2.height = height;
	header1.size = width * height * channels + header1.offset;
	header2.size = sizeof(header2) + sizeof(colorHeader);
	header1.offset = sizeof(header1) + sizeof(header2) + sizeof(colorHeader);
	header2.bitmapSize = width * height * channels;
}