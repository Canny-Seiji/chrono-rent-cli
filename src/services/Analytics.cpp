#include "../../include/services/Analytics.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

// Sorts the rental records by their recordId in ascending order
void Analytics::sortByUsage(std::list<RentalRecord>& records) {
    records.sort([](const RentalRecord& a, const RentalRecord& b) {
        return a.recordId < b.recordId;
    });
}

// Finds a rental record by its ID and returns a pointer to it, or nullptr if not found
RentalRecord* Analytics::findRecordById(std::list<RentalRecord>& records, const std::string& id) {
    for (auto& record : records) {
        if (record.recordId == id) return &record;
    }
    return nullptr;
}

double Analytics::calculateRevenue(const std::list<Customer>& customers) {
    double revenue = 0.0;

    for (const auto& customer : customers) {
        if (customer.rental.isRenting) {
            revenue += customer.rental.calculateCurrentCharge();
        }
    }

    return revenue;
}

double Analytics::calculateNetRevenue(const std::list<Customer>& customers) {
    double netRevenue = 0.0;

    for (const auto& customer : customers) {
        netRevenue += customer.rental.getTotalCharge();
    }

    return netRevenue;
}

void Analytics::autoSaveCustomerReport(const std::string& filename, const std::list<Customer>& customers, int vehicleCount) {
    std::ofstream outFile(filename);
    if (!outFile) return;

    auto formatDouble = [](double value) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << value;
        return out.str();
    };

    int activeCustomers = 0;
    for (const auto& customer : customers) {
        if (customer.rental.isRenting) {
            ++activeCustomers;
        }
    }

    outFile << "---Dashboard---\n\n";
    outFile << "Number of cars: " << vehicleCount << "\n";
    outFile << "Number of customers: " << customers.size() << "\n";
    outFile << "Number of active customers: " << activeCustomers << "\n";
    outFile << "Revenue: " << formatDouble(calculateRevenue(customers)) << "\n";
    outFile << "Net Revenue: " << formatDouble(calculateNetRevenue(customers)) << "\n\n";

    outFile << "---Customer list---\n";
    outFile << "ID|Name|Plate|Status|Remaining(hrs)|Rate|Charge\n";

    for (const auto& customer : customers) {
        bool hasActiveVehicle = customer.rental.isRenting && customer.rental.vehicle != nullptr;

        outFile << customer.id << "|"
                << customer.getFullName() << "|"
                << (hasActiveVehicle ? customer.rental.vehicle->getPlate() : "N/A") << "|"
                << (hasActiveVehicle ? "Active" : "Inactive") << "|"
                << formatDouble(customer.rental.getHoursRemaining()) << "|"
                << formatDouble(hasActiveVehicle ? customer.rental.vehicle->getRate() : 0.0) << "|"
                << formatDouble(customer.rental.getTotalCharge()) << "\n";
    }
}
