#ifndef APR_GENETIC_ALGORITHM_H
#define APR_GENETIC_ALGORITHM_H

#include "Matrix.h"
#include "Function.h"
#include "GeneticUnit.h"

enum GAFlags {
	GAF_MAXIMIZE_GOAL_FUNCTION = (1UL << 0),
	GAF_USE_BINARY_GENETIC_UNIT = (1UL << 1),
	GAF_FITNESS_WINDOWING = (1UL << 2),
	GAF_ALTERNATE_UNIT_CROSSOVER = (1UL << 3),
};

class GeneticAlgorithm {
public:
	unsigned long ga_ulFlags;
	GeneticUnit** ga_aguPopulation;
	GeneticUnit* ga_guBestUnit;
	Function* ga_iGoalFunction;
	int ga_iPopulationSize;
	int ga_iMaxEvaluations;
	int ga_iTournamentSize;
	int* ga_iTournament;
	int ga_iPrintFrequency;
	double ga_fWindowMin;
	double ga_fWindowMax;
	double ga_fBest;
	double ga_fWorst;
	double ga_fMutationChance;
	double ga_fStopCriteria;
public:
	GeneticAlgorithm(int iTournamentSize=3);
	~GeneticAlgorithm();
	void CreatePopulation(int iPopulationSize);
	Matrix Run(void);
	bool UpdateBestWorstUnits(GeneticUnit* pgu);
	void UpdateFitness(GeneticUnit* pgu);
	bool StopCriteriaMet(void);
};

#endif