#include "models/RentalRecord.hpp"

RentalRecord::RentalRecord(std::string id, Customer cust, Vehicle* veh)
    : recordId(id), customer(cust), vehicle(veh) {}
