#include "InstanceLoader.h"

#include <iostream>
#include <vector>
#include "MaxFlowFordFulkerson.h"
#include "SBRFunctionEvalOp.h"

void ChangeDimensionInFile(int dim);
void PerformDimensionRun(SBR::InstanceLoader& loader, int dim, int argc, char* argv[]);

double minVal = std::numeric_limits<double>::max();

int main(int argc, char* argv[])
{
	SBR::InstanceLoader loader("instances\\sbr1.txt");

	int minRoutes = (int)ceil(1.0 * loader.GetStudentPositions().size() / loader.GetCapacity());
	int step = 5;
	// -1 stands for school
	int maxRoutes = loader.GetStopPositions().size() - 1;

	float T = 0.1f;
	int dimension = minRoutes * (1 - T) + maxRoutes * T;
	PerformDimensionRun(loader, dimension, argc, argv);
	
	getchar();
	return 0;
}

void ChangeDimensionInFile(int dim)
{
	string strStart = "<Entry key=\"dimension\">";
	string strNew = std::string("			<Entry key=\"dimension\">") + std::to_string(dim) + "</Entry>";
	ifstream filein("parameters.txt"); //File to read from
	if (!filein)
	{
		cout << "Error opening files!" << endl;
		return;
	}

	string strTemp;
	string strOut("");
	while (getline(filein, strTemp))
	{
		if (strTemp.find(strStart) != std::string::npos) {
			strTemp = strNew;
		}
		strTemp += "\n";
		strOut += strTemp;
	}
	filein.close();
	ofstream fileout("parameters.txt");
	fileout << strOut;
	fileout.close();
}

void PerformDimensionRun(SBR::InstanceLoader& loader, int dim, int argc, char* argv[])
{
	cout << "Running algorithm for " << dim << " buses." << endl;

	ChangeDimensionInFile(dim);

	SBR::GreedyInstanceCalculator calc;

	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new SBRFunctionEvalOp(&loader, &calc));

	state->initialize(argc, argv);

	int* maxTime = (int*)state->getRegistry().get()->getEntry("term.maxtime").get();

	cout << "Time limit is set to: " << *maxTime << endl;

	state->run();

	HallOfFameP hallOfFame = state->getHoF();              // population hall of fame
	std::vector<IndividualP> best = hallOfFame->getBest();

	SectorManager manager(&loader);

	for (IndividualP individual : best)
	{
		FloatingPointP gen = boost::dynamic_pointer_cast<FloatingPoint::FloatingPoint> (individual->getGenotype());

		std::vector<double> angles = SBRFunctionEvalOp::GenomeToAngles(gen);

		std::vector<std::vector<int>> studentsBySector;
		std::vector<std::vector<int>> busStopsBySector;

		manager.PerformSectoring(angles, studentsBySector, busStopsBySector);

		// decode and write solution
		double val = calc.CalculateRoutingCost(&loader, studentsBySector, busStopsBySector);
		if (val < minVal)
		{
			calc.Print("res-ne-sbr7.txt");
			minVal = val;
		}
	}

	return;
}