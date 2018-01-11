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
};
typedef boost::shared_ptr<SBRFunctionEvalOp> SBRFunctionEvalOpP;