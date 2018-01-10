#include <ECF.h>
#include "SBRFunctionEvalOp.h"

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

	fitness->setValue(value);
	return fitness;
}
