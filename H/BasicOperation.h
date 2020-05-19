#pragma once
#include "Operation.h"
#include <math.h>

class BasicOperation : public Operation {
public:
	BasicOperation()  {
		basic = true;
		mediana = false;
	}
	virtual Pixel& execute(Pixel& p, const int r1, const int g1, const int b1) = 0;
};


class Addition : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1, const int g1, const int b1) override {
		int r = 0, g = 0, b = 0;
		if (r1 != 0) r = p.getR() + r1; else r = p.getR();
		if (g1 != 0) g = p.getG() + g1; else g = p.getG();
		if (b1 != 0) b = p.getB() + b1; else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class Subtraction : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1, const int g1, const int b1) override {
		int r, g, b;
		if (r1 != 0) r = p.getR() - r1; else r = p.getR();
		if (g1 != 0) g = p.getG() - g1; else g = p.getG();
		if (b1 != 0) b = p.getB() - b1; else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class InverseSubtraction : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r, g, b;
		if (r1 != 0) r = r1 - p.getR(); else r = p.getR();
		if (g1 != 0) g = g1 - p.getG(); else g = p.getG();
		if (b1 != 0) b = b1 - p.getB(); else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class Multiplication : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r, g, b;
		if (r1 != 0) r = r1 * p.getR(); else r = p.getR();
		if (g1 != 0) g = g1 * p.getG(); else g = p.getG();
		if (b1 != 0) b = b1 * p.getB(); else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class Division : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r, g, b;
		if (r1 != 0) r = p.getR() / r1; else r = p.getR();
		if (g1 != 0) g = p.getG() / g1; else g = p.getG();
		if (b1 != 0) b = p.getB() / b1; else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class InverseDivision : public BasicOperation {
public:
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r, g, b;
		if ((r1 != 0) && (p.getR() != 0)) r = r1 / p.getR(); else r = p.getR();
		if ((g1 != 0) && (p.getG() != 0)) g = g1 / p.getG(); else g = p.getG();
		if ((b1 != 0) && (p.getB() != 0)) b = b1 / p.getB(); else b = p.getB();
		p.setAllInt(r, g, b);
		return p;
	}
};

class Power : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r = pow(p.getR(), r1);
		int g = pow(p.getG(), r1);
		int b = pow(p.getB(), r1);
		p.setAllInt(r, g, b);
		return p;
	}
};

class Logarithm : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		double r, g, b;
		if (p.getR() != 0) r = log(p.getR());
		else r = 0;
		if(p.getG() != 0) g = log(p.getG());
		else g = 0;
		if (p.getB() != 0)b = log(p.getB());
		else b = 0;
		p.setAllInt((int)r, (int)g, (int)b);
		return p;
	}
};

class AbsoluteValue : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r = abs((int)p.getR());
		int g = abs((int)p.getG());
		int b = abs((int)p.getB());
		p.setAllInt(r, g, b);
		return p;
	}
};

class Minimum : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r = p.getR();
		int g = p.getG();
		int b = p.getB();
		if (r > r1) r = r1;
		if (g > r1) g = r1;
		if (b > r1) b = r1;
		p.setAllInt(r, g, b);
		return p;
	}
};


class Maximum : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		int r = p.getR();
		int g = p.getG();
		int b = p.getB();
		if (r < r1) r = r1;
		if (g < r1) g = r1;
		if (b < r1) b = r1;
		p.setAllInt(r, g, b);
		return p;
	}
};

class Grayscale : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		double d = (p.getR() + p.getG() + p.getB()) / 3;
		int y = (int)d;
		p.setAllInt(y, y, y);
		return p;
	}
};

class BlackWhite : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		double d = (p.getR() + p.getG() + p.getB()) / 3;
		int y = (int)d;
		if (y < 127) p.setAllInt(0, 0, 0);
		else p.setAllInt(255, 255, 255);
		return p;
	}
};

class Inversion : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		Operation* pom = new InverseSubtraction();
		return pom->execute(p, 255, 0, 0);
	}
};

class AllRed : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		p.setAllInt(255, 0, 0);
		return p;
	}
};

class AllGreen : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		p.setAllInt(0, 255, 0);
		return p;
	}
};




class AllBlue : public BasicOperation {
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override {
		p.setAllInt(0, 0, 255);
		return p;
	}
};

class Layer;

class Mediana : public BasicOperation {
public:
	friend class Layer;
	Layer* l = nullptr;
	Mediana() {
		mediana = true;
	}
	void setLayer(Layer& l1);
	Pixel& execute(Pixel& p, const int r1 = 0, const int g1 = 0, const int b1 = 0) override;
};