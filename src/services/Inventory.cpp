#include "services/Inventory.hpp"
#include "utils/Parser.hpp"
#include <iostream>

Inventory::~Inventory() {
    for (auto v : fleet) delete v; 
}

void Inventory::addVehicle(Vehicle* v) { fleet.push_back(v); }

Vehicle* Inventory::findVehicle(const std::string& plate) {
    for (auto v : fleet) {
        if (v->getPlate() == plate) return v;
    }
    return nullptr;
}

void Inventory::displayFleet() const {
    if (fleet.empty()) {
        std::cout << "Fleet is currently empty.\n";
        return;
    }
    std::cout << "\n--- Current Fleet ---\n";
    for (const auto* v : fleet) {
        std::cout << "Plate: " << v->getPlate() 
                  << " | Rented: " << (v->getRentedStatus() ? "Yes" : "No") << "\n";
    }
}

void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto* v : fleet) {
        // Format: Plate|Model|Rate|RentedStatus
        outFile << v->getPlate() << "|" 
                << "UnknownModel" << "|" // Simplified for now
                << "500.0" << "|" 
                << v->getRentedStatus() << "\n";
    }
    outFile.close();
}

void Inventory::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (std::getline(inFile, line)) {
        // Use Parser to split line by '|'
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() >= 4) {
            // Reconstruct the vehicle (assuming Sedan for simplicity)
            // Note: In a production system, you'd check a type field first
            addVehicle(new Sedan(data[0], data[1], std::stod(data[2])));
        }
    }
    inFile.close();
}