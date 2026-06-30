#include "models/Vehicle.hpp"

Vehicle::Vehicle(std::string p, std::string m, double r) 
    : plateNumber(p), model(m), rate(r), isRented(false) {}

double Sedan::calculateLateFee(int hours) const {
    return hours * (rate * 1.5); 
}

Sedan::Sedan(std::string plate, std::string model, double rate) 
    : Vehicle(plate, model, rate) {}
