#include "../../include/models/Customer.hpp"
#include "../../include/models/Vehicle.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

Customer::Customer(std::string id, std::string f, std::string m, std::string l, std::string lic, std::string cont)
    : id(id), fName(f), mName(m), lName(l), license(lic), contact(cont) {}

std::string Customer::getFullName() const {
    return fName + " " + mName + " " + lName;
}

double RentalInfo::getHoursRemaining() const {
    if (!isRenting) return 0.0;
    if (expectedReturnTime <= 0) return 0.0;
    std::time_t now = std::time(nullptr);
    return std::max(0.0, std::difftime(expectedReturnTime, now) / 3600.0);
}

double RentalInfo::getHoursOverdue() const {
    if (!isRenting || expectedReturnTime <= 0) return 0.0;
    std::time_t now = std::time(nullptr);
    return std::max(0.0, std::difftime(now, expectedReturnTime) / 3600.0);
}

int RentalInfo::getRentalDays() const {
    if (rentStartTime <= 0 || expectedReturnTime <= rentStartTime) return 1;

    double seconds = std::difftime(expectedReturnTime, rentStartTime);
    return std::max(1, static_cast<int>(std::ceil(seconds / 86400.0)));
}

double RentalInfo::calculateCurrentCharge() const {
    if (!isRenting || vehicle == nullptr) return 0.0;

    double baseCharge = vehicle->getRate() * getRentalDays();
    int lateHours = static_cast<int>(std::ceil(getHoursOverdue()));
    return baseCharge + vehicle->calculateLateFee(lateHours);
}
