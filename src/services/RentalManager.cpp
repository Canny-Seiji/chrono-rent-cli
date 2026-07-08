#include "../../include/services/RentalManager.hpp"
#include <iomanip>
#include <iostream>

// Processes a rental for a customer, checking vehicle availability and updating records
void RentalManager::processRental(Customer c, std::string plate, Inventory& inv) {
    Vehicle* v = inv.findVehicle(plate);
    if (v && !v->getRentedStatus()) {
        v->setRentedStatus(true);
        c.rental.isRenting = true;
        c.rental.vehicle = v;
        c.rental.rentStartTime = std::time(nullptr);
        c.rental.expectedReturnTime = c.rental.rentStartTime + 86400;
        activeRentals.emplace_back("TX-001", c, v);
        std::cout << "Rental successful for: " << c.getFullName() << "\n";
    } else {
        std::cout << "Vehicle not available for rental.\n";
    }
}

// Saves the active rental records to a file
void RentalManager::saveRentals(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto& record : activeRentals) {
        // Format: RecordID|CustomerFullName|VehiclePlate
        outFile << record.recordId << "|" 
                << record.customer.getFullName() << "|" 
                << record.vehicle->getPlate() << "\n";
    }
}

// Loads rental records from a file and associates them with vehicles in the inventory
void RentalManager::addRentalRecord(const RentalRecord& record) {
    activeRentals.push_back(record);
}

// Loads rental records from a file and associates them with vehicles in the inventory
void RentalManager::loadRentals(const std::string& filename, Inventory& inv) {
    std::ifstream inFile(filename);
    std::string line;
    int loadedCount = 1;

    while (std::getline(inFile, line)) {
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() == 3) {
            Vehicle* v = inv.findVehicle(data[2]);
            if (v) {
                std::string customerName = data[1].empty() ? "Unknown Customer" : data[1];
                std::vector<std::string> nameParts = Parser::split(customerName, ' ');

                std::string firstName = nameParts.empty() ? "Unknown" : nameParts.front();
                std::string middleName = "";
                std::string lastName = nameParts.size() > 1 ? nameParts.back() : "Customer";

                if (nameParts.size() > 2) {
                    for (size_t i = 1; i + 1 < nameParts.size(); ++i) {
                        if (!middleName.empty()) middleName += " ";
                        middleName += nameParts[i];
                    }
                }

                Customer loadedCustomer("LOAD" + std::to_string(loadedCount++), firstName, middleName, lastName, "", "");
                loadedCustomer.rental.isRenting = true;
                loadedCustomer.rental.vehicle = v;
                loadedCustomer.rental.rentStartTime = std::time(nullptr);
                loadedCustomer.rental.expectedReturnTime = loadedCustomer.rental.rentStartTime + 86400;

                v->setRentedStatus(true);
                activeRentals.emplace_back(data[0], loadedCustomer, v);
            }
        }
    }
}

// Returns a pointer to a rental record by its ID, or nullptr if not found
bool RentalManager::returnVehicle(const std::string& plate) {
    for (auto it = activeRentals.begin(); it != activeRentals.end(); ++it) {
        if (it->vehicle->getPlate() == plate) {
            double totalCharge = it->customer.rental.calculateCurrentCharge();
            double overdueHours = it->customer.rental.getHoursOverdue();

            it->vehicle->setRentedStatus(false);
            activeRentals.erase(it);

            std::cout << Colors::GREEN << "Vehicle " << plate << " returned successfully.\n" << Colors::RESET;
            std::cout << std::fixed << std::setprecision(2)
                      << "Overdue hours: " << overdueHours << "\n"
                      << "Total charge: " << totalCharge << "\n";
            return true;
        }
    }
    std::cout << Colors::RED << "Error: Rental record not found for plate: " << plate << "\n" << Colors::RESET;
    return false;
}
