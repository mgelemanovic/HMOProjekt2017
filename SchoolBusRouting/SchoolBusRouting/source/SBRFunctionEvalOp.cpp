#include <ECF.h>
#include "SBRFunctionEvalOp.h"

#include <boost\math\constants\constants.hpp>
#include <numeric>

SBRFunctionEvalOp::SBRFunctionEvalOp(SBR::InstanceLoader* loader, SBR::IInstanceCalculator* calculator)
	:
	loader(loader),
	calculator(calculator),
	manager(loader)
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
	std::vector<double> angles = SBRFunctionEvalOp::GenomeToAngles(gen);

	double value = evaluate_internal(angles);

	fitness->setValue(value);
	return fitness;
}

double SBRFunctionEvalOp::evaluate_internal(std::vector<double> angles)
{
	std::vector<std::vector<int>> studentsBySector;
	std::vector<std::vector<int>> busStopsBySector;

	manager.PerformSectoring(angles, studentsBySector, busStopsBySector);

	return calculator->CalculateRoutingCost(loader, studentsBySector, busStopsBySector);
}

void SectorManager::PerformSectoring(std::vector<double>& angles, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector)
{
	const std::vector<SBR::Position>& stopPositions = loader->GetStopPositions();
	const std::vector<SBR::Position>& studentPositions = loader->GetStudentPositions();
	const std::vector<int>& studentPositionIndicesSorted = loader->GetStudentPositionIndicesSorted();
	const std::vector<int>& stopPositionIndicesSorted = loader->GetStopPositionIndicesSorted();

	for (int i = 0; i < angles.size(); ++i)
	{
		std::vector<int> vec;
		// add empty student vec
		studentsBySector.push_back(vec);
		// add school to bus routes
		vec.push_back(0);
		busStopsBySector.push_back(vec);
	}

	{
		double angleSum = 0.0;
		int idxAngle = -1;

		int idxStop = 1;
		while (idxStop < stopPositions.size())
		{
			int idxCurStop = stopPositionIndicesSorted[idxStop];
			const SBR::Position& busStop = stopPositions[idxCurStop];
			if (angleSum < busStop.polarAngle)
			{
				double angle = angles[++idxAngle];
				angleSum += angle;
				continue;
			}
			busStopsBySector[idxAngle].push_back(idxCurStop);
			idxStop++;
		}
	}

	{
		double angleSum = 0.0;
		int idxAngle = -1;

		int idxStudent = 0;
		while (idxStudent < studentPositions.size())
		{
			int idxCurStudent = studentPositionIndicesSorted[idxStudent];
			const SBR::Position& studentStop = studentPositions[idxCurStudent];
			if (angleSum < studentStop.polarAngle)
			{
				double angle = angles[++idxAngle];
				angleSum += angle;
				continue;
			}
			studentsBySector[idxAngle].push_back(idxCurStudent);
			idxStudent++;
		}
	}

}

std::vector<double> SBRFunctionEvalOp::GenomeToAngles(FloatingPointP gen)
{
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

	return angles;
}