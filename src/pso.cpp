#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <omp.h>
#include <random>
#include <algorithm>
#include <climits>

#include "CStopWatch.h"
#include "cityData.h"
#include "particle.h"


template<typename T>
using vec = std::vector<T>;

void rearrange(int index, vec<particle>& particles, std::uniform_int_distribution<int> indexes, std::mt19937 rng)
{
	int a = indexes(rng);
	int b = indexes(rng);
	int tmp;

	tmp = particles[index].getPosition(b);
	particles[index].setPosition(b, particles[index].getPosition(a));
	particles[index].setPosition(a, tmp);
}



void PSO(const int Np, const int Nd, const int Nt, const int xMin, const int xMax, const double vMin, const double vMax,
		 double (*objFunc)(vec<particle>, int, int, vec<city>, int), std::vector<city> cities, int speed, int numThreads)
{
	//Random distributions
	std::uniform_int_distribution<int> pos(xMin, xMax);
	std::uniform_real_distribution<double> vel(vMin, vMax);
	std::uniform_real_distribution<double> zeroOne(0.0, 1.0);
	std::uniform_int_distribution<int> indexes(0,Nd-1);


	vec<particle> particles;

	vec<int> gBestPosition(Nd, INT_MIN);
	double gBestValue = -INFINITY;

	int bestTimeStep = 0;


	int numEvals = 0;

	double worstResults = 0;
	double vValue = 0;

	int changes = 0;
	CStopWatch timer;

	timer.startTimer();
	#pragma omg parallel private(vValue) reduction(+:numEvals)
	{
		std::random_device rd;
		std::mt19937 rng(rd());

		//Init Population to randomized indexes of cities
		vec<int> temp(Nd, 0);

		#pragma omp single
		{
			for( int p = 0; p < Np; p++)
			{
				std::iota (temp.begin(), temp.end(), 0);
				std::shuffle(temp.begin(), temp.end(), rd);
				particles.push_back(particle(temp));
			}
		}


		//Evaluate initial fitness
		#pragma omp for
		for( int p = 0; p < Np; p++)
		{
			particles[p].setFitness(objFunc(particles, Nd, p, cities, speed));
			numEvals++;
		}


		//Loop for Nt iterations
		for( int t = 0; t < Nt; t++)
		{
			#pragma omp single
			{
				if(t%100 == 0)
				{
					std::cout<< "Elapsed Time: " << timer.getElapsedTimeInSec() << "s, Time Step: " << t << "/" << Nt << "\n";
				}
			}

			//Update Positions
			for( int p = 1; p < Np; p++)
			{
				changes = std::floor(std::abs(particles[p].getVelocity()));
				for(int i = 0; i < changes; i++)
				{
					if(zeroOne(rng) > 0.5)
					{
						rearrange(p, particles, indexes, rng);
					}

					std::swap(particles[p], particles[p-1]);
				}
			}

			//Evaluate Fitness
			#pragma omp for
			for( int p = 0; p < Np; p++)
			{
				particles[p].setFitness(objFunc(particles, Nd, p, cities, speed));
				numEvals++;
			}

			//Update Bests
			#pragma omp single
			{
				for (int p = 0; p < Np; p++)
				{
					if (particles[p].getFitness() > gBestValue)
					{
						gBestValue = particles[p].getFitness();
						for (int i = 0; i < Nd; i++)
						{
							gBestPosition[i] = particles[p].getPosition(i);
						}
						bestTimeStep = t;
					}

					// Local
					if (particles[p].getFitness() > particles[p].getBestValue())
					{
						particles[p].setBestValue(particles[p].getFitness());
						for (int i = 0; i < Nd; i++)
						{
							particles[p].setBestPosition(i, particles[p].getPosition(i));
						}
					}
				}
			}
			//Update Velocities
			#pragma omp single
			{
				std::sort(particles.begin(),particles.end(), std::greater <>());
				worstResults = particles[Np-1].getBestValue();
			}
			vValue = 0.0;
			#pragma omp for
			for( int p = 0; p < Np; p++)
			{
				vValue = (vMax * particles[p].getBestValue()) / worstResults;
				if (vValue > vMax)
				{
					particles[p].setVelocity(vMax);
				}
				else if (vValue < vMin)
				{
					particles[p].setVelocity(vMin);
				}
				else
				{
					particles[p].setVelocity(vValue);
				}
			}


		}
	}
	timer.stopTimer();
	//Display results

	std::cout << "\n\nBest order of cities to distribute vaccine\n\n";
	for(int c = 0; c < Nd; c++)
	{
		std::cout << c+1 << ". " << cities[gBestPosition[c]].getName() << ", " << cities[gBestPosition[c]].getState() << "\n";
	}
	std::cout << "\nNumber of vaccines distributed per hour: " << gBestValue << "\n\n";

	std::cout << "This result was found on step " << bestTimeStep << " and took " << timer.getElapsedTimeInSec()  << " seconds to find\n\n";

	particles.clear();
	gBestPosition.clear();
}