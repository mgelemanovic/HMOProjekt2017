#include "Function.h"

#include <cmath>

Function::Function(int iDimension) : f_iCounter(0), f_iGradientCounter(0), f_iHesseCounter(0),
	f_iDimension(iDimension), f_mStartingPoint(1, iDimension), f_mDescent(1, iDimension)
{
}

Matrix Function::CalculateGradient(Matrix & mParams)
{
	// gradient calculation not implemented
	_ASSERT(0);
	return Matrix();
}

Matrix Function::CalculateHesse(Matrix & mParams)
{
	// Hesse calculation not implemented
	_ASSERT(0);
	return Matrix();
}

double Function::Calculate(double fParam)
{
	return Calculate(f_mStartingPoint + fParam * f_mDescent);
}

void Function::SetCoordinateDescent(Matrix& mStartingPoint, int iCoordinate)
{
	_ASSERT(mStartingPoint.m_iCol == f_iDimension && iCoordinate < f_iDimension);

	f_mStartingPoint = mStartingPoint;
	for (int i = 0; i < f_iDimension; ++i) {
		f_mDescent(1, i + 1) = (iCoordinate == i) ? 1 : 0;
	}
}

Function1::Function1() : Function(2) 
{
}

double Function1::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	return 100.0 * pow((x2 - x1 * x1), 2.0) + pow((1.0 - x1), 2.0);
}

Matrix Function1::CalculateGradient(Matrix & mParams)
{
	Matrix mGradient(1, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	mGradient(1, 1) = -400.0 * x1 * (x2 - x1 * x1) - 2.0 * (1 - x1);
	mGradient(1, 2) = 200.0 * (x2 - x1 * x1);
	f_mDescent = mGradient;
	f_mStartingPoint = mParams;
	++f_iGradientCounter;
	return mGradient;
}

Matrix Function1::CalculateHesse(Matrix & mParams)
{
	Matrix H = Matrix(2, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	H(1, 1) = -400.0 * (x2 - 3.0 * x1 * x1) + 2.0;
	H(1, 2) = H(2, 1) = -400.0 * x1;
	H(2, 2) = 200.0;
	++f_iHesseCounter;
	return H;
}

Function2::Function2() : Function(2)
{
}

double Function2::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	return pow((x1 - 4.0), 2.0) + 4.0 * pow((x2 - 2.0), 2.0);
}

Matrix Function2::CalculateGradient(Matrix & mParams)
{
	Matrix mGradient(1, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	mGradient(1, 1) = 2.0 * (x1 - 4.0);
	mGradient(1, 2) = 8.0 * (x2 - 2.0);
	f_mDescent = mGradient;
	f_mStartingPoint = mParams;
	++f_iGradientCounter;
	return mGradient;
}

Matrix Function2::CalculateHesse(Matrix & mParams)
{
	Matrix H = Matrix(2, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	H(1, 1) = 2.0;
	H(2, 2) = 8.0;
	++f_iHesseCounter;
	return H;
}

Function3::Function3(int iDimension) : Function(iDimension)
{
}

double Function3::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double fSum = 0.0;
	for (int i = 0; i < f_iDimension; ++i) {
		fSum += pow((mParams(1, i + 1) - (i + 1)), 2.0);
	}
	return fSum;
}

Function4::Function4() : Function(2)
{
}

double Function4::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	return fabs((x1 - x2) * (x1 + x2)) + sqrt(x1 * x1 + x2 * x2);
}

Function5::Function5(int iDimension) : Function(iDimension)
{
}

double Function5::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double fSum = 0.0;
	for (int i = 1; i <= f_iDimension; ++i) {
		fSum += pow((mParams(1, i)), 2.0);
	}
	return 0.5 + (pow(sin(sqrt(fSum)), 2.0) - 0.5) / pow(1.0 + 0.001 * fSum, 2.0);
}

Function6::Function6() : Function(2)
{
}

double Function6::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	return pow((x1 - 2.0), 2.0) + pow((x2 + 3.0), 2.0);
}

Matrix Function6::CalculateGradient(Matrix & mParams)
{
	Matrix mGradient(1, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	mGradient(1, 1) = 2.0 * (x1 - 2.0);
	mGradient(1, 2) = 2.0 * (x2 + 3.0);
	f_mDescent = mGradient;
	f_mStartingPoint = mParams;
	++f_iGradientCounter;
	return mGradient;
}

Matrix Function6::CalculateHesse(Matrix & mParams)
{
	Matrix H = Matrix(2, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	H(1, 1) = 2.0;
	H(2, 2) = 2.0;
	++f_iHesseCounter;
	return H;
}

Function7::Function7() : Function(2)
{
}

double Function7::Calculate(Matrix& mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	return pow((x1 - 3.0), 2.0) + pow(x2, 2.0);
}

Matrix Function7::CalculateGradient(Matrix & mParams)
{
	Matrix mGradient(1, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	mGradient(1, 1) = 2.0 * (x1 - 3.0);
	mGradient(1, 2) = 2.0 * x2;
	f_mDescent = mGradient;
	f_mStartingPoint = mParams;
	++f_iGradientCounter;
	return mGradient;
}

Matrix Function7::CalculateHesse(Matrix & mParams)
{
	Matrix H = Matrix(2, 2);
	double x1 = mParams(1, 1), x2 = mParams(1, 2);
	H(1, 1) = 2.0;
	H(2, 2) = 2.0;
	++f_iHesseCounter;
	return H;
}

Function8::Function8(int iDimension) : Function(iDimension)
{
}

double Function8::Calculate(Matrix & mParams)
{
	_ASSERT(mParams.m_iCol == f_iDimension);
	++f_iCounter;
	double fSum = 0.0;
	for (int i = 1; i <= f_iDimension; ++i) {
		fSum += pow((mParams(1, i)), 2.0);
	}
	return pow(fSum, 0.25) * (1.0 + pow(sin(50.0 * pow(fSum, 0.1)), 2));
}

LimitedFunction::LimitedFunction(Function & fFunction, std::vector<Limitation*> alLimitations) : f_fT(1.0), f_fFunction(fFunction), f_alLimitations(alLimitations), Function(fFunction.f_iDimension)
{
}

double LimitedFunction::Calculate(Matrix & mParams)
{
	double sum = f_fFunction.Calculate(mParams);
	for (unsigned int i = 0; i < f_alLimitations.size(); ++i) {
		sum += f_alLimitations[i]->Transform(mParams, f_fT);
	}
	return sum;
}

InnerPointFunction::InnerPointFunction(int iDimension, std::vector<Limitation*> alLimitations) : f_alLimitations(alLimitations), Function(iDimension)
{
}

double InnerPointFunction::Calculate(Matrix & mParams)
{
	double fSum = 0.0;
	for (unsigned int i = 0; i < f_alLimitations.size(); ++i) {
		double fG = f_alLimitations[i]->Calculate(mParams);
		if (fG < 0.0) {
			fSum -= fG;
		}
	}
	return fSum;
}