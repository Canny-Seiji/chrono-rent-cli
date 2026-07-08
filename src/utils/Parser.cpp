#include "../../include/utils/Parser.hpp"
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

// Clear the input buffer to avoid issues with leftover input
void Parser::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Converts a string to uppercase
string Parser::toUpper(string str) {
    for (char& c : str)
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return str;
}

// Converts a string to lowercase
string Parser::toLower(string str) {
    for (char& c : str)
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return str;
}

// Trims leading and trailing whitespace from a string
string Parser::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end   = str.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    return str.substr(start, end - start + 1);
}

// Validates if a string contains only alphabetic characters, hyphens, apostrophes, or spaces
bool Parser::isAlpha(const string& str) {
    for (char c : str)
        if (!isalpha(static_cast<unsigned char>(c)) && c != '-' && c != '\'' && c != ' ')
            return false;
    return true;
}

// Validates if a string contains only alphanumeric characters, hyphens, apostrophes, or spaces
bool Parser::isAlphaNum(const string& str) {
    for (char c : str)
        if (!isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '\'' && c != ' ')
            return false;
    return true;
}

// Checks if a given customer ID already exists in the list of customers
bool Parser::idExists(const list<Customer>& customers, const string& id) {
    string idUpper = toUpper(id);
    for (const auto& c : customers)
        if (toUpper(c.id) == idUpper) return true;
    return false;
}

// Sorts the list of customers by their full names in ascending order
void Parser::sortCustomers(list<Customer>& customers) {
    customers.sort([](const Customer& a, const Customer& b){
        return a.getFullName() < b.getFullName();
    });
}

// Recursively finds a unique customer ID based on a base ID and a counter
std::string Parser::findUniqueId(const std::string& baseId, int counter, const std::list<Customer>& customers) {
    std::string candidateId = (counter == 1) ? baseId : baseId + std::to_string(counter);
    if (!idExists(customers, candidateId)) {
        return candidateId;
    }
    return findUniqueId(baseId, counter + 1, customers);
}

// Generates a customer ID based on the first, middle, and last names, ensuring uniqueness
string Parser::generateCustomerId(const string& firstName, const string& middleName, const string& lastName, const list<Customer>& customers) {
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
    
    return findUniqueId(baseId, 1, customers);
}

// Validates if a vehicle plate number is in the correct format (3 letters followed by 4 digits)
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

// Validates if a driver's license number is in the correct format (1 letter followed by 10 digits)
bool Parser::isValidLicense(const std::string& license) {
    if (license.length() != 11) return false;
    
    if (!isalpha(static_cast<unsigned char>(license[0]))) return false;

    for (int i = 1; i < 11; ++i) {
        if (!isdigit(static_cast<unsigned char>(license[i]))) return false;
    }
    return true;
}

// Validates if a contact number is in the correct format (exactly 11 digits)
bool Parser::isValidContact(const std::string& contact) {
    if (contact.length() != 11) return false;
    for (char c : contact) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

// Prompts the user for a valid integer input within a specified range, with error handling
int Parser::getValidInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                clearInputBuffer(); 
                return value;
            }
            cout << Colors::RED << "Out of range (" << min << "-" << max << ")." << Colors::RESET << "\n";
            clearInputBuffer(); 
        } else {
            cout << Colors::RED << "Error: Invalid number format." << Colors::RESET << "\n";
            clearInputBuffer(); 
        }
    }
}

