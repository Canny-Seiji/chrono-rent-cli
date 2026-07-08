#include "../../include/services/Inventory.hpp"
#include "../../include/utils/Parser.hpp"

// Destructor for the Inventory class, deletes all vehicles in the fleet to free memory
Inventory::~Inventory() {
    for (auto v : fleet) delete v; 
}

// Adds a new vehicle to the fleet
void Inventory::addVehicle(Vehicle* v) { fleet.push_back(v); }

// Finds a vehicle in the fleet by its plate number and returns a pointer to it, or nullptr if not found
Vehicle* Inventory::findVehicle(const std::string& plate) {
    for (auto v : fleet) {
        if (v->getPlate() == plate) return v;
    }
    return nullptr;
}

// Displays the current fleet of vehicles in a formatted table
void Inventory::displayFleet() const {
    if (fleet.empty()) {
        std::cout << "Fleet is currently empty.\n";
        return;
    }

    // dynamic column widths
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

        std::string rateStr = "₱" + std::to_string((int)v->getRate());
        wRate = std::max(wRate, rateStr.length() + 5);
    }

    // Print Headers
    std::cout << "\n--- Current Fleet ---\n\n";
    
    std::cout << Colors::YELLOW; 
    std::cout << std::left
              << std::setw(wNo) << "No."
              << std::setw(wModel) << "Model"
              << std::setw(wPlate) << "Plate"
              << std::setw(wStatus) << "Status"
              << std::setw(wRate) << "Rate";
    std::cout << Colors::RESET << "\n"; 

    std::cout << std::string(wNo + wModel + wPlate + wStatus + wRate, '-') << "\n";

    // Print Fleet Data
    int carNumber = 1;
    for (const auto* v : fleet) {
        // Prepare data
        std::string statusStr = (v->getRentedStatus() ? "Rented" : "Available");
        std::string statusColor = (v->getRentedStatus() ? Colors::RED : Colors::GREEN);
        std::string rateStr = "₱" + std::to_string((int)v->getRate());

        // Print row
        std::cout << std::left
                  << std::setw(wNo) << carNumber++
                  << std::setw(wModel) << v->getModel()
                  << std::setw(wPlate) << v->getPlate();
        std::cout << statusColor << std::setw(wStatus) << statusStr << Colors::RESET;
        std::cout << std::setw(wRate) << rateStr << "\n";
    }
}

// Returns the total number of vehicles in the fleet
int Inventory::getVehicleCount() const {
    return static_cast<int>(fleet.size());
}

// Saves the fleet data to a file
void Inventory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) return; 

    for (const auto* v : fleet) {
        outFile << v->getPlate()<< "|" 
                << v->getModel() << "|"  
                << v->getRate() << "|"   
                << v->getRentedStatus() << "\n";
    }
    outFile.close();
}

// Loads the fleet data from a file
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
