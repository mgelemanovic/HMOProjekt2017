#include"InstanceLoader.h"
#include<fstream>
#include<iostream>
#include "Utils.h"
namespace SBR
{
	Position::Position(float x, float y) :
		x(x), 
		y(y)
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
		CalculatePolarAngles();
	}

	const std::vector<Position>& InstanceLoader::GetStopPositions()
	{
		return stopPositions;
	}

	const std::vector<Position>& InstanceLoader::GetStudentPositions()
	{
		return studentPositions;
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

	void InstanceLoader::CalculatePolarAngles()
	{
		Position posSchool = stopPositions[0];
		for (int i = 0; i < stopPositions.size(); ++i)
		{
			double dy = stopPositions[i].y - posSchool.y;
			double dx = stopPositions[i].x - posSchool.x;
			double angle = atan2(dy, dx);
			stopPositionPolarAngles.push_back(angle);
		}
		for (int i = 0; i < studentPositions.size(); ++i)
		{
			double dy = posSchool.y - studentPositions[i].y;
			double dx = posSchool.x - studentPositions[i].x;
			studentPositionPolarAngles.push_back(atan2(dy, dx));
		}
	}

	const std::vector<double>& InstanceLoader::GetStudentPositionPolarAngles()
	{
		return studentPositionPolarAngles;
	}

	const std::vector<double>& InstanceLoader::GetStopPositionPolarAngles()
	{
		return stopPositionPolarAngles;
	}
}