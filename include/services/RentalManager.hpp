#ifndef RENTAL_MANAGER_H
#define RENTAL_MANAGER_H

#include "../models/RentalRecord.hpp"
#include "Inventory.hpp"
#include "../utils/Parser.hpp"
#include <queue>
#include <list>

// RentalManager class to manage rental operations in the system
class RentalManager {
private:
    std::list<RentalRecord> activeRentals;
    std::queue<Customer> waitlist;

public:
    void processRental(Customer c, std::string plate, Inventory& inv);
    bool returnVehicle(const std::string& plate);
    void addRentalRecord(const RentalRecord& record);
        
    void saveRentals(const std::string& filename) const;
    void loadRentals(const std::string& filename, Inventory& inv);

    std::list<RentalRecord>& getActiveRentals() { return activeRentals; }
    
};
#endif
