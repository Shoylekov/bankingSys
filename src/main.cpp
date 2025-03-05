#include <iostream>
#include <string>
#include "database.h"

// METHOD FOR VALIDATING PIN CODE
bool isValidPin(const std::string& pin) {
    if (pin.length() != 4) return false;
    for (char c : pin) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main() {
    if (initDatabase() != 0) {
        return -1;
    }
    createTable();

    int choice;
    int accNumber;
    std::string pin;
    std::string name;
    double amount;

    do {
        std::cout << "**********" << std::endl;
        std::cout << "Fake Bank" << std::endl;
        std::cout << "1. Create a New Account" << std::endl;
        std::cout << "2. Display Account Info" << std::endl;
        std::cout << "3. Deposit Funds" << std::endl;
        std::cout << "4. Withdraw Funds" << std::endl;
        std::cout << "5. Delete Account" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << "**********" << std::endl;

        switch (choice) {
            case 1:  // Creating a new account
                std::cout << "Enter your Account Name: " << std::endl;
                std::cin.ignore();
                std::getline(std::cin, name);
                if (name.empty()) {
                    std::cout << "Account name cannot be empty. Please try again." << std::endl;
                    break;
                }
                do {
                    std::cout << "Enter your PIN code (4 digits): " << std::endl;
                    std::cin >> pin;
                    if (!isValidPin(pin)) {
                        std::cout << "Invalid PIN. Please enter exactly 4 digits." << std::endl;
                    }
                } while (!isValidPin(pin));
                createAccount(name, std::stoi(pin));
                break;

            case 2:  // Display account info
                std::cout << "Please enter your Account Number: " << std::endl;
                std::cin >> accNumber;
                std::cout << "Please enter your Pin: " << std::endl;
                std::cin >> pin;
                displayAccountInfo(accNumber, std::stoi(pin));
                break;

            case 3:  // Deposit funds
                std::cout << "Enter Account Number: ";
                std::cin >> accNumber;
                std::cout << "Please enter your Pin: " << std::endl;
                std::cin >> pin;
                std::cout << "Enter a deposit amount: ";
                std::cin >> amount;
                deposit(accNumber, std::stoi(pin), amount);
                break;

            case 4:  // Withdraw funds
                std::cout << "Enter Account Number: ";
                std::cin >> accNumber;
                std::cout << "Please enter your Pin: " << std::endl;
                std::cin >> pin;
                std::cout << "Enter Withdraw amount: ";
                std::cin >> amount;
                withdraw(accNumber, std::stoi(pin), amount);
                break;

            case 5:  // Delete account
                std::cout << "Enter Account Number: ";
                std::cin >> accNumber;
                std::cout << "Please enter your Pin: " << std::endl;
                std::cin >> pin;
                deleteAccount(accNumber, std::stoi(pin));
                break;

            case 6:
                std::cout << "Thank you for using Fake Bank. Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 6);

    sqlite3_close(db);
    return 0;
}