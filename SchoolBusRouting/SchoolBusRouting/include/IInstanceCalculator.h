#pragma once
#include <vector>
#include "InstanceLoader.h"

namespace SBR
{
	using namespace std;
	class IInstanceCalculator
	{
	public:
		/**
		*   \brief Method that calculates routing cost given students and bus stops per sector.
		*
		*   Method accepts vectors that at position i have sector i (slice of whole area), it's students and bus stops.
		*	Method calculates, based on given sectors, what is the value of routing function.
		*
		*   \param studentsBySector vector of student indices divided into sectors.
		*   \param busStopsBySector vector of bus stop indices divided into sectors.
		*   \return double representing value of routing function.
		*
		**/
		virtual double CalculateRoutingCost(InstanceLoader* loader, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector) = 0;
	};

	class GreedyInstanceCalculator : public SBR::IInstanceCalculator
	{
	private:
		vector<vector<int>> routes;
		vector<int> studentStops;
		vector<int> pickedSectorStops;
		int currentSector;
		int missedStudents;
	private:
		float PickStops(InstanceLoader* loader, const vector<int>& studentsInSector, const vector<int>& stopsInSector);
		float CreateRoutes(InstanceLoader* loader, const vector<int>& studentsInSector, const vector<int>& stopsInSector);
	public:
		virtual double CalculateRoutingCost(SBR::InstanceLoader* loader, vector<vector<int>>& studentsBySector, vector<vector<int>>& busStopsBySector);
		void Print(const char* fileName);
	};
}