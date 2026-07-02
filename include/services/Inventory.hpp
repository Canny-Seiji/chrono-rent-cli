#ifndef INVENTORY_H
#define INVENTORY_H

#include "models/Vehicle.hpp"
#include <vector>
#include <fstream> 
#include <iostream>
#include <iomanip>

class Inventory {
private:
    std::vector<Vehicle*> fleet;

public:
    ~Inventory();
    void addVehicle(Vehicle* v);
    Vehicle* findVehicle(const std::string& plate);
    void displayFleet() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};
#endif
