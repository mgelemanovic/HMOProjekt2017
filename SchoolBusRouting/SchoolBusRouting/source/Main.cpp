#include "InstanceLoader.h"

int main(void)
{
	SBR::InstanceLoader loader("instances\\sbr7.txt");
	SBR::Position pos1(0, 0), pos2(3, 4);
	float dist = SBR::Position::CalculateDistance(pos1, pos2);

	int cap = loader.GetCapacity();
	float maxWalk = loader.GetMaxWalk();
	const std::vector<SBR::Position>& busStops = loader.GetStopPositions();
	const std::vector<SBR::Position>& studentPositions = loader.GetStudentPositions();

	getchar();
}