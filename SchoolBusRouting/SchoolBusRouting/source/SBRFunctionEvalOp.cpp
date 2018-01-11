#include <ECF.h>
#include "SBRFunctionEvalOp.h"

#include <boost\math\constants\constants.hpp>
#include <numeric>

SBRFunctionEvalOp::SBRFunctionEvalOp(SBR::InstanceLoader* loader, SBR::IInstanceCalculator* calculator)
	:
	loader(loader),
	calculator(calculator)
{
	
}

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
	std::vector<std::vector<SBR::Position>> studentsBySector;
	std::vector<std::vector<SBR::Position>> busStopsBySector;

	const std::vector<SBR::Position>& stopPositions = loader->GetStopPositions();
	const std::vector<SBR::Position>& studentPositions = loader->GetStudentPositions();
	
	for (int i = 0; i < angles.size(); ++i)
	{
		std::vector<SBR::Position> vec;
		// add empty student vec
		studentsBySector.push_back(vec);
		// add school to bus routes
		vec.push_back(stopPositions[0]);
		busStopsBySector.push_back(vec);
	}

	{
		double angleSum = 0.0;
		int idxAngle = -1;

		int idxStop = 1;
		while (idxStop < stopPositions.size())
		{
			const SBR::Position& busStop = stopPositions[idxStop];
			if (angleSum < busStop.polarAngle)
			{
				double angle = angles[++idxAngle];
				angleSum += angle;
				continue;
			}
			busStopsBySector[idxAngle].push_back(busStop);
			idxStop++;
		}
	}

	{
		double angleSum = 0.0;
		int idxAngle = -1;

		int idxStudent = 0;
		while (idxStudent < studentPositions.size())
		{
			const SBR::Position& studentStop = studentPositions[idxStudent];
			if (angleSum < studentStop.polarAngle)
			{
				double angle = angles[++idxAngle];
				angleSum += angle;
				continue;
			}
			studentsBySector[idxAngle].push_back(studentStop);
			idxStudent++;
		}
	}

	double routingCost = calculator->CalculateRoutingCost(studentsBySector, busStopsBySector);

	double penalty = 0.0;

	int busCapacity = loader->GetCapacity();

	for (int i = 0; i < studentsBySector.size(); ++i)
	{
		int cntStudents = studentsBySector[i].size();
		penalty += (cntStudents > busCapacity ? ((cntStudents - busCapacity) * _capacityPenalty) : 0.0);
	}

	return penalty + routingCost;
}
