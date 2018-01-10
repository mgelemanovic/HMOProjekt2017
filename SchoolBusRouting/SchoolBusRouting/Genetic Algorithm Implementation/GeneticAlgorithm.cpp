#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int iTournamentSize/*=3*/) : ga_ulFlags(0), ga_iPopulationSize(0), 
	ga_fMutationChance(0.0), ga_fWindowMin(0), ga_fWindowMax(100), ga_iPrintFrequency(1000), ga_fStopCriteria(1e-6)
{
	// tournament size can't be lower than 3
	if (iTournamentSize < 3) {
		iTournamentSize = 3;
	}
	ga_iTournamentSize = iTournamentSize;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	delete[] ga_aguPopulation;
}

void GeneticAlgorithm::CreatePopulation(int iPopulationSize)
{
	// delete previous population if it exists
	if (ga_aguPopulation != NULL) {
		delete[] ga_aguPopulation;
	}
	ga_iPopulationSize = iPopulationSize;
	ga_aguPopulation = new GeneticUnit*[ga_iPopulationSize];
	for (int i = 0; i < ga_iPopulationSize; ++i) {
		// create new unit
		if (ga_ulFlags & GAF_USE_BINARY_GENETIC_UNIT) {
			ga_aguPopulation[i] = new BinaryUnit();
		} else {
			ga_aguPopulation[i] = new FloatUnit();
		}
		// calculate it's goal value
		ga_aguPopulation[i]->gu_fGoalValue = ga_iGoalFunction->Calculate(ga_aguPopulation[i]->Evaluate());
	}
}

Matrix GeneticAlgorithm::Run(void)
{
	GeneticUnit::gu_bAlternateCrossOver = ga_ulFlags & GAF_ALTERNATE_UNIT_CROSSOVER;
	// set first unit as both best and worst solution
	ga_fBest = ga_fWorst = ga_aguPopulation[0]->gu_fGoalValue;
	ga_guBestUnit = ga_aguPopulation[0];
	// find actual best and worst solution
	for (int i = 1; i < ga_iPopulationSize; ++i) {
		UpdateBestWorstUnits(ga_aguPopulation[i]);
	}
	bool bShouldUpdateAllFitness = true;
	// allocate memory for tournament
	ga_iTournament = new int[ga_iTournamentSize];

	// main algorithm loop
	do {
		// update all fitness values only when worst/best solution changes
		if (bShouldUpdateAllFitness) {
			// calculate fitness of all genetic units
			for (int i = 0; i < ga_iPopulationSize; ++i) {
				UpdateFitness(ga_aguPopulation[i]);
			}
			bShouldUpdateAllFitness = false;
		}
		// pick random units for tournament (don't repeat units)
		int iCounter = 0;
		while (iCounter < ga_iTournamentSize) {
			int iNewCandidate = rand() % ga_iPopulationSize;
			bool bNotUsed = true;
			for (int i = 0; i < iCounter; ++i) {
				if (ga_iTournament[i] == iNewCandidate) {
					bNotUsed = false;
					break;
				}
			}
			if (bNotUsed) {
				ga_iTournament[iCounter] = iNewCandidate;
				++iCounter;
			}
		}
		// determine the worst candidate in the tournament
		int iWorstCandidate = 0;
		for (int i = 1; i < ga_iTournamentSize; ++i) {
			if (ga_aguPopulation[ga_iTournament[i]]->gu_fFitness < ga_aguPopulation[ga_iTournament[iWorstCandidate]]->gu_fFitness) {
				iWorstCandidate = i;
			}
		}
		// pick two parents from the rest of tournament candidates
		int iFirstParent = 0, iSecondParent = 0;
		// roulette selection when tournament size is bigger than 3
		if (ga_iTournamentSize > 3) {
			double fTotalFitnes = 0.0;
			// sum up fitness of all possible parents
			for (int i = 0; i < ga_iTournamentSize; ++i) {
				if (i != iWorstCandidate) {
					fTotalFitnes += ga_aguPopulation[ga_iTournament[i]]->gu_fFitness;
				}
			}
			// chose parents
			double fR1 = (rand() % (1000 + 1)) / 1000.0, fR2 = (rand() % (1000 + 1)) / 1000.0;
			double fA = 0.0, fB = 0.0;
			for (int i = 0; i < ga_iTournamentSize; ++i) {
				if (i != iWorstCandidate) {
					fB += ga_aguPopulation[ga_iTournament[i]]->gu_fFitness / fTotalFitnes;
					// first parent
					if (fA <= fR1 && fR1 <= fB) {
						iFirstParent = i;
					}
					// second parent
					if (fA <= fR2 && fR2 <= fB) {
						iSecondParent = i;
					}
					fA = fB;
				}
			}
		}
		else {
			iFirstParent = iWorstCandidate == 0 ? 2 : 0;
			iSecondParent = iWorstCandidate == 1 ? 2 : 1;
		}

		// get indicises for the worst unit and two parents that will create a new unit
		int iWorstIndex, iFirstParentIndex, iSecondParentIndex;
		iWorstIndex = ga_iTournament[iWorstCandidate];
		iFirstParentIndex = ga_iTournament[iFirstParent];
		iSecondParentIndex = ga_iTournament[iSecondParent];

		// apply genetic operators to create a new unit
		GeneticUnit *pguNewUnit;
		pguNewUnit = ga_aguPopulation[iWorstIndex];
		pguNewUnit->CrossOver(ga_aguPopulation[iFirstParentIndex], ga_aguPopulation[iSecondParentIndex]);
		pguNewUnit->Mutate();

		// update goal value for this unit
		pguNewUnit->gu_fGoalValue = ga_iGoalFunction->Calculate(pguNewUnit->Evaluate());
		// check if best/worst solution changed
		bShouldUpdateAllFitness = UpdateBestWorstUnits(pguNewUnit);
		// if it didn't, we can just update fitness for this new unit
		// otherwise, we'll update all units next iteration of the loop
		if (!bShouldUpdateAllFitness) {
			UpdateFitness(pguNewUnit);
		}

		if (ga_iGoalFunction->f_iCounter % ga_iPrintFrequency == 0) {
			std::cout << ga_iGoalFunction->f_iCounter << "th Evaluation, best solution: { " << ga_guBestUnit->Evaluate() <<  "}, value = " << ga_fBest << std::endl;
		}
	} while (!StopCriteriaMet());

	// deallocate memory for tournament
	delete[] ga_iTournament;
	// return best unit, evaluated to Matrix form
	return ga_guBestUnit->Evaluate();
}

