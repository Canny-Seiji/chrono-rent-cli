#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <string>

class Vehicle {
protected:
    std::string plateNumber;
    std::string model;
    double rate;
    bool isRented;

public:
    Vehicle(std::string p, std::string m, double r); 
    virtual ~Vehicle() = default;

    bool getRentedStatus() const { return isRented; }
    void setRentedStatus(bool status) { isRented = status; }
    double getRate() const { return rate; }
    std::string getPlate() const { return plateNumber; }
    
    virtual double calculateLateFee(int hours) const = 0; 
};

class Sedan : public Vehicle {
public:
    Sedan(std::string plate, std::string model, double rate);
    double calculateLateFee(int hours) const override;
};

#endif 
