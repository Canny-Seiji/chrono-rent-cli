#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include <ctime>

// Forward declaration to avoid circular dependencies
class Vehicle; 

struct RentalInfo {
    bool isRenting = false;
    Vehicle* vehicle = nullptr;
    std::time_t rentStartTime = 0;
    std::time_t expectedReturnTime = 0;

    double getHoursRemaining() const;
    double getHoursOverdue() const;
    int getRentalDays() const;
    double calculateCurrentCharge() const;
};

class Customer {
public:
    std::string id, fName, mName, lName, license, contact;
    RentalInfo rental;

    Customer(std::string id, std::string f, std::string m, std::string l, std::string lic, std::string cont);
    
    std::string getFullName() const;
};

#endif
