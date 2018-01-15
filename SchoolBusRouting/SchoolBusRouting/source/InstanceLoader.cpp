#include"InstanceLoader.h"
#include<fstream>
#include<iostream>
#include "Utils.h"
#include<boost\math\constants\constants.hpp>
#include <algorithm>

namespace SBR
{
	Position::Position(float x, float y) :
		x(x), 
		y(y)
	{

	}

	Position::Position(const Position& other) :
		x(other.x),
		y(other.y),
		polarAngle(other.polarAngle)
	{

	}

	float Position::CalculateDistance2(const Position& first, const Position& second)
	{
		float dx = first.x - second.x;
		float dy = first.y - second.y;

		return dx * dx + dy * dy;
	}

	float Position::CalculateDistance(const Position& first, const Position& second)
	{
		return sqrt(CalculateDistance2(first, second));
	}

	InstanceLoader::InstanceLoader(const std::string& strFilePath)
	{
		Load(strFilePath);
		FindStudentStops();
		CalculatePolarAngles();
		SortIndices();
	}

	const std::vector<Position>& InstanceLoader::GetStopPositions()
	{
		return stopPositions;
	}

	const std::vector<Position>& InstanceLoader::GetStudentPositions()
	{
		return studentPositions;
	}

	const std::vector<int>& InstanceLoader::GetStopPositionIndicesSorted()
	{
		return stopPositionsIndicesSorted;
	}

	const std::vector<int>& InstanceLoader::GetStudentsInRangeOfStop(int busStopIndex)
	{
		return studentsInRange[busStopIndex];
	}

	float InstanceLoader::GetMaxWalk()
	{
		return maxWalk;
	}

	int InstanceLoader::GetCapacity()
	{
		return capacity;
	}

	void InstanceLoader::Load(const std::string& strFilePath)
	{
		std::fstream fin(strFilePath);

		if (!fin.is_open())
		{
			std::cout << "File: " << strFilePath << " is not present." << std::endl;
		}

		std::string strLine;

		ReadState rsCurrentState = ReadState::START_INFO;

		while (std::getline(fin, strLine))
		{
			if (strLine == "")
			{
				while (strLine == "")
				{
					std::getline(fin, strLine);
				}
				switch (rsCurrentState)
				{
				case START_INFO:
					rsCurrentState = STOPS;
					break;
				case STOPS:
					rsCurrentState = STUDENTS;
					break;
				case STUDENTS:
					rsCurrentState = END;
					break;
				}
			}
			switch (rsCurrentState)
			{
			case START_INFO:
				ReadStartInfo(strLine);
				break;
			case STOPS:
				ReadStop(strLine);
				break;
			case STUDENTS:
				ReadStudent(strLine);
				break;
			}
			//std::cout << strLine << '\n';
		}

		fin.close();
	}

	void InstanceLoader::ReadStartInfo(const std::string& strLine)
	{
		int STOP_COUNT_INDEX = 0;
		int STUDENT_COUNT_INDEX = 2;
		int MAX_WALK_INDEX = 4;
		int CAPACITY_INDEX = 7;
		std::vector<std::string> tokens = split(strLine, " \t,");
		int numStops = std::atoi(tokens[STOP_COUNT_INDEX].c_str());
		int numStudents = std::atoi(tokens[STUDENT_COUNT_INDEX].c_str());
		maxWalk = std::atof(tokens[MAX_WALK_INDEX].c_str());
		capacity = std::atoi(tokens[CAPACITY_INDEX].c_str());
	}

	void InstanceLoader::ReadStop(const std::string& strLine)
	{
		int STOP_INDEX = 0;
		int X_INDEX = 1;
		int Y_INDEX = 2;

		std::vector<std::string> tokens = split(strLine, " \t");
		int index = atoi(tokens[STOP_INDEX].c_str());
		float x = atof(tokens[X_INDEX].c_str());
		float y = atof(tokens[Y_INDEX].c_str());

		stopPositions.push_back(Position(x, y));
	}

	void InstanceLoader::ReadStudent(const std::string& strLine)
	{
		int STOP_INDEX = 0;
		int X_INDEX = 1;
		int Y_INDEX = 2;

		std::vector<std::string> tokens = split(strLine, " \t");
		int index = atoi(tokens[STOP_INDEX].c_str());
		float x = atof(tokens[X_INDEX].c_str());
		float y = atof(tokens[Y_INDEX].c_str());

		studentPositions.push_back(Position(x, y));
	}
	
	class PositionComparator
	{
	private:
		const std::vector<Position>& positions;
	public:
		PositionComparator(const std::vector<Position>& positions) : positions(positions) {}

		bool operator()(int i, int j)
		{
			return positions[i].polarAngle < positions[j].polarAngle;
		}
	};

	void InstanceLoader::CalculatePolarAngles()
	{
		Position posSchool = stopPositions[0];
		for (int i = 0; i < stopPositions.size(); ++i)
		{
			double dy = stopPositions[i].y - posSchool.y;
			double dx = stopPositions[i].x - posSchool.x;
			double angle = atan2(dy, dx);
			angle = angle < 0.0 ? angle + 2 * boost::math::constants::pi<double>() : angle;
			stopPositions[i].polarAngle = angle;
			stopPositionsIndicesSorted.push_back(i);
		}
		for (int i = 0; i < studentPositions.size(); ++i)
		{
			double dy = studentPositions[i].y - posSchool.y;
			double dx = studentPositions[i].x - posSchool.x;
			double angle = atan2(dy, dx);
			angle = angle < 0.0 ? angle + 2 * boost::math::constants::pi<double>() : angle;
			studentPositions[i].polarAngle = angle;
		}
	}

	void InstanceLoader::FindStudentStops(void)
	{
		studentsInRange.push_back(std::vector<int>());

		float maxWalk2 = maxWalk * maxWalk;

		// check if bus stop is reachable by student
		for (int i = 1; i < stopPositions.size(); ++i) {
			studentsInRange.push_back(std::vector<int>());
			for (int j = 0; j < studentPositions.size(); ++j) {
				// square root isn't calculated since we only care about which distance is lesser
				if (SBR::Position::CalculateDistance2(stopPositions[i], studentPositions[j]) <= maxWalk2) {
					studentsInRange[i].push_back(j);
				}
			}
		}
	}

	void InstanceLoader::SortIndices()
	{
		// sort positions by polar angle to make handling it easier
		// step over school, just in case
		std::sort(stopPositionsIndicesSorted.begin() + 1, stopPositionsIndicesSorted.end(), PositionComparator(stopPositions));
	}
}