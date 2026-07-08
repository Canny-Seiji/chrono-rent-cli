#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include <ctime>

class Vehicle; 

// RentalInfo struct to hold rental-related information for a customer
struct RentalInfo {
    bool isRenting = false;
    Vehicle* vehicle = nullptr;
    std::time_t rentStartTime = 0;
    std::time_t expectedReturnTime = 0;
    double completedCharge = 0.0;
    std::string completedPlate;
    double completedRate = 0.0;

    double getHoursRemaining() const;
    double getHoursOverdue() const;
    int getRentalDays() const;
    double calculateCurrentCharge() const;
    double getTotalCharge() const;
};

// Customer class to represent a customer in the rental system
class Customer {
public:
    std::string id, fName, mName, lName, license, contact;
    RentalInfo rental;

    Customer(std::string id, std::string f, std::string m, std::string l, std::string lic, std::string cont);
    
    std::string getFullName() const;
};

#endif
