#include "InstanceLoader.h"
#include "InstanceProblem.h"

#include <iostream>
#include "MaxFlowFordFulkerson.h"
#include "SBRFunctionEvalOp.h"

int main(int argc, char* argv[])
{
	SBR::InstanceLoader loader("instances\\sbr7.txt");
	SBR::Position pos1(0, 0), pos2(3, 4);
	float dist = SBR::Position::CalculateDistance(pos1, pos2);

	int cap = loader.GetCapacity();
	float maxWalk = loader.GetMaxWalk();
	const std::vector<SBR::Position>& busStops = loader.GetStopPositions();
	const std::vector<SBR::Position>& studentPositions = loader.GetStudentPositions();

	SBR::InstanceProblem problem(loader);
	std::cout << problem.CalculateTotalCost() << std::endl;
	std::cout << problem.PrintRoutes() << std::endl;

	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new SBRFunctionEvalOp);

	state->initialize(argc, argv);
	state->run();

	getchar();
	return 0;
}