#include <ECF.h>
#include "SBRFunctionEvalOp.h"

#include <boost\math\constants\constants.hpp>
#include <numeric>

void SBRFunctionEvalOp::registerParameters(StateP state)
{
	
}


bool SBRFunctionEvalOp::initialize(StateP state)
{
	return true;
}


FitnessP SBRFunctionEvalOp::evaluate(IndividualP individual)
{
	FitnessP fitness(new FitnessMin);

	FloatingPointP gen = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (individual->getGenotype());
	
	// the number of variables is read from the genotype itself (size of 'realValue' vector)
	double realTemp = 0, value = 0;

	for (uint i = 0; i < gen->realValue.size(); i++) {
		realTemp = gen->realValue[i];
		value += realTemp;
	}

	double valueSum = value;
	std::vector<double> angles;
	for (uint i = 0; i < gen->realValue.size(); ++i)
	{
		realTemp = gen->realValue[i];
		double angle = realTemp / valueSum * 2 * boost::math::constants::pi<double>();
		angles.push_back(angle);
	}

	value = evaluate_internal(angles);

	fitness->setValue(value);
	return fitness;
}

/*
* This is the function which needs to be implemented to evaluate bus routes based on given angles.
*/
double SBRFunctionEvalOp::evaluate_internal(std::vector<double> angles)
{
	return std::accumulate(angles.begin(), angles.end(), 0.0);
}
