#include "../../include/models/RentalRecord.hpp"

// Constructor for the RentalRecord class
RentalRecord::RentalRecord(std::string id, Customer cust, Vehicle* veh)
    : recordId(id), customer(cust), vehicle(veh) {}
