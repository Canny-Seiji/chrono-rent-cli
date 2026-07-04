#include <iostream>
#include "../include/services/Inventory.hpp"
#include "../include/services/RentalManager.hpp"
#include "../include/services/Analytics.hpp"
#include "../include/services/CustomerManager.hpp"
#include "../include/utils/Parser.hpp"

int main() {
    Inventory myFleet;
    RentalManager myManager;
    CustomerManager custRegistry;

    myFleet.loadFromFile("data/fleet.txt");
    myManager.loadRentals("data/rentals.txt", myFleet);

    int choice = 0;
    
    do {
        std::cout << "\n--- ChronoRent System ---\n";
        std::cout << "1. Rent Vehicle\n2. View Fleet\n3. Return Vehicle\n4. View Customers\n5. Add Vehicle\n0. Exit\n";
        
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

                std::string fName = Parser::getValidName("First Name: ");          if(fName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string mName = Parser::getValidName("Middle Name: ");         if(mName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string lName = Parser::getValidName("Last Name: ");           if(lName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string license = Parser::getValidLicense("License Number: "); if(license.empty()) { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string contact = Parser::getValidContact("Contact Number: "); if(contact.empty()) { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                int rentalDays = Parser::getValidInt("Rental duration in days: ", 1, 365);

                std::string id = custRegistry.generateUniqueId(fName, mName, lName);
                Customer newCust(id, fName, mName, lName, license, contact);
                
                newCust.rental.isRenting = true;
                newCust.rental.vehicle = vehiclePtr;
                newCust.rental.rentStartTime = std::time(nullptr);
                newCust.rental.expectedReturnTime = newCust.rental.rentStartTime + (rentalDays * 86400);

                custRegistry.addCustomer(newCust);
                vehiclePtr->setRentedStatus(true);

                myManager.addRentalRecord(RentalRecord("TX-001", newCust, vehiclePtr));
                std::cout << "Rental successful! Customer ID: " << id << "\n";
                std::cout << "Estimated charge: " << newCust.rental.calculateCurrentCharge() << "\n";
                break;
            }
            case 2: // View Fleet
                myFleet.displayFleet();
                break;
            
            case 3: { // Return Vehicle
                std::string plate = Parser::getExistingPlate(myFleet);
                
                if (plate.empty()) {
                    std::cout << Colors::YELLOW << "Return process cancelled.\n" << Colors::RESET;
                    break;
                }

                if (myManager.returnVehicle(plate)) {
                    custRegistry.endRentalByPlate(plate);
                }
                break;
            }
            
            case 4: { // View Customers
                custRegistry.displayDashboard(myFleet.getVehicleCount());
                std::cout << "Sort by:\n";
                std::cout << "1. Name\n";
                std::cout << "2. Status\n";
                std::cout << "3. Rental Expense\n";
                
                int sortChoice = Parser::getValidInt("Choice (1-3): ", 1, 3);
                
                custRegistry.sortCustomers(sortChoice);
                custRegistry.displayReport();
                
                break;
            }
            
            case 5: { // Add Vehicle
                std::string model = Parser::getValidName("Enter Model: ");
                std::string plate = Parser::getValidPlate("Enter New Vehicle Plate (e.g., ABC1234): ");
                double rate = Parser::getValidDouble("Enter Daily Rental Rate: ", 8000.00);
                
                myFleet.addVehicle(new Car(model, plate, rate));
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
