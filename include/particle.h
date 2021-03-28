#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

template<typename T>
using vec = std::vector<T>;

class particle
{
    public:
    particle();
    particle(vec<int>, double);
    int getPosition(int x) {return position[x];}
    double getVelocity() {return velocity;}
    double getFitness() {return fitness;}
    int getBestPosition(int x){return bestPosition[x];}
    double getBestValue(){return bestValue;}
    vec<int> getAllPositions() {return position;}

    void setPosition(int, int);
    void setAllPositions(vec<int>);
    void setVelocity(double);
    void setFitness(double);
    void setBestPosition(int, int);
    void setBestValue(double);
    void updateBest();

    bool operator<(const particle&);
    bool operator>(const particle&);
    bool operator==(const particle&);


    private:
    vec<int> position;
	double velocity;
	double fitness;
    vec<int> bestPosition;
    double bestValue;

};

#endif