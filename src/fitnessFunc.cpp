#include <math.h>
#include "cityData.h"
#include "particle.h"

template<typename T>
using vec = std::vector<T>;

double tspFitness(vec<particle> particles, int numCities, int particle, vec<city> cities, int speed)
{
	city a;
	city b;
	double distance = 0;
	double travelTime = 0;
	double timePerPerson = 0;

    for(int i = 0; i < numCities-1; i++)
	{
		a = cities[particles[particle].getPosition(i)];
		b = cities[particles[particle].getPosition(i+1)];

		distance = 3959 * acos(sin(std::get<0>(b.getLocation()))* sin(std::get<0>(a.getLocation()))) + (cos(std::get<0>(b.getLocation()))*cos(std::get<0>(a.getLocation()))*cos(std::get<0>(b.getLocation())-std::get<0>(a.getLocation())));

		travelTime = distance/speed;
		timePerPerson += b.getPop()/travelTime;
	}

	return timePerPerson/numCities;
}