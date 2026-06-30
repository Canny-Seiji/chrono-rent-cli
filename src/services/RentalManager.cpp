#include "services/RentalManager.hpp"
#include <iostream>

void RentalManager::processRental(Customer c, std::string plate, Inventory& inv) {
    Vehicle* v = inv.findVehicle(plate);
    if (v && !v->getRentedStatus()) {
        v->setRentedStatus(true);
        activeRentals.emplace_back("TX-001", c, v);
        std::cout << "Rental successful for: " << c.getFullName() << "\n";
    } else {
        std::cout << "Vehicle unavailable or not found. Adding to waitlist...\n";
        addToWaitlist(c);
    }
}

void RentalManager::addToWaitlist(Customer c) {
    waitlist.push(c);
}

void RentalManager::saveRentals(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto& record : activeRentals) {
        // Format: RecordID|CustomerFullName|VehiclePlate
        outFile << record.recordId << "|" 
                << record.customer.getFullName() << "|" 
                << record.vehicle->getPlate() << "\n";
    }
}

void RentalManager::loadRentals(const std::string& filename, Inventory& inv) {
    std::ifstream inFile(filename);
    std::string line;
    while (std::getline(inFile, line)) {
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() == 3) {
            Vehicle* v = inv.findVehicle(data[2]);
            if (v) {
                // Here you would typically reconstruct the customer object
                // For simplicity in this lab, we link the existing vehicle
                activeRentals.emplace_back(data[0], Customer("","","","", "", ""), v);
            }
        }
    }
}

void RentalManager::returnVehicle(const std::string& plate) {
    for (size_t i = 0; i < activeRentals.size(); ++i) {
        if (activeRentals[i].vehicle->getPlate() == plate) {
            activeRentals[i].vehicle->setRentedStatus(false);
            activeRentals.erase(activeRentals.begin() + i);
            std::cout << "Vehicle " << plate << " returned successfully.\n";
            return;
        }
    }
    std::cout << "Error: Rental record not found for plate: " << plate << "\n";
}