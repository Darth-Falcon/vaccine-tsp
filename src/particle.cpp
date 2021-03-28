    #include "particle.h"    
    #include <climits>
    #include <math.h>
    
    template<typename T>
    using vec = std::vector<T>;
    
    particle::particle(vec<int> newPositions, double newVelocity)
    {
        position = newPositions;
        velocity = newVelocity;
        fitness = INFINITY;
        bestPosition = vec<int>(position.size(), INT_MIN);
        bestValue = -INFINITY;
    }
    void particle::setPosition(int dimension, int newPosition)
    {
        position[dimension] = newPosition;
    }
    void particle::setAllPositions(vec<int> newPositions)
    {
        position = newPositions;
    }
    void particle::setVelocity(double newVelocity)
    {
        velocity = newVelocity;
    }
    void particle::setFitness(double newFitness)
    {
        fitness = newFitness;
    }
    void particle::setBestPosition(int dimension, int newBestPosition)
    {
        bestPosition[dimension] = newBestPosition;
    }
    void particle::setBestValue(double newBestValue)
    {
        bestValue = newBestValue;
    }
    void particle::updateBest()
    {
        if(fitness > bestValue)
        {
            bestValue = fitness;
            bestPosition = position;
        }
    }

    bool particle::operator<(const particle& p)
    {
        return bestValue < p.bestValue;
    }
    bool particle::operator>(const particle& p)
    {
        return !(bestValue < p.bestValue);
    }
    bool particle::operator==(const particle& p)
    {
        return (bestValue==p.bestValue);
    }