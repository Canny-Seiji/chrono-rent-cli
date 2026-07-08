#include "../../include/models/Customer.hpp"
#include "../../include/models/Vehicle.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// Constructor for the Customer class
Customer::Customer(std::string id, std::string f, std::string m, std::string l, std::string lic, std::string cont)
    : id(id), fName(f), mName(m), lName(l), license(lic), contact(cont) {}

std::string Customer::getFullName() const {
    return fName + " " + mName + " " + lName;
}

// RentalInfo methods to calculate rental-related information
double RentalInfo::getHoursRemaining() const {
    if (!isRenting) return 0.0;
    if (expectedReturnTime <= 0) return 0.0;
    std::time_t now = std::time(nullptr);
    return std::max(0.0, std::difftime(expectedReturnTime, now) / 3600.0);
}

// Calculate the number of hours overdue for a rental
double RentalInfo::getHoursOverdue() const {
    if (!isRenting || expectedReturnTime <= 0) return 0.0;
    std::time_t now = std::time(nullptr);
    return std::max(0.0, std::difftime(now, expectedReturnTime) / 3600.0);
}

// Calculate the number of rental days based on the rental start and expected return times
int RentalInfo::getRentalDays() const {
    if (rentStartTime <= 0 || expectedReturnTime <= rentStartTime) return 1;

    double seconds = std::difftime(expectedReturnTime, rentStartTime);
    return std::max(1, static_cast<int>(std::ceil(seconds / 86400.0)));
}

// Calculate the current charge for the rental, including base charge and any late fees
double RentalInfo::calculateCurrentCharge() const {
    if (!isRenting || vehicle == nullptr) return 0.0;

    double baseCharge = vehicle->getRate() * getRentalDays();
    int lateHours = static_cast<int>(std::ceil(getHoursOverdue()));
    return baseCharge + vehicle->calculateLateFee(lateHours);
}
