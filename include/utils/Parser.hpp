#ifndef PARSER_H
#define PARSER_H

#include "services/Inventory.hpp"
#include <string>
#include <vector>
#include <cctype>

class Customer;

class Parser {
public:
    // File Parsing
    static std::vector<std::string> split(const std::string& str, char delimiter);

    // Input Protections & Streams
    static void clearInputBuffer();
    static int getValidInt(const std::string& prompt, int min, int max);

    // Conversions
    static std::string toUpper(std::string str);
    static std::string toLower(std::string str);
    static std::string trim(const std::string& str);

    // Validations
    static bool isAlpha(const std::string& str);
    static bool isValidPlateOrCategory(const std::string& str);
    static bool idExists(const std::vector<Customer>& customers, const std::string& id);
    static bool isValidPlate(const std::string& plate);
    static std::string getValidPlate(const std::string& prompt);
    static std::string getValidName(const std::string& prompt);
    static std::string getValidLicense(const std::string& prompt);
    static std::string getValidContact(const std::string& prompt);
    static std::string getExistingPlate(Inventory& fleet);

    // Sorting & Generators
    static void sortCustomers(std::vector<Customer>& customers);
    static std::string generateCustomerId(const std::string& firstName, const std::string& middleName, const std::string& lastName, const std::vector<Customer>& customers);
};

#endif
