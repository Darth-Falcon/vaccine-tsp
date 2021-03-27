#include <algorithm>
#include "pso.cpp"
#include "fitnessFunc.cpp"
#include "rapidcsv.h"


std::vector<city> createData(int numCities, std::string fileName)
{
	//Read csv and store in temporary vectors
	rapidcsv::Document doc(fileName);

	std::vector<std::string> names = doc.GetColumn<std::string>("City Name");
	std::vector<std::string> states = doc.GetColumn<std::string>("State");
	std::vector<int> populations = doc.GetColumn<int>("Population");
	std::vector<double> areas = doc.GetColumn<double>("Area");
	std::vector<std::string> latitudes = doc.GetColumn<std::string>("Latitude");
	std::vector<std::string> longitudes = doc.GetColumn<std::string>("Longitude");

	std::vector<city> cities;
	//Create vector of cities
	for( int i = 0; i < names.size(); i++)
	{
		cities.push_back(city(names[i], states[i], populations[i], areas[i], latitudes[i], longitudes[i]));
	}

	//Randomly shuffle the sitties and pick the first N to be used
	std::random_shuffle(cities.begin(), cities.end());

	return std::vector<city>(cities.begin(),cities.begin()+numCities);
}


int main(int argc, char** argv)
{
		std::string fileName;
	int numCities, numThreads, speed, numParticles;
	if (argc == 6)
	{

		fileName = argv[1];
		numParticles = std::stoi(argv[2]);
		numCities = std::stoi(argv[3]);
		numThreads = std::stoi(argv[4]);
		speed = std::stoi(argv[5]);
		
	}
	else if (argc == 1)
	{
		fileName = "../data/cityData.csv";
		numParticles = 50;
		numCities = 150;
		numThreads = 1;
		speed = 60;
	}
	else
	{
		throw std::invalid_argument("Please input 0 or 5 parameters");
	}

	std::vector<city> cities = createData(numCities, fileName);

	PSO(numParticles, numCities, 3000, 0, numCities-1, -(numCities/10), numCities/10, &tspFitness, cities, speed, numThreads);

	return 0;
}