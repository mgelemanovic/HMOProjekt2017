#ifndef APR_GENETIC_UNIT_H
#define APR_GENETIC_UNIT_H

#include "Matrix.h"

class GeneticUnit {
public:
	static int gu_iNotableDecimals, gu_iElements;
	static double gu_fMutationChance;
	static bool gu_bAlternateCrossOver;
	static Matrix gu_mMin, gu_mMax;
public:
	double gu_fGoalValue;
	double gu_fFitness;
public:
	GeneticUnit(void) {};
	~GeneticUnit(void) {};
	static void Prepare(int iElements, Matrix& mMin, Matrix& mMax, int iNotableDecimals);
	virtual Matrix Evaluate(void) = 0;
	virtual void CrossOver(GeneticUnit* guFirst, GeneticUnit* guSecond) = 0;
	virtual void Mutate() = 0;
};

class BinaryUnit : public GeneticUnit {
public:
	unsigned long *gu_mUnit;
	static int *gu_aiBits;
public:
	BinaryUnit(void);
	~BinaryUnit(void);
	virtual Matrix Evaluate(void);
	virtual void CrossOver(GeneticUnit* guFirst, GeneticUnit* guSecond);
	virtual void Mutate();
};

class FloatUnit : public GeneticUnit {
public:
	Matrix gu_mUnit;
	static double gu_fMaxMutation;
public:
	FloatUnit(void);
	~FloatUnit(void);
	virtual Matrix Evaluate(void);
	virtual void CrossOver(GeneticUnit* guFirst, GeneticUnit* guSecond);
	virtual void Mutate();
};

#endif