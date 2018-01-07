#ifndef APR_FUNCTION_H
#define APR_FUNCTION_H

#include "Matrix.h"
#include "Limitation.h"
#include <vector>

class Function {
public:
	Matrix f_mStartingPoint;
	Matrix f_mDescent;
	int f_iDimension;
	int f_iCounter, f_iGradientCounter, f_iHesseCounter;
public:
	Function(int iDimension);
	virtual double Calculate(Matrix& mParams) = 0;
	virtual Matrix CalculateGradient(Matrix& mParams);
	virtual Matrix CalculateHesse(Matrix& mParams);
	double Calculate(double fParam);
	void SetCoordinateDescent(Matrix& mStartingPoint, int iCoordinate);
};

class Function1 : public Function {
public:
	Function1();
	virtual double Calculate(Matrix& mParams);
	virtual Matrix CalculateGradient(Matrix& mParams);
	virtual Matrix CalculateHesse(Matrix& mParams);
};

class Function2 : public Function {
public:
	Function2();
	virtual double Calculate(Matrix& mParams);
	virtual Matrix CalculateGradient(Matrix& mParams);
	virtual Matrix CalculateHesse(Matrix& mParams);
};

class Function3 : public Function {
public:
	Function3(int iDimension);
	virtual double Calculate(Matrix& mParams);
};

class Function4 : public Function {
public:
	Function4();
	virtual double Calculate(Matrix& mParams);
};

class Function5 : public Function {
public:
	Function5(int iDimension);
	virtual double Calculate(Matrix& mParams);
};

class Function6 : public Function {
public:
	Function6();
	virtual double Calculate(Matrix& mParams);
	virtual Matrix CalculateGradient(Matrix& mParams);
	virtual Matrix CalculateHesse(Matrix& mParams);
};

class Function7 : public Function {
public:
	Function7();
	virtual double Calculate(Matrix& mParams);
	virtual Matrix CalculateGradient(Matrix& mParams);
	virtual Matrix CalculateHesse(Matrix& mParams);
};

class Function8 : public Function {
public:
	Function8(int iDimension);
	virtual double Calculate(Matrix& mParams);
};

class LimitedFunction : public Function {
public:
	Function& f_fFunction;
	std::vector<Limitation*> f_alLimitations;
	double f_fT;
public:
	LimitedFunction(Function& fFunction, std::vector<Limitation*> alLimitations);
	virtual double Calculate(Matrix& mParams);
};

class InnerPointFunction : public Function {
public:
	std::vector<Limitation*> f_alLimitations;
public:
	InnerPointFunction(int iDimension, std::vector<Limitation*> alLimitations);
	virtual double Calculate(Matrix& mParams);
};

#endif