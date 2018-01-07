#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "Matrix.h"
#include "Function.h"
#include "GeneticAlgorithm.h"

int main(int argc, char *argv[])
{
	// check if arguments were given
	if (argc != 2) {
		std::cout << "Command line arguments error: file missing." << std::endl;
		return 1;
	}

	// check if file can be opened
	std::ifstream ifs(argv[1]);
	if (!ifs) {
		std::cout << "Error opening file." << std::endl;
		return 1;
	}

	// which function are we optimizing and what dimension it is
	int iFunctionNumber, iSize = 0;
	Function* pf = nullptr;
	// limitations
	Matrix mMin, mMax;
	// params for genetic algorithm and genetic units
	unsigned long ulFlags = 0;
	int iNotableDecimals = 2, iMaxEvaluations = 10000, iMaxPopulation = 50, iTournamentSize = 3, iPrintFreq = 1000;
	double fWindowMin = 0.0, fWindowMax = 100.0, fMutationChance = 0.015, fMaxMutation = 5.0, fStopCriteria = 1e-6;
	// initialize random number generator
	srand((unsigned int)time(NULL));

	// read input params
	{
		// input data
		std::string strInput;
		while (ifs) {
			ifs >> strInput;
			switch (strInput[0]) {
			case 'f':	// function:
				// check if function was already created
				if (pf) {
					break;
				}
				ifs >> iFunctionNumber;
				ifs >> iSize;
				// resize matrices now that we know their sizes
				mMin.Resize(1, iSize);
				mMax.Resize(1, iSize);
				// create new function object based on function number
				switch (iFunctionNumber) {
				case 1:
					pf = new Function1();
					break;
				case 2:
					pf = new Function2();
					break;
				case 3:
					pf = new Function3(iSize);
					break;
				case 4:
					pf = new Function4();
					break;
				case 5:
					pf = new Function5(iSize);
					break;
				case 6:
					pf = new Function6();
					break;
				case 7:
					pf = new Function7();
					break;
				case 8:
					pf = new Function8(iSize);
					break;
				default:
					break;
				}
				break;
			case 'l':	// limitations:
				for (int i = 1; i <= iSize; ++i) {
					ifs >> mMin(i);
				}
				for (int i = 1; i <= iSize; ++i) {
					ifs >> mMax(i);
				}
				break;
			case 'w':	// windowing_params:
				ulFlags |= GAF_FITNESS_WINDOWING;
				ifs >> fWindowMin;
				ifs >> fWindowMax;
				break;
			case 'm':	// mutation_params:
				ifs >> fMutationChance;
				ifs >> fMaxMutation;
				break;
			case 'n':	// notable_decimals:
				ifs >> iNotableDecimals;
				break;
			case 'p':	// population_size:
				ifs >> iMaxPopulation;
				break;
			case 'e':	// evaluation_criteria:
				ifs >> iMaxEvaluations;
				break;
			case 't':	// tournament_size:
				ifs >> iTournamentSize;
				break;
			case 'a':	// alternate_crossover
				ulFlags |= GAF_ALTERNATE_UNIT_CROSSOVER;
				break;
			case 'b':	// binary_genetic_unit
				ulFlags |= GAF_USE_BINARY_GENETIC_UNIT;
				break;
			case 'o':	// optimization_type:
				ifs >> strInput;
				if (!strncmp(strInput.c_str(), "max", 3)) {
					ulFlags |= GAF_MAXIMIZE_GOAL_FUNCTION;
				}
				break;
			case 's':	// stop_criteria:
				ifs >> fStopCriteria;
				break;
			case 'c':
				ifs >> iPrintFreq;
				break;
			default:
				// unknown input, ignore this line
				std::getline(ifs, strInput);
				break;
			}
		}
	}

	// setup genetic unit params
	GeneticUnit::Prepare(iSize, mMin, mMax, iNotableDecimals);
	GeneticUnit::gu_fMutationChance = fMutationChance;
	FloatUnit::gu_fMaxMutation = fMaxMutation;

	// setup genetic algorithm params
	GeneticAlgorithm ga(iTournamentSize);
	ga.ga_ulFlags = ulFlags;
	ga.ga_fWindowMin = fWindowMin;
	ga.ga_fWindowMax = fWindowMax;
	ga.ga_iGoalFunction = pf;
	ga.ga_iMaxEvaluations = iMaxEvaluations;
	ga.ga_fStopCriteria = fStopCriteria;
	ga.ga_iPrintFrequency = iPrintFreq;

	int iHits = 0;
	int iAttempts = 100;
	for (int i = 0; i < iAttempts; ++i) {
		// create initial population and run the algorithm
		ga.ga_iGoalFunction->f_iCounter = 0;
		ga.CreatePopulation(iMaxPopulation);
		Matrix mBestSolution = ga.Run();
		double fBestValue = ga.ga_fBest;

		if (fBestValue < fStopCriteria) {
			++iHits;
		}

		std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "Genetic algorithm found solution in " << ga.ga_iGoalFunction->f_iCounter << " function evaluations" << std::endl;
		std::cout << "Found solution: { " << mBestSolution << "}" << std::endl;
		std::cout << "Goal function value for found solution: " << fBestValue << std::endl;
		std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	}

	std::cout << "Correct solution was found " << iHits << "/" << iAttempts << "(" << (100.0f * iHits) / iAttempts << "%) times" << std::endl;
	return 0;
}