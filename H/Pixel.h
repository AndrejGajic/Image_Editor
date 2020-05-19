#pragma once
#include <iostream>
using namespace std;
using byte = uint8_t;

class Pixel {
private:
	byte R;
	byte G;
	byte B;
	byte A = 255; // alpha
	int R1,  G1,  B1,  A1; // koristimo ih u operacijama i kompozitnim funkcijama
public:
	Pixel(byte R = 0, byte G = 0, byte B = 0, byte A = 0) {
		this->B = B;
		this->G = G;
		this->R = R;
		this->A = A;
		setInt();
	}
	void setInt() {
		R1 = (int)R;
		G1 = (int)G;
		B1 = (int)B;
		A1 = (int)A;
	}
	bool hasA() const {
		return (A != 0);
	}
	int getR() const {
		return R1;
	}
	int getG() const {
		return G1;
	}
	int getB() const {
		return B1;
	}
	int getA() const {
		return A1;
	}
	void setAll(int red, int green, int blue) {
		correct(red); correct(green); correct(blue);
		R = red; G = green; B = blue;
		R1 = R; G1 = G; B1 = B;
	}
	void setAllA(int red, int green, int blue, int alpha) {
		setAll(red, green, blue);
		correct(alpha);
		A = A1 = alpha;
	}


	void setAllInt(int red, int green, int blue) {
		R1 = red; G1 = green; B1 = blue;
	}
	void intToBin() {
		if (R1 < 0) R1 = 0; if (G1 < 0) G1 = 0; if (B1 < 0) B1 = 0;
		if (R1 > 255) R1 = 255; if (G1 > 255) G1 = 255; if (B1 > 255) B1 = 255;
		R = (byte)R1; G = (byte)G1; B = (byte)B1;
	}

	friend ostream& operator<<(ostream& os, const Pixel& p) {
		cout << "[" << p.getR() << ", " << p.getG() << ", " << p.getB() << ", " << p.getA() << "]";
	}

	void correct(int& x) {
		if (x < 0) x = 0;
		if (x > 255) x = 255;
	}
	void setAlpha(int alpha) {
		A = (byte)alpha;
		A1 = alpha;
	}
};