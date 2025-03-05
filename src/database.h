// PREVENTS MULTIPLE INCLUDES OF THE database.h file during compilation
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>
//Allowing the access to the database in the main.cpp file
extern sqlite3* db;
//Initializing the functions of the database.cpp so they can be used in the main.cpp file
int initDatabase();
void createTable();
void createAccount(const std::string& name, int pin);
void displayAccountInfo(int accNumber, int pin);
void deposit(int accNumber, int pin, double amount);
void withdraw(int accNumber, int pin, double amount);
void deleteAccount(int accNumber, int pin);

#endif // DATABASE_H