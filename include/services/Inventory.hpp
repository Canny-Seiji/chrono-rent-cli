#ifndef INVENTORY_H
#define INVENTORY_H

#include "../models/Vehicle.hpp"
#include <list>
#include <fstream> 
#include <iostream>
#include <iomanip>

class Inventory {
private:
    std::list<Vehicle*> fleet;

public:
    ~Inventory();
    void addVehicle(Vehicle* v);
    Vehicle* findVehicle(const std::string& plate);
    void displayFleet() const;
    int getVehicleCount() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};
#endif
