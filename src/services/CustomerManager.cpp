#include "../../include/services/CustomerManager.hpp"
#include "../../include/utils/Colors.hpp"
#include "../../include/utils/Parser.hpp"
#include "../../include/models/Vehicle.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

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

    std::string revenueColor = revenue < 0 ? Colors::RED : (revenue > 0 ? Colors::GREEN : Colors::RESET);

    std::cout << "\n" << Colors::YELLOW << "---Dashboard---" << Colors::RESET << "\n\n";
    std::cout << Colors::YELLOW << "Number of cars: " << Colors::RESET << vehicleCount << "\n";
    std::cout << Colors::YELLOW << "Number of customers: " << Colors::RESET << customers.size() << "\n";
    std::cout << Colors::YELLOW << "Number of active customers: " << Colors::RESET << activeCustomers << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << Colors::YELLOW << "Revenue: " << Colors::RESET
              << revenueColor << revenue << Colors::RESET << "\n\n";
}

// Ends the rental for a customer based on the provided vehicle plate number
void CustomerManager::displayReport() const {
    auto formatDouble = [](double value) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << value;
        return out.str();
    };

    size_t wId = std::string("ID").length();
    size_t wName = std::string("Name").length();
    size_t wPlate = std::string("Plate").length();
    size_t wStatus = std::string("Status").length();
    size_t wRemaining = std::string("Remaining(hrs)").length();
    size_t wRate = std::string("Rate").length();
    size_t wCharge = std::string("Charge").length();

    for (const auto& c : customers) {
        bool hasActiveVehicle = c.rental.isRenting && c.rental.vehicle != nullptr;
        std::string plate = hasActiveVehicle ? c.rental.vehicle->getPlate() : "N/A";
        std::string status = hasActiveVehicle ? "Active" : "Inactive";
        std::string remaining = formatDouble(c.rental.getHoursRemaining());
        std::string rate = formatDouble(hasActiveVehicle ? c.rental.vehicle->getRate() : 0.0);
        std::string charge = formatDouble(c.rental.calculateCurrentCharge());

        wId = std::max(wId, c.id.length());
        wName = std::max(wName, c.getFullName().length());
        wPlate = std::max(wPlate, plate.length());
        wStatus = std::max(wStatus, status.length());
        wRemaining = std::max(wRemaining, remaining.length());
        wRate = std::max(wRate, rate.length());
        wCharge = std::max(wCharge, charge.length());
    }

    const size_t padding = 4;
    wId += padding;
    wName += padding;
    wPlate += padding;
    wStatus += padding;
    wRemaining += padding;
    wRate += padding;
    wCharge += padding;

    std::cout << Colors::YELLOW << "-Customer list\n";
    std::cout << std::left << std::setw(wId) << "ID"
              << std::setw(wName) << "Name"
              << std::setw(wPlate) << "Plate"
              << std::setw(wStatus) << "Status"
              << std::setw(wRemaining) << "Remaining(hrs)"
              << std::setw(wRate) << "Rate"
              << std::setw(wCharge) << "Charge" << "\n"
              << Colors::RESET;
    
    for (const auto& c : customers) {
        bool hasActiveVehicle = c.rental.isRenting && c.rental.vehicle != nullptr;
        double currentCharge = c.rental.calculateCurrentCharge();
        std::string chargeColor = currentCharge < 0 ? Colors::RED : (currentCharge > 0 ? Colors::GREEN : Colors::RESET);
        std::string plate = hasActiveVehicle ? c.rental.vehicle->getPlate() : "N/A";
        std::string status = hasActiveVehicle ? "Active" : "Inactive";
        std::string remaining = formatDouble(c.rental.getHoursRemaining());
        std::string rate = formatDouble(hasActiveVehicle ? c.rental.vehicle->getRate() : 0.0);
        std::string charge = formatDouble(currentCharge);

        std::cout << std::left << std::setw(wId) << c.id
                  << std::setw(wName) << c.getFullName()
                  << std::setw(wPlate) << plate
                  << std::setw(wStatus) << status
                  << std::setw(wRemaining) << remaining
                  << std::setw(wRate) << rate
                  << chargeColor << std::setw(wCharge) << charge << Colors::RESET << "\n";
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

bool CustomerManager::contactExists(const std::string& contact) const {
    for (const auto& c : customers) {
        if (c.contact == contact) {
            return true;
        }
    }
    return false;
}

// Returns a reference to the list of customers
std::list<Customer>& CustomerManager::getCustomers() {
    return customers;
}
