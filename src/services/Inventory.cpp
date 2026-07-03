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

    //Dynamic column widths
    size_t wNo = 5 + 5;
    size_t wModel = 5 + 5;  
    size_t wPlate = 5 + 5;  
    size_t wStatus = 6 + 5; 
    size_t wRate = 4 + 5;   

    for (const auto* v : fleet) {
        wModel = std::max(wModel, v->getModel().length() + 5);
        wPlate = std::max(wPlate, v->getPlate().length() + 5);
        
        std::string statusStr = (v->getRentedStatus() ? "Rented" : "Available");
        wStatus = std::max(wStatus, statusStr.length() + 5);
        
        std::string rateStr = "$" + std::to_string((int)v->getRate()); 
        wRate = std::max(wRate, rateStr.length() + 5);
    }

    std::cout << "\n--- Current Fleet ---\n\n";
    
    std::cout << std::left 
              << std::setw(wNo) << "No."
              << std::setw(wModel) << "Model"
              << std::setw(wPlate) << "Plate"
              << std::setw(wStatus) << "Status"
              << std::setw(wRate) << "Rate" << "\n";

    std::cout << std::string(wNo + wModel + wPlate + wStatus + wRate, '-') << "\n";

    int carNumber = 1;
    for (const auto* v : fleet) {
        std::cout << std::left 
                  << std::setw(wNo) << carNumber++
                  << std::setw(wModel) << v->getModel()
                  << std::setw(wPlate) << v->getPlate()
                  << std::setw(wStatus) << (v->getRentedStatus() ? "Rented" : "Available")
                  << std::setw(wRate) << ("$" + std::to_string((int)v->getRate())) << "\n";
    }
}

void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) return; 

    for (const auto* v : fleet) {
        outFile << v->getModel()<< "|" 
                << v->getPlate() << "|"  
                << v->getRate() << "|"   
                << v->getRentedStatus() << "\n";
    }
    outFile.close();
}

void Inventory::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    
    while (std::getline(inFile, line)) {
        std::vector<std::string> data = Parser::split(line, '|');
        if (data.size() >= 4) { 
            Car* newCar = new Car(data[1], data[0], std::stod(data[2]));
            newCar->setRentedStatus(std::stoi(data[3]) != 0);
            addVehicle(newCar);
        }
    }
    inFile.close();
}