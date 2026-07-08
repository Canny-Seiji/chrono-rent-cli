#ifndef RENTAL_RECORD_H
#define RENTAL_RECORD_H

#include "Customer.hpp"
#include "Vehicle.hpp"

// RentalRecord class to represent a rental transaction in the system
class RentalRecord {
public:
    std::string recordId;
    Customer customer;
    Vehicle* vehicle; 

    RentalRecord(std::string id, Customer cust, Vehicle* veh);
};

#endif
