#include "../../include/services/CustomerManager.hpp"
#include "../../include/services/Analytics.hpp"
#include "../../include/utils/Colors.hpp"
#include "../../include/utils/Parser.hpp"
#include "../../include/models/Vehicle.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

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
            return a.rental.getTotalCharge() > b.rental.getTotalCharge();
        }
        return false;
    });
}

// Displays a dashboard with the number of vehicles, customers, active customers, and total revenue
void CustomerManager::displayDashboard(int vehicleCount) const {
    int activeCustomers = 0;

    for (const auto& c : customers) {
        if (c.rental.isRenting) {
            ++activeCustomers;
        }
    }

    double netRevenue = Analytics::calculateNetRevenue(customers);
    std::string netRevenueColor = netRevenue < 0 ? Colors::RED : (netRevenue > 0 ? Colors::GREEN : Colors::RESET);

    std::cout << Colors::YELLOW << std::string(50, '=') << "\n" << Colors::RESET;
    std::cout << Colors::YELLOW << "                 System Dashboard" << Colors::RESET << "\n";
    std::cout << Colors::YELLOW << std::string(50, '=') << "\n" << Colors::RESET;

    std::cout << Colors::CYAN << "Number of cars: " << Colors::RESET << vehicleCount << "\n";
    std::cout << Colors::CYAN << "Number of customers: " << Colors::RESET << customers.size() << "\n";
    std::cout << Colors::CYAN << "Number of active customers: " << Colors::RESET << activeCustomers << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << Colors::CYAN << "Net Revenue: " << Colors::RESET
              << netRevenueColor << netRevenue << Colors::RESET << "\n\n";
}

// Displays a detailed report of all customers, including their ID, name, vehicle plate, rental status, remaining hours, rate, and current charge
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
        std::string plate = hasActiveVehicle ? c.rental.vehicle->getPlate() : (c.rental.completedPlate.empty() ? "N/A" : c.rental.completedPlate);
        std::string status = hasActiveVehicle ? "Active" : "Inactive";
        std::string remaining = formatDouble(c.rental.getHoursRemaining());
        std::string rate = formatDouble(hasActiveVehicle ? c.rental.vehicle->getRate() : c.rental.completedRate);
        std::string charge = formatDouble(c.rental.getTotalCharge());

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

    std::cout << Colors::YELLOW << "--- Customer list\n";
    std::cout << std::left << std::setw(wId) << "ID"
              << std::setw(wName) << "Name"
              << std::setw(wPlate) << "Plate"
              << std::setw(wStatus) << "Status"
              << std::setw(wRemaining) << "Remaining(hrs)"
              << std::setw(wRate) << "Rate"
              << std::setw(wCharge) << "Charge" << "\n"
              << Colors::RESET;

    std::cout << std::string(wId + wName + wPlate + wStatus + wRemaining + wRate + wCharge, '-') << "\n";

    for (const auto& c : customers) {
        bool hasActiveVehicle = c.rental.isRenting && c.rental.vehicle != nullptr;
        double totalCharge = c.rental.getTotalCharge();
        
        std::string plate = hasActiveVehicle ? c.rental.vehicle->getPlate() : (c.rental.completedPlate.empty() ? "N/A" : c.rental.completedPlate);
        std::string status = hasActiveVehicle ? "Active" : "Inactive";
        std::string remaining = formatDouble(c.rental.getHoursRemaining());
        std::string rate = formatDouble(hasActiveVehicle ? c.rental.vehicle->getRate() : c.rental.completedRate);
        std::string charge = formatDouble(totalCharge);

        std::string statusColor = hasActiveVehicle ? Colors::GREEN : Colors::RED;
        std::string chargeColor = totalCharge < 0 ? Colors::RED : (totalCharge > 0 ? Colors::GREEN : Colors::RESET);

        std::cout << std::left << std::setw(wId) << c.id
                  << std::setw(wName) << c.getFullName()
                  << std::setw(wPlate) << plate;
        std::cout << statusColor << std::setw(wStatus) << status << Colors::RESET;
        std::cout << std::setw(wRemaining) << remaining
                  << std::setw(wRate) << rate
                  << chargeColor << std::setw(wCharge) << charge << Colors::RESET << "\n";
    }
}


// Ends the rental for a customer based on the provided vehicle plate number
void CustomerManager::endRentalByPlate(const std::string& plate) {
    for (auto& c : customers) {
        if (c.rental.isRenting && c.rental.vehicle && c.rental.vehicle->getPlate() == plate) {
            c.rental.completedCharge += c.rental.calculateCurrentCharge();
            c.rental.completedPlate = c.rental.vehicle->getPlate();
            c.rental.completedRate = c.rental.vehicle->getRate();
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

bool CustomerManager::loadCustomers(const std::string& filename, Inventory& inventory) {
    std::ifstream inFile(filename);
    std::string line;

    while (std::getline(inFile, line)) {
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() != 13) continue;

        try {
            Customer customer(data[0], data[1], data[2], data[3], data[4], data[5]);
            customer.rental.isRenting = data[6] == "1";
            customer.rental.rentStartTime = static_cast<std::time_t>(std::stoll(data[8]));
            customer.rental.expectedReturnTime = static_cast<std::time_t>(std::stoll(data[9]));
            customer.rental.completedCharge = std::stod(data[10]);
            customer.rental.completedPlate = data[11];
            customer.rental.completedRate = std::stod(data[12]);

            if (customer.rental.isRenting) {
                customer.rental.vehicle = inventory.findVehicle(data[7]);
                if (customer.rental.vehicle == nullptr) continue;
                customer.rental.vehicle->setRentedStatus(true);
            }

            customers.push_back(customer);
        } catch (const std::exception&) {
            continue;
        }
    }

    return !customers.empty();
}

void CustomerManager::saveCustomers(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) return;

    outFile << std::setprecision(17);
    for (const auto& customer : customers) {
        std::string activePlate = customer.rental.isRenting && customer.rental.vehicle
            ? customer.rental.vehicle->getPlate()
            : "";

        outFile << customer.id << "|"
                << customer.fName << "|"
                << customer.mName << "|"
                << customer.lName << "|"
                << customer.license << "|"
                << customer.contact << "|"
                << (customer.rental.isRenting ? 1 : 0) << "|"
                << activePlate << "|"
                << customer.rental.rentStartTime << "|"
                << customer.rental.expectedReturnTime << "|"
                << customer.rental.completedCharge << "|"
                << customer.rental.completedPlate << "|"
                << customer.rental.completedRate << "\n";
    }
}

// Returns a reference to the list of customers
std::list<Customer>& CustomerManager::getCustomers() {
    return customers;
}
