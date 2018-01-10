#ifndef APR_LIMITATION_H
#define APR_LIMITATION_H

#include "Matrix.h"

class MinMaxLimitation {
public:
	Matrix l_mMin, l_mMax;
	MinMaxLimitation(Matrix& mMin, Matrix& mMax);
	bool Check(Matrix &mX);
};

class Limitation {
public:
	Limitation() {}
	~Limitation() {}
	virtual bool Check(Matrix &mX) = 0;
	virtual double Calculate(Matrix &mX) = 0;
	virtual double Transform(Matrix &mX, double t) = 0;
};

class Limitation1 : public Limitation {
public:
	virtual bool Check(Matrix &mX);
	virtual double Calculate(Matrix &mX);
	virtual double Transform(Matrix &mX, double t);
};

class Limitation2 : public Limitation {
public:
	virtual bool Check(Matrix &mX);
	virtual double Calculate(Matrix &mX);
	virtual double Transform(Matrix &mX, double t);
};

class Limitation3 : public Limitation {
public:
	virtual bool Check(Matrix &mX);
	virtual double Calculate(Matrix &mX);
	virtual double Transform(Matrix &mX, double t);
};

class Limitation4 : public Limitation {
public:
	virtual bool Check(Matrix &mX);
	virtual double Calculate(Matrix &mX);
	virtual double Transform(Matrix &mX, double t);
};

class Limitation5 : public Limitation {
public:
	virtual bool Check(Matrix &mX);
	virtual double Calculate(Matrix &mX);
	virtual double Transform(Matrix &mX, double t);
};

#endif