#pragma once

#include "InstanceLoader.h"

namespace SBR
{
	class InstanceProblem 
	{
	private:
		InstanceLoader &data;
		const int busStopsCount;
		const int studentCount;
		int maxBusCount;

		float *costs;
		bool *reachableStops;

		bool *routes;
		bool *visitedStops;
		bool *studentBoarding;
	public:
		InstanceProblem(InstanceLoader &loader);
		void InitializeCosts(void);
		void InitializeReachableStops(void);
		void InitializeDecisionVariables(void);
	};
}
