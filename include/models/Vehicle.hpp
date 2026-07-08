#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>

// Vehicle class to represent a vehicle in the rental system
class Vehicle {
protected:
    std::string model;
    std::string plateNumber;
    double rate;
    bool isRented;

public:
    Vehicle(std::string m, std::string p, double r); 
    virtual ~Vehicle() = default;
    
    std::string getModel() const { return model; }
    std::string getPlate() const { return plateNumber; }
    double getRate() const { return rate; }
    bool getRentedStatus() const { return isRented; }
    void setRentedStatus(bool status) { isRented = status; }

    virtual double calculateLateFee(int hours) const = 0; 
};

// Car class derived from Vehicle, representing a specific type of vehicle
class Car : public Vehicle {
public:
    Car(std::string m, std::string p, double r) : Vehicle(m, p, r) {}

    double calculateLateFee(int hours) const override {
        return hours * 15.0; 
    }
};
#endif 
