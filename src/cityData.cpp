#include "cityData.h"
#include "dms.h"


city::city()
{
    name = "";
    state = "";
    population = 0;
    area = 0;
    popDensity = 0;

    cords = std::make_tuple(0, 0);
}
city::city(const city &c)
{
    name = c.name;
    state = c.state;
    population = c.population;
    area = c.area;
    popDensity = c.popDensity;
    cords = c.cords;
}
city& city::operator=(const city& c)
{
    if(this== &c)
    {
        return *this;
    }
    this->name = c.name;
    this->state = c.state;
    this->population = c.population;
    this->area = c.area;
    this->popDensity = c.popDensity;
    this->cords = c.cords;

    return *this;
}
city::city(std::string newName, std::string newState, int newPop, double newArea, std::string newLatitude, std::string newLongitude)
{
    name = newName;
    state = newState;
    population = newPop;
    area = newArea;
    popDensity = area/population;

     cords = std::make_tuple(DMS::DecimalDegrees(newLatitude), DMS::DecimalDegrees(newLongitude));

}