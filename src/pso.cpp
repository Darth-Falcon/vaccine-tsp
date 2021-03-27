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


template<typename T>
using vec = std::vector<T>;

void PSO(const int Np, const int Nd, const int Nt, const int xMin, const int xMax, const float vMin, const float vMax,
		 double (*objFunc)(vec<vec<int>>, int, int, vec<city>, int), std::vector<city> cities, int speed, int numThreads)
{
	//Random distributions
	std::uniform_int_distribution<int> pos(xMin, xMax);
	std::uniform_real_distribution<float> vel(vMin, vMax);
	std::uniform_real_distribution<float> zeroOne(0.0, 1.0);


	vec<vec<int>> R(Np, vec<int>(Nd, 0));
	vec<vec<double>> V(Np, vec<double>(Nd, 0));
	vec<double> M(Np, 0);

	vec<vec<int>>pBestPosition(Np, vec<int>(Nd, INT_MIN));
	vec<double> pBestValue(Np, -INFINITY);

	vec<int> gBestPosition(Nd, INT_MIN);
	double gBestValue = -INFINITY;

	int bestTimeStep = 0;

	const double C1 = 2.05, C2 = 2.05;
	double w;
	const double wMax = 0.9, wMin = 0.1;
	double R1, R2;

	int numEvals = 0;

	

	double gBest = -1;
	std::random_device rd;
	std::mt19937 rng(rd());

	CStopWatch timer;

	timer.startTimer();

	//Init Population to randomized indexes of cities
	for( int p = 0; p < Np; p++)
	{
		std::iota (R[p].begin(), R[p].end(), 0);
		std::shuffle(R[p].begin(), R[p].end(), rd);
	}


	//Evaluate initial fitness
	for( int p = 0; p < Np; p++)
	{
		M[p] = objFunc(R, Nd, p, cities, speed);
		numEvals++;
	}


	//Loop for Nt iterations
	for( int t = 0; t < Nt; t++)
	{
		if(t%100 == 0)
		{
			std::cout<< "Elapsed Time: " << timer.getElapsedTimeInSec() << "s, Time Step: " << t << "/" << Nt << "\n";
		}
		//Update Positions
		



		//Evaluate Fitness
		for( int i = 0; i < Np; i++)
		{
			M[i] = objFunc(R, Nd, i, cities, speed);
			numEvals++;
		}

		//Update Bests
		for (int p = 0; p < Np; p++)
		{
			if (M[p] > gBestValue)
			{
				gBestValue = M[p];

				for (int i = 0; i < Nd; i++)
				{
					gBestPosition[i] = R[p][i];
				}
				bestTimeStep = t;
			}

			// Local
			if (M[p] > pBestValue[p])
			{
				pBestValue[p] = M[p];
				for (int i = 0; i < Nd; i++)
				{
					pBestPosition[p][i] = R[p][i];
				}
			}
		}

		//Update Velocities
		w = wMax - ((wMax - wMin) / Nt) * t;
		
		for (int p = 0; p < Np; p++)
		{
			for (int i = 0; i < Nd; i++)
			{
				R2 = zeroOne(rng);
				R1 = zeroOne(rng);

				// Original PSO
				V[p][i] = w * V[p][i] + C1 * R1 * (pBestPosition[p][i] - R[p][i]) + C2 * R2 * (gBestPosition[i] - R[p][i]);

				if (V[p][i] > vMax) { V[p][i] = vMax;}
				else if(V[p][i] < vMin) {V[p][i] = vMin;}
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


	R.clear();
	V.clear();
	M.clear();
	pBestPosition.clear();
	pBestValue.clear();
	gBestPosition.clear();
}