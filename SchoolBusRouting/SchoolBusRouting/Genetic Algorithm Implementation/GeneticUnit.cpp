#include "GeneticUnit.h"
#include <stdlib.h>

int GeneticUnit::gu_iElements = 0;
int GeneticUnit::gu_iNotableDecimals = 0;
double GeneticUnit::gu_fMutationChance = 0.015;
bool GeneticUnit::gu_bAlternateCrossOver = false;
Matrix GeneticUnit::gu_mMin;
Matrix GeneticUnit::gu_mMax;

void GeneticUnit::Prepare(int iElements, Matrix & mMin, Matrix & mMax, int iNotableDecimals)
{
	gu_iElements = iElements;
	gu_mMin = mMin;
	gu_mMax = mMax;
	gu_iNotableDecimals = iNotableDecimals;
}

int* BinaryUnit::gu_aiBits = NULL;

BinaryUnit::BinaryUnit(void)
{
	gu_mUnit = new unsigned long[gu_iElements];
	bool bCreatedBits = gu_aiBits != NULL;
	if (!bCreatedBits) {
		gu_aiBits = new int[gu_iElements];
	}
	for (int i = 0; i < gu_iElements; ++i) {
		if (!bCreatedBits) {
			unsigned long iN = (unsigned long)ceil(log(1.0 + (gu_mMax(i + 1) - gu_mMin(i + 1)) * pow(10.0, gu_iNotableDecimals)) / log(2.0));
			gu_aiBits[i] = iN;
		}
		unsigned long iValue = (rand() * rand()) % (unsigned long)pow(2, gu_aiBits[i]);
		gu_mUnit[i] = iValue;
	}
}

BinaryUnit::~BinaryUnit(void)
{
	delete[] gu_mUnit;
}

Matrix BinaryUnit::Evaluate(void)
{
	Matrix mValue(1, gu_iElements);
	for (int i = 1; i <= gu_iElements; ++i) {
		double fValue = gu_mMin(i) + (gu_mMax(i) - gu_mMin(i)) * gu_mUnit[i-1] / (pow(2, gu_aiBits[i-1]) - 1);
		mValue(i) = fValue;
	}
	return mValue;
}

void BinaryUnit::CrossOver(GeneticUnit* guFirst, GeneticUnit* guSecond)
{
	for (int i = 0; i < gu_iElements; ++i) {
		if (gu_bAlternateCrossOver) {
			unsigned long iR = (rand() * rand()) % (unsigned long)pow(2, gu_aiBits[i]);
			unsigned long iA = ((BinaryUnit*)guFirst)->gu_mUnit[i];
			unsigned long iB = ((BinaryUnit*)guSecond)->gu_mUnit[i];
			gu_mUnit[i] = (iA & iB) | iR & (iA ^ iB);
		}
		else {
			int iSwitcher = rand() % (gu_aiBits[i] - 2) + 1;
			for (int j = 0; j < gu_aiBits[i]; ++j) {
				BinaryUnit* pguParent = (BinaryUnit*)(j < iSwitcher ? guFirst : guSecond);
				unsigned long iFlag = (1UL << j);
				if (pguParent->gu_mUnit[i] & iFlag) {
					gu_mUnit[i] |= iFlag;
				}
				else {
					gu_mUnit[i] &= ~iFlag;
				}
			}
		}
	}
}

void BinaryUnit::Mutate()
{
	for (int i = 0; i < gu_iElements; ++i) {
		double fR = (rand() % (1000 + 1)) / 1000.0;
		if (fR < gu_fMutationChance) {
			gu_mUnit[i] ^= (1UL << rand() % gu_aiBits[i]);
		}
	}
}

double  FloatUnit::gu_fMaxMutation = 5.0;

FloatUnit::FloatUnit(void) : gu_mUnit(1, gu_iElements)
{
	for (int i = 1; i <= gu_iElements; ++i) {
		double fValue = gu_mMin(i);
		fValue += rand() % (int)trunc(gu_mMax(i) - gu_mMin(i));
		fValue += (rand() % (int)pow(10, gu_iNotableDecimals)) / pow(10, gu_iNotableDecimals);
		gu_mUnit(i) = fValue;
	}
}

FloatUnit::~FloatUnit(void)
{
}

Matrix FloatUnit::Evaluate(void)
{
	return gu_mUnit;
}

void FloatUnit::CrossOver(GeneticUnit* guFirst, GeneticUnit* guSecond)
{
	for (int i = 1; i <= gu_iElements; ++i) {
		double fX1 = ((FloatUnit*)guFirst)->gu_mUnit(i);
		double fX2 = ((FloatUnit*)guSecond)->gu_mUnit(i);
		if (gu_bAlternateCrossOver) {
			// X2 is the better unit, the one with bigger fitness value
			if (guFirst->gu_fFitness > guSecond->gu_fFitness) {
				double fTmp = fX1;
				fX1 = fX2;
				fX2 = fTmp;
			}
			bool bDone = false;
			int iCounter = 0;
			double fNewValue;
			// generate values until it satisfies all limitations
			while (!bDone) {
				double fR = (rand() % (1000 + 1)) / 1000.0;
				fNewValue = fR * (fX2 - fX1) + fX2;
				if (fNewValue >= gu_mMin(i) && fNewValue <= gu_mMax(i)) {
					bDone = true;
				}
				else {
					// after 100 attempts, just clamp the value to one of the limits
					++iCounter;
					if (iCounter == 100) {
						if (fNewValue < gu_mMin(i)) {
							fNewValue = gu_mMin(i);
						} else {
							fNewValue = gu_mMax(i);
						}
						bDone = true;
					}
				}
			}
			gu_mUnit(i) = fNewValue;
		}
		else {
			// random lerp between parents' values
			double fR = (rand() % 1001) / 1000.0;
			gu_mUnit(i) = (1.0 - fR) * fX1 + fR * fX2;
		}
	}
}

void FloatUnit::Mutate()
{
	for (int i = 1; i <= gu_iElements; ++i) {
		double fR = (rand() % (1000 + 1)) / 1000.0;
		if (fR < gu_fMutationChance) {
			double fR = (rand() % 1001) / 1000.0 * ((rand() % 2 == 0) ? 1.0 : -1.0);
			gu_mUnit(i) += fR * gu_fMaxMutation;
			// clamp value if it goes outside limitations
			if (gu_mUnit(i) < gu_mMin(i)) {
				gu_mUnit(i) = gu_mMin(i);
			}
			else if(gu_mUnit(i) > gu_mMax(i)) {
				gu_mUnit(i) = gu_mMax(i);
			}
		}
	}
}
