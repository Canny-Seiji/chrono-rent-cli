#ifndef PARSER_H
#define PARSER_H

#include "services/Inventory.hpp"
#include "models/Customer.hpp"
#include "utils/Colors.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
class Customer;

class Parser {
public:
    // File Parsing
    static std::vector<std::string> split(const std::string& str, char delimiter);    

    // Conversions
    static std::string toUpper(std::string str);
    static std::string toLower(std::string str);
    static std::string trim(const std::string& str);

    // Validations
    static void clearInputBuffer();
    static bool isAlpha(const std::string& str);
    static bool isAlphaNum(const std::string& str);
    static bool isValidPlateOrCategory(const std::string& str);
    static bool idExists(const std::vector<Customer>& customers, const std::string& id);
    static bool isValidPlate(const std::string& plate);
    static bool isValidLicense(const std::string& license);
    static bool isValidContact(const std::string& contact);
    static int  getValidInt(const std::string& prompt, int min, int max);
    static double getValidDouble(const std::string& prompt, double min);
    static std::string getValidPlate(const std::string& prompt);
    static std::string getValidName(const std::string& prompt);
    static std::string getValidLicense(const std::string& prompt);
    static std::string getValidContact(const std::string& prompt);
    static std::string getExistingPlate(Inventory& fleet);

    // Sorting & Generators
    static void sortCustomers(std::vector<Customer>& customers);
    static std::string findUniqueId(const std::string& baseId, int counter, const std::vector<Customer>& customers);
    static std::string generateCustomerId(const std::string& firstName, const std::string& middleName, const std::string& lastName, const std::vector<Customer>& customers);
};

#endif
