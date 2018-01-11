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
		*   \param studentsBySector vector of students divided into sectors.
		*   \param busStopsBySector vector of bus stops divided into sectors.
		*   \return double representing value of routing function.
		*
		**/
		virtual double CalculateRoutingCost(vector<vector<Position>> studentsBySector, vector<vector<Position>> busStopsBySector) = 0;
	};
}