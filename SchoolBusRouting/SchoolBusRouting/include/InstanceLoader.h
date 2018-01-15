#pragma once
#include<string>
#include<vector>

namespace SBR
{

	struct Position
	{
		float x;
		float y;
		double polarAngle;

		Position(float x, float y);
		Position(const Position& other);

		static float CalculateDistance2(const Position& first, const Position& second);
		static float CalculateDistance(const Position& first, const Position& second);
	};

	enum ReadState
	{
		START_INFO,
		STOPS,
		STUDENTS,
		END
	};

	class InstanceLoader
	{
	private:
		std::vector<Position> stopPositions;
		std::vector<Position> studentPositions;

		std::vector<int> stopPositionsIndicesSorted;
		std::vector<int> studentPositionsIndicesSorted;

		// which students can walk to bus stops
		std::vector<std::vector<int>> studentsInRange;

		float maxWalk;
		int capacity;
	public:
		// will load instance from the file and sort bus stops and student positions by polar angle
		InstanceLoader(const std::string& strFilePath);
		const std::vector<Position>& GetStopPositions();
		const std::vector<Position>& GetStudentPositions();
		float GetMaxWalk();
		int GetCapacity();
		const std::vector<int>& GetStopPositionIndicesSorted();
		const std::vector<int>& GetStudentPositionIndicesSorted();
		const std::vector<int>& GetStudentsInRangeOfStop(int busStopIndex);
	private:
		// calculates polar angles for both stop and student positions
		void CalculatePolarAngles();
		void FindStudentStops(void);
		void Load(const std::string& strFilePath);
		void ReadStartInfo(const std::string& strLine);
		void ReadStop(const std::string& strLine);
		void ReadStudent(const std::string& strLine);
		void SortIndices();
	};

}