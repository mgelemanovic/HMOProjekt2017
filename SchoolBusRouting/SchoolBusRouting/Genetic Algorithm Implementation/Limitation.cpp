#include "Limitation.h"

MinMaxLimitation::MinMaxLimitation(Matrix& mMin, Matrix& mMax) : l_mMin(mMin), l_mMax(mMax)
{
}

bool MinMaxLimitation::Check(Matrix & mX)
{
	for (int i = 1; i <= mX.m_iRow; ++i) {
		for (int j = 1; j <= mX.m_iCol; ++j) {
			if (!(mX(i, j) >= l_mMin(i, j) && mX(i, j) <= l_mMax(i, j))) {
				return false;
			}
		}
	}
	return true;
}

bool Limitation1::Check(Matrix& mX)
{
	return Calculate(mX) >= 0.0;
}

double Limitation1::Calculate(Matrix& mX)
{
	double x1 = mX(1, 1), x2 = mX(1, 2);
	return x2 - x1;
}

double Limitation1::Transform(Matrix& mX, double t)
{
	double x = Calculate(mX);
	if (x <= 0.0) {
		return 1e100;
	}
	return - log(x) / t;
}

bool Limitation2::Check(Matrix& mX)
{
	return Calculate(mX) >= 0.0;
}

double Limitation2::Calculate(Matrix& mX)
{
	double x1 = mX(1, 1), x2 = mX(1, 2);
	return 2.0 - x1;
}

double Limitation2::Transform(Matrix& mX, double t)
{
	double x = Calculate(mX);
	if (x <= 0.0) {
		return 1e100;
	}
	return -log(x) / t;
}

bool Limitation3::Check(Matrix& mX)
{
	return Calculate(mX) >= 0.0;
}

double Limitation3::Calculate(Matrix& mX)
{
	double x1 = mX(1, 1), x2 = mX(1, 2);
	return 3.0 - x1 - x2;
}

double Limitation3::Transform(Matrix& mX, double t)
{
	double x = Calculate(mX);
	if (x <= 0.0) {
		return 1e100;
	}
	return -log(x) / t;
}

bool Limitation4::Check(Matrix& mX)
{
	return Calculate(mX) >= 0.0;
}

double Limitation4::Calculate(Matrix& mX)
{
	double x1 = mX(1, 1), x2 = mX(1, 2);
	return 3.0 + 1.5 * x1 - x2;
}

double Limitation4::Transform(Matrix& mX, double t)
{
	double x = Calculate(mX);
	if (x <= 0.0) {
		return 1e100;
	}
	return -log(x) / t;
}

bool Limitation5::Check(Matrix& mX)
{
	return Calculate(mX) == 0.0;
}

double Limitation5::Calculate(Matrix& mX)
{
	double x1 = mX(1, 1), x2 = mX(1, 2);
	return x2 - 1;
}

double Limitation5::Transform(Matrix& mX, double t)
{
	double x = Calculate(mX);
	return t * pow(x, 2);
}
