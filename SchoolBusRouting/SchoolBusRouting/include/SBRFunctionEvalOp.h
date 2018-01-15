#pragma once

#include <cmath>
#include "ECF.h"
#include "InstanceLoader.h"
#include "IInstanceCalculator.h"

/**
* \par Description
* Tries to find the (global) minimum of bus routing cost function.
* The example uses FloatingPoint genotype.
*
* \par Parameters (in the config file)
* - with FloatingPoint genotype, a number of dimensions and the interval [lbound, ubound] must be set
*/

class SectorManager
{
private:
	SBR::InstanceLoader* loader;
public:
	SectorManager(SBR::InstanceLoader* loader) : loader(loader) {}
	void PerformSectoring(std::vector<double>& angles, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector);
};

/**
*/
class SBRFunctionEvalOp : public EvaluateOp
{
protected:
	const double _capacityPenalty = 1e12;
	SBR::InstanceLoader* loader;
	SBR::IInstanceCalculator* calculator;
	SectorManager manager;
public:
	SBRFunctionEvalOp(SBR::InstanceLoader* loader, SBR::IInstanceCalculator* calculator);
	FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
	static std::vector<double> GenomeToAngles(FloatingPointP gen);
private:
	double evaluate_internal(std::vector<double> angles);
	/**
	*   \brief Method that performs sectoring.
	*
	*   Method fills both position vectors based on sectors determined by angles.
	*
	*   \param studentsBySector reference to a vector of students that is to be filled.
	*   \param busStopsBySector reference to a vector of bus stops that is to be filled.
	*
	**/
	//void PerformSectoring(std::vector<double>& angles, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector);
};
typedef boost::shared_ptr<SBRFunctionEvalOp> SBRFunctionEvalOpP;