bool GeneticAlgorithm::UpdateBestWorstUnits(GeneticUnit* pgu)
{
	// get goal value for this genetic unit
	double fGoalValue = pgu->gu_fGoalValue;
	if (ga_ulFlags & GAF_MAXIMIZE_GOAL_FUNCTION) {
		// check if unit has smaller goal value than the previous worst
		if (fGoalValue < ga_fWorst) {
			ga_fWorst = fGoalValue;
			return true;
		}
		// check if unit has larger goal value than the previous best
		if (fGoalValue > ga_fBest) {
			ga_fBest = fGoalValue;
			ga_guBestUnit = pgu;
			return true;
		}
	}
	else {
		// check if unit has larger goal value than the previous worst
		if (fGoalValue > ga_fWorst) {
			ga_fWorst = fGoalValue;
			return true;
		}
		// check if unit has smaller goal value than the previous best
		if (fGoalValue < ga_fBest) {
			ga_fBest = fGoalValue;
			ga_guBestUnit = pgu;
			return true;
		}
	}
	// if we got here, this unit is neither worst/best so signal that fitness shouldn't be calculated for all units
	return false;
}

void GeneticAlgorithm::UpdateFitness(GeneticUnit* pgu)
{
	if (ga_ulFlags & GAF_FITNESS_WINDOWING) {
		// use windowing method
		double fRatio = (pgu->gu_fGoalValue - ga_fWorst) / (ga_fBest - ga_fWorst);
		pgu->gu_fFitness = ga_fWindowMin + (ga_fWindowMax - ga_fWindowMin) * fRatio;
		return;
	} else {
		// use default method
		pgu->gu_fFitness = (ga_ulFlags & GAF_MAXIMIZE_GOAL_FUNCTION ? -1.0 : 1.0) * (ga_fWorst - pgu->gu_fGoalValue);
	}
}

bool GeneticAlgorithm::StopCriteriaMet(void)
{
	// was goal function evaluated too many times
	bool bMaxEvaluationReached = ga_iGoalFunction->f_iCounter >= ga_iMaxEvaluations;
	// is the best found value lower (or higher) than some given limit
	bool bValueReached = ga_fBest < ga_fStopCriteria;
	if (ga_ulFlags & GAF_MAXIMIZE_GOAL_FUNCTION) {
		bValueReached = ga_fBest > ga_fStopCriteria;
	}
	return bMaxEvaluationReached || bValueReached;
}
