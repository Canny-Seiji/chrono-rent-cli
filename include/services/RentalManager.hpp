#ifndef RENTAL_MANAGER_H
#define RENTAL_MANAGER_H

#include "models/RentalRecord.hpp"
#include "services/Inventory.hpp"
#include "utils/Parser.hpp"
#include <queue>
#include <vector>

class RentalManager {
private:
    std::vector<RentalRecord> activeRentals;
    std::queue<Customer> waitlist;

public:
    void processRental(Customer c, std::string plate, Inventory& inv);
    void addToWaitlist(Customer c);
    void returnVehicle(const std::string& plate);
    
    void saveRentals(const std::string& filename) const;
    void loadRentals(const std::string& filename, Inventory& inv);

    std::vector<RentalRecord>& getActiveRentals() { return activeRentals; }
    
};
#endif