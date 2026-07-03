#include "models/Customer.hpp"
#include <iostream>

Customer::Customer(std::string id, std::string f, std::string m, std::string l, std::string lic, std::string cont)
    : id(id), fName(f), mName(m), lName(l), license(lic), contact(cont) {}

std::string Customer::getFullName() const {
    return fName + " " + mName + " " + lName;
}

double RentalInfo::getHoursRemaining() const {
    if (!isRenting) return 0.0;
    std::time_t now = std::time(nullptr);
    return std::difftime(expectedReturnTime, now) / 3600.0;
}
