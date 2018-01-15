#include "IInstanceCalculator.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const float missedStudentPenalty = 500.0f;
const float maxCapacityPenalty = 1000.0f;

float SBR::GreedyInstanceCalculator::PickStops(SBR::InstanceLoader* loader, const vector<int>& studentsInSector, const vector<int>& stopsInSector)
{
	float totalCost = 0.0f;

	pickedSectorStops.clear();

	float maxWalk = loader->GetMaxWalk();
	float maxWalk2 = maxWalk * maxWalk;

	const std::vector<SBR::Position>& students = loader->GetStudentPositions();
	const std::vector<SBR::Position>& stops = loader->GetStopPositions();

	for (int j = 0; j < studentsInSector.size(); ++j) {
		// find the closest stop to student
		int pickedStopIndex = -1;
		float minDistance = 0.0f;

		// check every stop in sector
		for (int k = 0; k < stopsInSector.size(); ++k) {
			// skip school
			if (stopsInSector[k] == 0) {
				continue;
			}

			float distance2 = SBR::Position::CalculateDistance2(students[studentsInSector[j]], stops[stopsInSector[k]]);
			// nothing to do if too far
			if (distance2 > maxWalk2) {
				continue;
			}

			// stop is not too far, remember it if it's first or closer than previous
			if (pickedStopIndex == -1 || distance2 < minDistance) {
				pickedStopIndex = k;
				minDistance = distance2;
			}
		}

		// if no reachable stop was found
		if (pickedStopIndex == -1) {
			// add massive cost and put school as chosen stop
			totalCost += missedStudentPenalty;
			studentStops[studentsInSector[j]] = 0;
			missedStudents++;
			continue;
		}
		else {
			// otherwise, remember student's stop
			studentStops[studentsInSector[j]] = stopsInSector[pickedStopIndex];
			studentCountPerRoute[currentSector]++;
		}

		// go through list of stops already used in sector
		bool alreadyAdded = false;
		for (int k = 0; k < pickedSectorStops.size(); ++k) {
			// if stop was already used, don't add it
			if (pickedSectorStops[k] == stopsInSector[pickedStopIndex]) {
				alreadyAdded = true;
				break;
			}
		}

		if (!alreadyAdded) {
			pickedSectorStops.push_back(stopsInSector[pickedStopIndex]);
		}
	}

	return totalCost;
}

float SBR::GreedyInstanceCalculator::CreateRoutes(InstanceLoader* loader, const vector<int>& studentsInSector, const vector<int>& stopsInSector)
{
	float totalCost = 0.0f;
	
	const std::vector<SBR::Position>& stops = loader->GetStopPositions();
	SBR::Position school = stops[0];

	// determine closest stop to school
	int firstStop = 0;
	float minDistance = SBR::Position::CalculateDistance2(school, stops[pickedSectorStops[0]]);
	for (int currentStop = 1; currentStop < pickedSectorStops.size(); ++currentStop) {
		// make sure school hasn't ended in this list
		assert(pickedSectorStops[currentStop] != 0);

		// if shorter distance was found, update first stop on route
		float distance = SBR::Position::CalculateDistance2(school, stops[pickedSectorStops[currentStop]]);
		if (distance < minDistance) {
			minDistance = distance;
			firstStop = currentStop;
		}
	}
	// add cost of traveling from school to first stop
	totalCost += sqrt(minDistance);
	routes[currentSector].push_back(pickedSectorStops[firstStop]);

	// greedily choose next stops on route, based on nearest neighbour principle
	for (int currentStop = 0; currentStop < pickedSectorStops.size() - 1; ++currentStop) {
		// starting point is the latest stop on route
		int currentIndex = routes[currentSector][currentStop];
		SBR::Position current = stops[currentIndex];
		int nextStop = -1;
		float minDistanceNext = 0.0f;

		// check every stop
		for (int otherStops = 0; otherStops < pickedSectorStops.size(); ++otherStops) {
			float distance = SBR::Position::CalculateDistance2(current, stops[pickedSectorStops[otherStops]]);

			// if we found a closer stop
			if (nextStop == -1 || distance < minDistanceNext) {
				// check that it's a new stop
				bool alreadyAdded = false;
				for (int sortedStops = 0; sortedStops < routes[currentSector].size(); ++sortedStops) {
					// if stop was already used, don't add it
					if (routes[currentSector][sortedStops] == pickedSectorStops[otherStops]) {
						alreadyAdded = true;
						break;
					}
				}
				if (alreadyAdded) {
					continue;
				}
				// remember next stop on route
				nextStop = otherStops;
				minDistanceNext = distance;
			}
		}

		// add cost of traveling from one stop to next
		totalCost += sqrt(minDistanceNext);
		routes[currentSector].push_back(pickedSectorStops[nextStop]);
	}

	// make sure all the stops were used
	assert(routes[currentSector].size() == pickedSectorStops.size());

	// after route is done, add cost of returning to school
	totalCost += SBR::Position::CalculateDistance(school, stops[routes[currentSector][routes[currentSector].size() - 1]]);
	return totalCost;
}

