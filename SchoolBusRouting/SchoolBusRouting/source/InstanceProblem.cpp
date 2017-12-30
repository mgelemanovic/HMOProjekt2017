#include "InstanceProblem.h"
#include "InstanceLoader.h"

#include <cstring>

namespace SBR
{
	InstanceProblem::InstanceProblem(InstanceLoader &loader) :
		data(loader), busStopsCount(loader.GetStopPositions().size()), studentCount(loader.GetStudentPositions().size())
	{
		maxBusCount = busStopsCount - 1;
		InitializeCosts();
		InitializeReachableStops();
		InitializeDecisionVariables();
	}

	void InstanceProblem::InitializeCosts(void)
	{
		std::vector<SBR::Position> busStops = data.GetStopPositions();
		costs = new float[busStopsCount * busStopsCount];

		for (int i = 0; i < busStopsCount; ++i) {
			// there is no distance between the same bus stop
			costs[i * busStopsCount + i] = 0.0f;
			for (int j = i + 1; j < busStopsCount; ++j) {
				// calculate distance both from stop i to j and from j to i
				costs[j * busStopsCount + i] = costs[i * busStopsCount + j] = SBR::Position::CalculateDistance(busStops[i], busStops[j]);
			}
		}
	}

	void InstanceProblem::InitializeReachableStops(void)
	{
		const std::vector<SBR::Position> busStops = data.GetStopPositions();
		const std::vector<SBR::Position> students = data.GetStudentPositions();
		reachableStops = new bool[busStopsCount * studentCount];

		float maxWalk = data.GetMaxWalk();
		float maxWalk2 = maxWalk * maxWalk;

		// check if bus stop is reachable by student
		for (int i = 0; i < busStopsCount; ++i) {
			for (int j = 0; j < studentCount; ++j) {
				// square root isn't calculated since we only care about which distance is lesser
				reachableStops[i * studentCount + j] = (SBR::Position::CalculateDistance2(busStops[i], students[j]) <= maxWalk2) ? 1 : 0;
			}
		}
	}

	void InstanceProblem::InitializeDecisionVariables(void)
	{
		routes = new bool[busStopsCount * busStopsCount * maxBusCount];
		memset(routes, 0, busStopsCount * busStopsCount * maxBusCount * sizeof(bool));

		visitedStops = new bool[busStopsCount * maxBusCount];
		memset(visitedStops, 0, busStopsCount * maxBusCount * sizeof(bool));

		studentBoarding = new bool[busStopsCount * studentCount * maxBusCount];
		memset(studentBoarding, 0, busStopsCount * studentCount * maxBusCount * sizeof(bool));

		for (int i = 0; i < maxBusCount; ++i) {
			// each bus stop is visited by a new bus (excepts school)
			visitedStops[(i+1) * maxBusCount + i] = 1;

			routes[(i+1) * maxBusCount + i] = 1;
			routes[(i+1) * busStopsCount * maxBusCount + i] = 1;
		}
	}
}