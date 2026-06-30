#include <iostream>
#include "services/Inventory.hpp"
#include "services/RentalManager.hpp"
#include "services/Analytics.hpp"
#include "services/CustomerManager.hpp"
#include "utils/Parser.hpp"

int main() {
    Inventory myFleet;
    RentalManager myManager;
    CustomerManager custRegistry;

    myFleet.loadFromFile("data/fleet.txt");
    myManager.loadRentals("data/rentals.txt", myFleet);

    int choice = 0;
    
    do {
        std::cout << "\n--- ChronoRent System ---\n";
        std::cout << "1. Rent Vehicle\n2. View Fleet\n3. Return Vehicle\n4. View Reports\n5. Add Vehicle\n0. Exit\n";
        
        choice = Parser::getValidInt("Select operation: ", 0, 5);

        switch (choice) {
            case 1: { // Rent Vehicle
                std::string plate = Parser::getExistingPlate(myFleet);
                if (plate.empty()) {
                    std::cout << "Rental process cancelled.\n";
                    break;
                }

                Vehicle* vehiclePtr = myFleet.findVehicle(plate);
                if (vehiclePtr->getRentedStatus()) {
                    std::cout << "Vehicle already rented!\n";
                    break;
                }

                std::string fName = Parser::getValidName("First Name: ");
                std::string mName = Parser::getValidName("Middle Initial: ");
                std::string lName = Parser::getValidName("Last Name: ");
                std::string license = Parser::getValidLicense("License Number: ");
                std::string contact = Parser::getValidContact("Contact Number: ");

                std::string id = custRegistry.generateUniqueId(fName, mName, lName);
                Customer newCust(id, fName, mName, lName, license, contact);
                
                newCust.rental.isRenting = true;
                newCust.rental.vehicle = vehiclePtr;
                newCust.rental.rentStartTime = std::time(nullptr);

                custRegistry.addCustomer(newCust);
                vehiclePtr->setRentedStatus(true);

                std::cout << "Rental successful! Customer ID: " << id << "\n";
                break;
            }
            case 2: // View Fleet
                myFleet.displayFleet();
                break;
            
            case 3: { // Return Vehicle
                std::string plate = Parser::getExistingPlate(myFleet);
                
                if (plate.empty()) {
                    std::cout << "Return process cancelled.\n";
                    break;
                }

                myManager.returnVehicle(plate);
                break;
            }
            
            case 4: { // Reports
                std::cout << "\n--- Analytics Report ---\n";
                std::cout << "Sort by:\n";
                std::cout << "1. Latest Customers\n";
                std::cout << "2. Currently Renting\n";
                std::cout << "3. Rental Rate (High-Low)\n";
                
                int sortChoice = Parser::getValidInt("Choice (1-3): ", 1, 3);
                
                custRegistry.sortCustomers(sortChoice);
                custRegistry.displayReport();
                
                break;
            }
            
            case 5: { // Add Vehicle
                std::string plate = Parser::getValidPlate("Enter New Vehicle Plate (e.g., ABC1234): ");
                
                std::string model;
                double rate;
                std::cout << "Model: "; std::cin >> model;
                std::cout << "Rate: "; std::cin >> rate;
                
                myFleet.addVehicle(new Sedan(plate, model, rate));
                std::cout << "Vehicle added successfully.\n";
                break;
            }
            
            case 0:
                std::cout << "Exiting system...\n";
                break;

            default:
                std::cout << "Invalid selection. Please try again.\n";
        }
    } while (choice != 0);

    // 2. SAVE DATA BEFORE EXIT
    myFleet.saveToFile("data/fleet.txt");
    myManager.saveRentals("data/rentals.txt");
    
    std::cout << "Data saved. Goodbye!\n";
    return 0;
}
