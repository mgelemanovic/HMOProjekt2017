#include "IInstanceCalculator.h"
#include "MaxFlowFordFulkerson.h"

#include <iostream>
#include <cassert>
#include <fstream>

#include <set>

using namespace std;

const float missedStudentPenalty = 500.0f;
const float maxCapacityPenalty = 1000.0f;

float SBR::GreedyInstanceCalculator::CalculateCost_internal(void)
{
	return cost + missedStudentPenalty * missedStudents + maxCapacityPenalty * overCapacityRoutes;
}

void SBR::GreedyInstanceCalculator::CreateInitialRoutes(const vector<int>& stopsInSector)
{
	// add new route for sector
	routes.push_back(std::vector<int>());

	// initial route is all stops in sector (without school, unordered)
	for (int i = 0; i < stopsInSector.size(); ++i) {
		if (stopsInSector[i] == 0) {
			continue;
		}
		routes[currentSector].push_back(stopsInSector[i]);
	}
}

void SBR::GreedyInstanceCalculator::AddReachableStudents(Graph& g, int currentRoute, int studentStart, InstanceLoader* loader)
{
	// find a set of students that can reach this bus route
	std::set<int> students;
	for (int i = 0; i < routes[currentRoute].size(); ++i) {
		const std::vector<int>& reachableStudents = loader->GetStudentsInRangeOfStop(routes[currentRoute][i]);
		for (int j = 0; j < reachableStudents.size(); ++j)  {
			bool alreadyAdded = students.find(reachableStudents[j]) != students.end();

			if (!alreadyAdded) {
				students.insert(reachableStudents[j]);
			}
		}
	}

	// create edges from bus routes to reachable students, with capacity of 1
	for (auto it = students.begin(); it != students.end(); ++it) {
		g.AddEdge(currentRoute + 1, studentStart + *it, 1);
	}
}

void SBR::GreedyInstanceCalculator::PickStops(InstanceLoader* loader, const vector<int>& studentIndices)
{
	int studentCount = studentIndices.size();

	const std::vector<int> pickedSectorStops = routes[currentSector];
	std::vector<bool> visitedStops(pickedSectorStops.size(), 0);
	routes[currentSector].clear();

	const std::vector<SBR::Position>& students = loader->GetStudentPositions();
	const std::vector<SBR::Position>& stops = loader->GetStopPositions();
	float maxWalk = loader->GetMaxWalk();
	float maxWalk2 = maxWalk * maxWalk;

	// check capacity constraint
	float capacity = loader->GetCapacity();
	if (studentCount > capacity) {
		overCapacityRoutes += (studentCount - capacity);
	}

	// determine which stops are visited by students
	for (int i = 0; i < studentCount; ++i) {
		for (int j = 0; j < pickedSectorStops.size(); ++j) {
			// student picks first reachable stop in the route
			if (SBR::Position::CalculateDistance2(students[studentIndices[i]], stops[pickedSectorStops[j]]) < maxWalk2) {
				// remember that stop is being used
				visitedStops[j] = true;
				// remember where this student is going
				studentStops[studentIndices[i]] = pickedSectorStops[j];
				break;
			}
		}
	}

	// final route will only contain those stops that are visited by atleast one student
	assert(visitedStops.size() == pickedSectorStops.size());
	for (int i = 0; i < visitedStops.size(); ++i) {
		if (visitedStops[i]) {
			routes[currentSector].push_back(pickedSectorStops[i]);
		}
	}
}

void SBR::GreedyInstanceCalculator::AssignStudents(InstanceLoader* loader, int sectors)
{
	int studentCount = loader->GetStudentPositions().size();
	// at which index student nodes start
	int studentNodesStart = sectors + 1;
	// school node + route nodes + student nodes + final node
	int totalGraphNodes = studentNodesStart + studentCount + 1;
	if (studentStops.size() == 0) {
		studentStops.resize(studentCount);
	}

	// create graph that will be used in Ford-Fulkerson algorithm to determine student/route assigments
	int capacity = loader->GetCapacity();
	Graph g(totalGraphNodes);
	for (int i = 0; i < sectors; ++i) {
		// add edges that go from school to bus routes, with bus capacity
		g.AddEdge(0, i + 1, capacity);
		// connect route nodes with reachable students
		AddReachableStudents(g, i, studentNodesStart, loader);
	}

	for (int i = 0; i < studentCount; ++i) {
		// edges that go from students to end node
		g.AddEdge(studentNodesStart + i, totalGraphNodes - 1, 1);
	}

	// run max flow Ford-Fulkerson algorithm
	int d = g.MaxFlow(0, totalGraphNodes - 1);
	// check if some students weren't assigned a route
	missedStudents = studentCount - d;

	// go through sectors (routes) and discover which students are in that route
	for (currentSector = 0; currentSector < sectors; ++currentSector) {
		int i = currentSector;
		std::vector<int> pickedStudents;
		// if flow from route node to student node is 1, that means student is using that bus route
		for (int j = 0; j < g.adj[i+1].size(); ++j) {
			if (g.adj[i+1][j].flow == 1) {
				// remember student index
				int studentIndex = g.adj[i + 1][j].dst - studentNodesStart;
				pickedStudents.push_back(studentIndex);
			}
		}
		// discover used bus stops
		PickStops(loader, pickedStudents);
	}
}

void SBR::GreedyInstanceCalculator::CreateRoutes(InstanceLoader* loader)
{
	const vector<int> pickedSectorStops = routes[currentSector];
	routes[currentSector].clear();

	// nothing to do here if no stops are used in this sector
	if (pickedSectorStops.size() == 0) {
		return;
	}
	
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
	cost += sqrt(minDistance);
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
		cost += sqrt(minDistanceNext);
		routes[currentSector].push_back(pickedSectorStops[nextStop]);
	}

	// make sure all the stops were used
	assert(routes[currentSector].size() == pickedSectorStops.size());

	// after route is done, add cost of returning to school
	cost += SBR::Position::CalculateDistance(school, stops[routes[currentSector][routes[currentSector].size() - 1]]);
}

double SBR::GreedyInstanceCalculator::CalculateRoutingCost(SBR::InstanceLoader* loader, vector<vector<int>>& busStopsBySector)
{
	int sectors = busStopsBySector.size();

	missedStudents = 0;
	overCapacityRoutes = 0;

	routes.clear();
	for (currentSector = 0; currentSector < sectors; ++currentSector) {
		CreateInitialRoutes(busStopsBySector[currentSector]);
	}
	
	AssignStudents(loader, sectors);

	for (currentSector = 0; currentSector < sectors; ++currentSector) {
		CreateRoutes(loader);
	}


	return CalculateCost_internal();
}

void SBR::GreedyInstanceCalculator::Print(const char* fileName)
{
	ofstream fileout(fileName);

	// print used bus routes
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

	// print which stops are used by students
	for (int i = 0; i < studentStops.size(); ++i) {
		fileout << (i + 1) << " " << studentStops[i] << endl;
	}
	fileout << endl;

	fileout.close();

	cout << "Missed students: " << missedStudents << endl;
}