// Prompts the user for a valid double input greater than or equal to a specified minimum, with error handling
double Parser::getValidDouble(const std::string& prompt, double min) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        std::string normalized;
        bool hasDigit = false;
        bool hasDecimal = false;
        bool invalidCharacter = false;

        for (char raw : input) {
            unsigned char c = static_cast<unsigned char>(raw);

            if (std::isdigit(c)) {
                normalized += static_cast<char>(c);
                hasDigit = true;
            } else if (c == '.' && !hasDecimal) {
                normalized += '.';
                hasDecimal = true;
            } else if ((c == '+' || c == '-') && normalized.empty()) {
                normalized += static_cast<char>(c);
            } else if (c == ',' || c == '$' || c == '?' || std::isspace(c) || c >= 128) {
                continue;
            } else if (c == 'P' || c == 'p' || c == 'H' || c == 'h') {
                continue;
            } else {
                invalidCharacter = true;
                break;
            }
        }

        try {
            if (!invalidCharacter && hasDigit) {
                double value = std::stod(normalized);
                if (value >= min) {
                    return value;
                }
                std::cout << Colors::RED << "Rate must be at least " << min << Colors::RESET << "\n";
            } else {
                std::cout << Colors::RED << "Error: Invalid number format. Examples: 10000, 10,000, PHP 10,000, or 10 000." << Colors::RESET << "\n";
            }
        } catch (...) {
            std::cout << Colors::RED << "Error: Invalid number format. Examples: 10000, 10,000, PHP 10,000, or 10 000." << Colors::RESET << "\n";
        }
    }
}

// Prompts the user for a valid vehicle plate number, ensuring it meets the required format
std::string Parser::getValidPlate(const std::string& prompt) {
    std::string plate;
    while (true) {
        std::cout << prompt;
        std::cin >> plate;
        clearInputBuffer();

        trim(plate);
        plate = toUpper(plate);

        if (isValidPlate(plate)) {
            return plate;
        }

        clearInputBuffer();
        std::cout << Colors::RED << "Error: Invalid Format!" << Colors::RESET << "\n";
        std::cout << "Required Format: 3 letters followed by 4 numbers (e.g., ABC1234).\n";
        std::cout << "You entered: " << plate << Colors::RESET << "\n";
    }
}

// Prompts the user for a valid name input, ensuring it contains only letters, hyphens, apostrophes, or spaces
std::string Parser::getValidName(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        getline(std::cin, input);
        
        trim(input); 
        if (isAlphaNum(input)) { 
            return input;
        }
        char choice;
        cout << "Do you like to cancel transaction? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        
        if (std::tolower(choice) != 'y') {
            return ""; 
        }
        
        clearInputBuffer();
        std::cout << Colors::RED << "Error: Invalid name format. Use letters only.\n" << Colors::RESET;
    }
}

// Prompts the user for a valid driver's license number, ensuring it meets the required format
std::string Parser::getValidLicense(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        clearInputBuffer();

        trim(input);
        // 3-2-6 format
        if (isValidLicense(input)) {
            return input;
        } else {
            clearInputBuffer();
            std::cout << Colors::RED << "Invalid license format. Please use the format \"A123456789\" (1 letter followed by 10 digits)." << Colors::RESET << "\n";
        }      
        
        char choice;
        cout << "Would you like to try entering the license again? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (std::tolower(choice) != 'y') {
            return ""; 
        }
    }
}

// Prompts the user for a valid contact number, ensuring it meets the required format
std::string Parser::getValidContact(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        clearInputBuffer();

        trim(input);
        // Check if the input is exactly 11 digits
        if (isValidContact(input)) {
            return input;
        } else {
            clearInputBuffer();
            std::cout << Colors::RED << "Invalid contact. Enter exactly 11 digits." << Colors::RESET << "\n";
        }

        char choice;
        cout << "Would you like to try entering the contact again? (y/n): ";
        std::cin >> choice;
        clearInputBuffer();
        if (std::tolower(choice) != 'y') {
            return ""; 
        }
    }
}

// Prompts the user for an existing vehicle plate number from the inventory, ensuring it meets the required format and exists in the fleet
std::string Parser::getExistingPlate(Inventory& fleet) {
    std::string plate;
    
    while (true) {
        std::cout << "Enter Vehicle Plate Number (7 chars: 3 letters + 4 digits, e.g., ABC1234): ";
        std::cin >> plate;
        clearInputBuffer();
        
        trim(plate);
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
        clearInputBuffer();

        if (std::tolower(choice) != 'y') {
            return ""; 
        }
    }
}
