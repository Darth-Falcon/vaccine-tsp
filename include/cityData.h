#include <fstream>
#include <vector>
#include <string>
#include <tuple> 


#ifndef CITY_H
#define CITY_H

class city
{
    public:
    city();
    city(const city &c);
    city(std::string, std::string, int, double, std::string, std::string);
    //getters
    std::string getName() {return name;}
    std::string getState() {return state;}
    int getPop() {return population;}
    double getArea() {return area;}
    double getDensity() {return popDensity;}
    std::tuple<double,double> getLocation() {return cords;}

    city& operator=(const city& c);


    private:
    std::string name;
    std::string state;
    int population; 
    double area;
    double popDensity;
    std::tuple<double,double> cords;
};

#endif