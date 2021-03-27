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

city::city(std::string newName, std::string newState, int newPop, double newArea, std::string newLatitude, std::string newLongitude)
{
    name = newName;
    state = newState;
    population = newPop;
    area = newArea;
    popDensity = area/population;

     cords = std::make_tuple(DMS::DecimalDegrees(newLatitude), DMS::DecimalDegrees(newLongitude));

}