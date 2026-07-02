#include "services/Inventory.hpp"
#include "utils/Parser.hpp"

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
    std::cout << "\n" << std::left 
              << std::setw(5) << "No." 
              << std::setw(15) << "Model" 
              << std::setw(15) << "Plate" 
              << std::setw(12) << "Status" 
              << "Rate" << "\n";
    
    std::cout << std::string(60, '-') << "\n"; // Decorative line

    // 2. Print the items
    int carNumber = 1;
    for (const auto* v : fleet) {
        std::cout << std::left 
                  << std::setw(5) << carNumber++
                  << std::setw(15) << v->getModel()
                  << std::setw(15) << v->getPlate()
                  << std::setw(12) << (v->getRentedStatus() ? "Rented" : "Available")
                  << "$" << v->getRate() << "\n";
    }
}

void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto* v : fleet) {
        // Format: Plate|Model|Rate|RentedStatus
        outFile << v->getPlate() << "|" 
                << "UnknownModel" << "|" 
                << "500.0" << "|" 
                << v->getRentedStatus() << "\n";
    }
    outFile.close();
}

void Inventory::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    
    while (std::getline(inFile, line)) {
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() >= 3) {
            // Instantiate 'Car' instead of 'Sedan'
            addVehicle(new Car(data[1], data[0], std::stod(data[2])));
        }
    }
    inFile.close();
}