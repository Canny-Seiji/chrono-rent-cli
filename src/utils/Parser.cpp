#include "models/Customer.hpp"
#include "utils/Parser.hpp"
#include "utils/Colors.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// High-level clean split using string streams
vector<string> Parser::split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

void Parser::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int Parser::getValidInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                return value;
            }
            cout << "\033[1;31mError: Out of range (" << min << "-" << max << ").\033[0m\n";
        } else {
            cout << "\033[1;31mError: Invalid number format.\033[0m\n";
            clearInputBuffer();
        }
    }
}

string Parser::toUpper(string str) {
    for (char& c : str)
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return str;
}

string Parser::toLower(string str) {
    for (char& c : str)
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return str;
}

string Parser::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end   = str.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

bool Parser::isAlpha(const string& str) {
    for (char c : str)
        if (!isalpha(static_cast<unsigned char>(c)) && c != '-' && c != '\'' && c != ' ')
            return false;
    return true;
}

bool Parser::isValidPlateOrCategory(const string& str) {
    if (str.empty()) return false;
    for (char c : str)
        if (!isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '-' && c != '.')
            return false;
    return true;
}

bool Parser::idExists(const vector<Customer>& customers, const string& id) {
    string idUpper = toUpper(id);
    for (const auto& c : customers)
        if (toUpper(c.id) == idUpper) return true;
    return false;
}

void Parser::sortCustomers(vector<Customer>& customers) {
    sort(customers.begin(), customers.end(), [](const Customer& a, const Customer& b){
        return a.getFullName() < b.getFullName();
    });
}

string Parser::generateCustomerId(const string& firstName, const string& middleName, const string& lastName, const vector<Customer>& customers) {
    string baseId;
    if (!firstName.empty())
        baseId += static_cast<char>(toupper(static_cast<unsigned char>(firstName[0])));
    if (!middleName.empty())
        baseId += static_cast<char>(toupper(static_cast<unsigned char>(middleName[0])));
    if (!lastName.empty()) {
        baseId += static_cast<char>(toupper(static_cast<unsigned char>(lastName[0])));
        for (size_t i = 1; i < lastName.length(); ++i)
            baseId += static_cast<char>(tolower(static_cast<unsigned char>(lastName[i])));
    }
    
    string finalId = baseId;
    int counter = 2;
    while (idExists(customers, finalId)) {
        finalId = baseId + to_string(counter);
        counter++;
    }
    return finalId;
}

bool Parser::isValidPlate(const std::string& plate) {
    // 1. Length check: Must be exactly 7 characters
    if (plate.length() != 7) return false;
    // 2. Check first 3 characters: Must be alphabetic (Letters)
    for (int i = 0; i < 3; ++i) {
        if (!isalpha(static_cast<unsigned char>(plate[i]))) return false;
    }
    // 3. Check last 4 characters: Must be digits (Numbers)
    for (int i = 3; i < 7; ++i) {
        if (!isdigit(static_cast<unsigned char>(plate[i]))) return false;
    }

    return true;
}

std::string Parser::getValidPlate(const std::string& prompt) {
    std::string plate;
    while (true) {
        std::cout << prompt;
        std::cin >> plate;
        
        plate = toUpper(plate);

        if (isValidPlate(plate)) {
            return plate;
        }

        clearInputBuffer();
        std::cout << "\033[1;31mError: Invalid Format!\n";
        std::cout << "Required Format: 3 letters followed by 4 numbers (e.g., ABC1234).\n";
        std::cout << "You entered: " << plate << "\033[0m\n";
    }
}

std::string Parser::getValidName(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        
        if (isAlpha(input)) { // Reusing your existing isAlpha
            return input;
        }
        
        clearInputBuffer();
        std::cout << "\033[1;31mError: Invalid name format. Use letters only.\033[0m\n";
    }
}

std::string Parser::getValidLicense(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        
        if (isValidPlateOrCategory(input)) { // Reusing your existing logic
            return input;
        }
        
        clearInputBuffer();
        std::cout << "\033[1;31mError: Invalid license format (Alphanumeric only).\033[0m\n";
    }
}

std::string Parser::getValidContact(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        
        bool isNum = true;
        for (char c : input) if (!isdigit(c)) isNum = false;
        
        if (isNum && !input.empty()) return input;
        
        clearInputBuffer();
        std::cout << "\033[1;31mError: Invalid contact. Enter digits only.\033[0m\n";
    }
}
std::string Parser::getExistingPlate(Inventory& fleet) {
    std::string plate;
    
    while (true) {
        std::cout << "Enter Vehicle Plate Number: ";
        std::cin >> plate;
        
        plate = toUpper(plate); 

        if (!isValidPlate(plate)) {
            std::cout << Colors::RED << "Error: Invalid format. Please use the format \"ABC1234\".\n" << Colors::RESET;
        }
        else if (fleet.findVehicle(plate) == nullptr) {
            std::cout << Colors::RED << "Error: Plate " << Colors::RESET <<  Colors::YELLOW << plate << Colors::RESET << Colors::RED << " not found in the fleet.\n" << Colors::RESET;
        }
        else {
            return plate; 
        }

        std::cout << "Would you like to try entering the plate again? (y/n): ";
        char choice;
        std::cin >> choice;

        if (std::tolower(choice) != 'y') {
            return ""; 
        }
    }
}