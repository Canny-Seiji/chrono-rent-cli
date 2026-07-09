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

    // Load existing data from files
    myFleet.loadFromFile("data/fleet.txt");
    myManager.loadRentals("data/rentals.txt", myFleet);
    if (!custRegistry.loadCustomers("data/customers.txt", myFleet)) {
        for (const auto& record : myManager.getActiveRentals()) {
            custRegistry.addCustomer(record.customer);
        }
    }

    int choice = 0;
    
    do {
        std::cout << Colors::YELLOW << "\n=================================================\n" << Colors::RESET;
        std::cout << Colors::YELLOW << "        ChronoRent Vehicle Rental System " << Colors::RESET;
        std::cout << Colors::YELLOW << "\n=================================================\n" << Colors::RESET;
        std::cout << "1. Rent Vehicle\n2. View Fleet\n3. Return Vehicle\n4. View Customers\n5. Add Vehicle\n6. Remove Vehicle\n0. Exit\n";
        
        choice = Parser::getValidInt("Select operation (0-6): ", 0, 6);

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

                std::string fName = Parser::getValidName("First Name (letters only, e.g., Juan): ");          if(fName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string mName = Parser::getValidName("Middle Name (letters only, e.g., Santos): ");       if(mName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string lName = Parser::getValidName("Last Name (letters only, e.g., Dela Cruz): ");      if(lName.empty())   { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string license = Parser::getValidLicense("License Number (11 chars: 1 letter + 10 digits, e.g., A1234567890): "); if(license.empty()) { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                std::string contact;
                do {
                    contact = Parser::getValidContact("Contact Number (unique, 11 digits, e.g., 09123456789): ");
                    if(contact.empty()) { std::cout << Colors::YELLOW << "Rental process cancelled.\n" << Colors::RESET; break; }
                    if(custRegistry.contactExists(contact)) {
                        std::cout << Colors::RED << "Contact number already exists. Enter a unique contact number.\n" << Colors::RESET;
                    }
                } while (!contact.empty() && custRegistry.contactExists(contact));
                if(contact.empty()) { break; }
                int rentalDays = Parser::getValidInt("Rental duration in days (1-365): ", 1, 365);

                std::string id = custRegistry.generateUniqueId(fName, mName, lName);
                Customer newCust(id, fName, mName, lName, license, contact);
                
                newCust.rental.isRenting = true;
                newCust.rental.vehicle = vehiclePtr;
                newCust.rental.rentStartTime = std::time(nullptr);
                newCust.rental.expectedReturnTime = newCust.rental.rentStartTime + (rentalDays * 86400);

                custRegistry.addCustomer(newCust);
                vehiclePtr->setRentedStatus(true);

                myManager.addRentalRecord(RentalRecord("TX-001", newCust, vehiclePtr));
                std::cout << Colors::GREEN << "Rental successful!\n" << Colors::RESET << "Customer ID: " << id << "\n";
                std::cout << "Estimated charge: " << Colors::YELLOW << newCust.rental.calculateCurrentCharge() << "\n" << Colors::RESET;
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
                custRegistry.displayReport();
                break;
            }
            
            case 5: { // Add Vehicle
                std::string model = Parser::getValidName("Enter Model (letters/numbers allowed, e.g., Toyota Vios): ");
                std::string plate = Parser::getValidPlate("Enter New Vehicle Plate (7 chars: 3 letters + 4 digits, e.g., ABC1234): ");
                double rate = Parser::getValidDouble("Enter Daily Rental Rate (minimum 800.00): ", 800.00);
                
                myFleet.addVehicle(new Car(model, plate, rate));
                std::cout << Colors::GREEN << "Vehicle added successfully.\n" << Colors::RESET;
                break;
            }

            case 6: { // Remove Vehicle
                std::string plate = Parser::getExistingPlate(myFleet);

                if (plate.empty()) {
                    std::cout << Colors::YELLOW << "Remove vehicle process cancelled.\n" << Colors::RESET;
                    break;
                }

                Vehicle* vehiclePtr = myFleet.findVehicle(plate);
                if (vehiclePtr && vehiclePtr->getRentedStatus()) {
                    std::cout << Colors::RED << "Cannot remove a rented vehicle. Return it first.\n" << Colors::RESET;
                    break;
                }

                if (myFleet.removeVehicle(plate)) {
                    std::cout << Colors::GREEN << "Vehicle " << plate << " removed from fleet.\n" << Colors::RESET;
                } else {
                    std::cout << Colors::RED << "Error: Vehicle " << plate << " could not be removed.\n" << Colors::RESET;
                }
                break;
            }

            case 0:
                std::cout << "Exiting system...\n";
                break;

            default:
                std::cout << "Invalid selection. Please try again.\n";
        }
    } while (choice != 0);

    // Save data before exiting
    Analytics::sortByUsage(myManager.getActiveRentals());
    myFleet.saveToFile("data/fleet.txt");
    myManager.saveRentals("data/rentals.txt");
    custRegistry.saveCustomers("data/customers.txt");
    Analytics::autoSaveCustomerReport("data/customer_report.txt", custRegistry.getCustomers(), myFleet.getVehicleCount());
    
    std::cout << Colors::GREEN << "Data has been saved.\n" << Colors::RESET;
    std::cout << Colors::GREEN << "Customer report has been auto-saved.\n" << Colors::RESET;
    return 0;
}
