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


/**
*/
class SBRFunctionEvalOp : public EvaluateOp
{
protected:
	const double _capacityPenalty = 10000.0;
	SBR::InstanceLoader* loader;
	SBR::IInstanceCalculator* calculator;
public:
	SBRFunctionEvalOp(SBR::InstanceLoader* loader, SBR::IInstanceCalculator* calculator);
	FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
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
	void PerformSectoring(std::vector<double>& angles, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector);
};
typedef boost::shared_ptr<SBRFunctionEvalOp> SBRFunctionEvalOpP;