#include "../../include/models/Vehicle.hpp"

Vehicle::Vehicle(std::string m, std::string p, double r) 
    : model(m), plateNumber(p), rate(r), isRented(false) {}
