#include "services/CustomerManager.hpp"
#include "utils/Parser.hpp"
#include "models/Vehicle.hpp"
#include <iostream>
#include <iomanip>

void CustomerManager::addCustomer(const Customer& c) {
    customers.push_back(c);
}

std::string CustomerManager::generateUniqueId(const std::string& f, const std::string& m, const std::string& l) {
    return Parser::generateCustomerId(f, m, l, customers);
}

void CustomerManager::sortCustomers(int criteria) {
    std::sort(customers.begin(), customers.end(), [criteria](const Customer& a, const Customer& b) {
        if (criteria == 1) return a.id > b.id; // Assuming ID is alphabetical/sequential
        if (criteria == 2) return a.rental.isRenting > b.rental.isRenting; // Active first
        if (criteria == 3) {
            double rateA = (a.rental.vehicle) ? a.rental.vehicle->getRate() : 0.0;
            double rateB = (b.rental.vehicle) ? b.rental.vehicle->getRate() : 0.0;
            return rateA > rateB;
        }
        return false;
    });
}

void CustomerManager::displayReport() const {
    std::cout << std::left << std::setw(10) << "ID" 
              << std::setw(20) << "Name" 
              << std::setw(15) << "Vehicle" 
              << std::setw(10) << "Status" 
              << "Remaining(hrs)\n";
    
    for (const auto& c : customers) {
        std::cout << std::left << std::setw(10) << c.id 
                  << std::setw(20) << c.getFullName() 
                  << std::setw(15) << (c.rental.isRenting ? c.rental.vehicle->getPlate() : "N/A")
                  << std::setw(10) << (c.rental.isRenting ? "Active" : "Inactive")
                  << c.rental.getHoursRemaining() << "\n";
    }
}

std::vector<Customer>& CustomerManager::getCustomers() {
    return customers;
}