double SBR::GreedyInstanceCalculator::CalculateRoutingCost(SBR::InstanceLoader* loader, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector)
{
	int sectors = studentsBySector.size();
	assert(sectors == busStopsBySector.size());

	missedStudents = 0;
	routes.clear();
	if (studentStops.size() == 0) {
		studentStops.resize(loader->GetStudentPositions().size());
	}

	float totalCost = 0.0f;
	for (currentSector = 0; currentSector < sectors; ++currentSector) {
		routes.push_back(std::vector<int>());

		const vector<int>& studentsInSector = studentsBySector[currentSector];
		const vector<int>& stopsInSector = busStopsBySector[currentSector];

		studentCountPerRoute.push_back(0);

		totalCost += PickStops(loader, studentsInSector, stopsInSector);
		if (pickedSectorStops.size() == 0) {
			continue;
		}

		totalCost += CreateRoutes(loader, studentsInSector, stopsInSector);
	}

	// try to find stops for the students that don't have assigned bus stop
	totalCost += RouteRemainingStudents(loader, studentsBySector);

	totalCost += CalculateCapacityPenalty(loader);

	return totalCost;
}

void SBR::GreedyInstanceCalculator::Print(const char* fileName)
{
	ofstream fileout(fileName);

	for (int i = 0; i < routes.size(); ++i) {
		if (routes[i].size() == 0) {
			continue;
		}

		for (int j = 0; j < routes[i].size(); ++j) {
			fileout << routes[i][j] << " ";
		}
		fileout << endl;
	}
	fileout << endl;

	for (int i = 0; i < studentStops.size(); ++i) {
		fileout << (i + 1) << " " << studentStops[i] << endl;
	}
	fileout << endl;

	fileout.close();

	cout << "Missed students: " << missedStudents << endl;
}

float SBR::GreedyInstanceCalculator::RouteRemainingStudents(SBR::InstanceLoader* loader, vector<vector<int>>& studentsBySector)
{
	if (missedStudents == 0)
	{
		return 0.0f;
	}
	float penalty = 0.0f;
	const std::vector<SBR::Position>& stops = loader->GetStopPositions();
	const std::vector<SBR::Position>& students = loader->GetStudentPositions();
	const float maxWalk2 = loader->GetMaxWalk() * loader->GetMaxWalk();
	const int startMissedStudents = missedStudents;
	int maxCapacity = loader->GetCapacity();
	for (int idxSector = 0; idxSector < studentsBySector.size(); ++idxSector)
	{
		for (int idx = 0; idx < studentsBySector[idxSector].size(); ++idx)
		{
			int idxStudent = studentsBySector[idxSector][idx];
			bool studentPlaced = false;
			// continue if student was assigned
			if (studentStops[idxStudent] != 0)
			{
				continue;
			}
			const SBR::Position& studentPosition = students[idxStudent];
			// find stop on route that is in max walk
			for (int i = 0; i < routes.size(); ++i) {
				if (routes[i].size() == 0) {
					continue;
				}
				for (int j = 0; j < routes[i].size(); ++j) {
					const SBR::Position& stopPosition = stops[routes[i][j]];
					// if in walkable area, assign
					if (SBR::Position::CalculateDistance2(stopPosition, studentPosition) < maxWalk2 && studentCountPerRoute[i] < maxCapacity)
					{
						studentStops[idxStudent] = routes[i][j];
						missedStudents--;
						penalty -= missedStudentPenalty;
						studentCountPerRoute[i]++;
						studentPlaced = true;
						break;
					}
				}
				if (studentPlaced)
				{
					break;
				}
			}
		}
	}
	return penalty;
}

float SBR::GreedyInstanceCalculator::CalculateCapacityPenalty(SBR::InstanceLoader* loader)
{
	float penalty = 0;
	int maxCapacity = loader->GetCapacity();
	for (int i = 0; i < studentCountPerRoute.size(); ++i)
	{
		int overhead = studentCountPerRoute[i] - maxCapacity;
		overhead = overhead > 0 ? overhead : 0;
		penalty += maxCapacityPenalty * overhead;
	}
	return penalty;
}