#include "../../include/services/CustomerManager.hpp"
#include "../../include/utils/Parser.hpp"
#include "../../include/models/Vehicle.hpp"
#include <iostream>
#include <iomanip>

// Adds a new customer to the list of customers
void CustomerManager::addCustomer(const Customer& c) {
    customers.push_back(c);
}

// Generates a unique customer ID based on the provided first, middle, and last names
std::string CustomerManager::generateUniqueId(const std::string& f, const std::string& m, const std::string& l) {
    return Parser::generateCustomerId(f, m, l, customers);
}

// Sorts the list of customers based on the specified criteria (1 = Name, 2 = Status, 3 = Rental Expense)
void CustomerManager::sortCustomers(int criteria) {
    customers.sort([criteria](const Customer& a, const Customer& b) {
        if (criteria == 1) return a.getFullName() < b.getFullName();
        if (criteria == 2) return a.rental.isRenting > b.rental.isRenting;
        if (criteria == 3) {
            return a.rental.calculateCurrentCharge() > b.rental.calculateCurrentCharge();
        }
        return false;
    });
}

// Displays a dashboard with the number of vehicles, customers, active customers, and total revenue
void CustomerManager::displayDashboard(int vehicleCount) const {
    int activeCustomers = 0;
    double revenue = 0.0;

    for (const auto& c : customers) {
        if (c.rental.isRenting) {
            ++activeCustomers;
            revenue += c.rental.calculateCurrentCharge();
        }
    }

    std::cout << "\n---Dashboard---\n\n";
    std::cout << "Number of cars: " << vehicleCount << "\n";
    std::cout << "Number of customers: " << customers.size() << "\n";
    std::cout << "Number of active customers: " << activeCustomers << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Revenue: " << revenue << "\n\n";
}

// Ends the rental for a customer based on the provided vehicle plate number
void CustomerManager::displayReport() const {
    std::cout << "-Customer list\n";
    std::cout << std::left << std::setw(10) << "ID" 
              << std::setw(20) << "Name" 
              << std::setw(15) << "Plate" 
              << std::setw(10) << "Status" 
              << std::setw(16) << "Remaining(hrs)"
              << std::setw(12) << "Rate"
              << "Charge\n";
    
    for (const auto& c : customers) {
        bool hasActiveVehicle = c.rental.isRenting && c.rental.vehicle != nullptr;

        std::cout << std::left << std::setw(10) << c.id 
                  << std::setw(20) << c.getFullName() 
                  << std::setw(15) << (hasActiveVehicle ? c.rental.vehicle->getPlate() : "N/A")
                  << std::setw(10) << (hasActiveVehicle ? "Active" : "Inactive")
                  << std::fixed << std::setprecision(2)
                  << std::setw(16) << c.rental.getHoursRemaining()
                  << std::setw(12) << (hasActiveVehicle ? c.rental.vehicle->getRate() : 0.0)
                  << c.rental.calculateCurrentCharge() << "\n";
    }
}

// Ends the rental for a customer based on the provided vehicle plate number
void CustomerManager::endRentalByPlate(const std::string& plate) {
    for (auto& c : customers) {
        if (c.rental.isRenting && c.rental.vehicle && c.rental.vehicle->getPlate() == plate) {
            c.rental.isRenting = false;
            c.rental.vehicle = nullptr;
            return;
        }
    }
}

// Returns a reference to the list of customers
std::list<Customer>& CustomerManager::getCustomers() {
    return customers;
}
