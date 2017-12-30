#pragma once

#include "InstanceLoader.h"

#include <cstring>

namespace SBR
{
	class InstanceProblem 
	{
	private:
		InstanceLoader &data;
		const int busStopsCount;
		const int studentCount;
		int maxBusCount;

		std::vector<float> costs;
		std::vector<bool> reachableStops;

		std::vector<bool> routes;
		std::vector<bool> visitedStops;
		std::vector<bool> studentBoarding;
	public:
		InstanceProblem(InstanceLoader &loader);
		void InitializeCosts(void);
		void InitializeReachableStops(void);
		void InitializeDecisionVariables(void);
		float CalculateTotalCost(void);
		std::string PrintRoutes(void);
	};
}
