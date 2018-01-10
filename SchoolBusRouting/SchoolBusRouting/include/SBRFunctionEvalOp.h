#pragma once

#include <cmath>
#include "ECF.h"

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
	
public:
	FitnessP evaluate(IndividualP individual);
	void registerParameters(StateP);
	bool initialize(StateP);
};
typedef boost::shared_ptr<SBRFunctionEvalOp> SBRFunctionEvalOpP;