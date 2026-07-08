#include "../../include/models/Vehicle.hpp"

// Constructor for the Vehicle class
Vehicle::Vehicle(std::string m, std::string p, double r) 
    : model(m), plateNumber(p), rate(r), isRented(false) {}
