#include "InstanceLoader.h"
#include "InstanceProblem.h"

#include <iostream>
#include "MaxFlowFordFulkerson.h"
#include "SBRFunctionEvalOp.h"

class DummyInstanceCalculator : public SBR::IInstanceCalculator
{
public:
	virtual double CalculateRoutingCost(vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector)
	{
		return 0.0;
	}
};

int main(int argc, char* argv[])
{
	SBR::InstanceLoader loader("instances\\sbr7.txt");
	SBR::Position pos1(0, 0), pos2(3, 4);
	float dist = SBR::Position::CalculateDistance(pos1, pos2);

	int cap = loader.GetCapacity();
	float maxWalk = loader.GetMaxWalk();
	const std::vector<SBR::Position>& busStops = loader.GetStopPositions();
	const std::vector<SBR::Position>& studentPositions = loader.GetStudentPositions();

	DummyInstanceCalculator calc;

	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new SBRFunctionEvalOp(&loader, &calc));

	state->initialize(argc, argv);
	state->run();

	HallOfFameP hallOfFame = state->getHoF();              // population hall of fame
	std::vector<IndividualP> best = hallOfFame->getBest();

	for (IndividualP individual : best)
	{
		FloatingPointP gen = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (individual->getGenotype());


	}

	getchar();
	return 0;
}