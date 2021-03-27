#include <math.h>
#include "cityData.h"

double tspFitness(std::vector<std::vector<int>> particles, int numCities, int particle, std::vector<city> cities, int speed)
{
	city a;
	city b;
	double distance = 0;
	double travelTime = 0;
	double timePerPerson = 0;

    for(int i = 0; i < numCities-1; i++)
	{
		a = cities[particles[particle][i]];
		b = cities[particles[particle][i+1]];

		distance = 3959 * acos(sin(std::get<0>(b.getLocation()))* sin(std::get<0>(a.getLocation()))) + (cos(std::get<0>(b.getLocation()))*cos(std::get<0>(a.getLocation()))*cos(std::get<0>(b.getLocation())-std::get<0>(a.getLocation())));

		travelTime = distance/speed;
		timePerPerson += b.getPop()/travelTime;
	}

	return timePerPerson/numCities;
}