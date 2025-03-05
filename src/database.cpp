#include "database.h"
#include <iostream>
#include <sqlite3.h>

sqlite3* db;
//Starting the database
int initDatabase() {
    int exit = sqlite3_open("bank.db", &db);
    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return exit;
    }
    return 0;
}

//Creating the Accounts table for the users data
void createTable() {
    std::string sql = "CREATE TABLE IF NOT EXISTS Accounts ("
                      "AccountNumber INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "AccountName TEXT NOT NULL, "
                      "Pin INTEGER NOT NULL, "
                      "Balance REAL NOT NULL DEFAULT 0);";
    
    // Output for debug if there is a problem
    char* errMsg;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if (exit != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}


void createAccount(const std::string& name, int pin) {
    //Making sure the User cant make a blank name!
    if (name.empty()) {
        std::cerr << "Error: Account name cannot be empty." << std::endl;
        return;
    }

    //Inputting the data into the database
    std::string sql = "INSERT INTO Accounts (AccountName, Pin) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, pin);

    //If the values match the requirements the account is created
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Account created successfully! Remember your Account Number!\n"  <<"Your Account Number is: " << sqlite3_last_insert_rowid(db) << std::endl;
    } else {
        std::cerr << "Error creating account." << std::endl;
    }

    sqlite3_finalize(stmt);
}

//Getting the Account name and balance from the database to show them to the user
void displayAccountInfo(int accNumber, int pin) {
    std::string sql = "SELECT AccountName, Balance FROM Accounts WHERE AccountNumber = ? AND Pin = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, accNumber);
    sqlite3_bind_int(stmt, 2, pin);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "===============" << std::endl;
        std::cout << "Account Name: " << sqlite3_column_text(stmt, 0) << std::endl;
        std::cout << "Balance: " << sqlite3_column_double(stmt, 1) << " $" << std::endl;
        std::cout << "===============" << std::endl;
    } else {
        std::cerr << "Account not found or incorrect PIN!" << std::endl;
    }

    sqlite3_finalize(stmt);
}

void deposit(int accNumber, int pin, double amount) {
    //Making sure the User cant deposit a negative value
    if (amount <= 0) {
        std::cerr << "Error: Deposit amount more than 0." << std::endl;
        return;
    }
    //Get the Balance from the User account based in AccName and Pin
    std::string sql = "SELECT Balance FROM Accounts WHERE AccountNumber = ? AND Pin = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, accNumber);
    sqlite3_bind_int(stmt, 2, pin);

    //Update the Balance with the deposited amount
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);

        sql = "UPDATE Accounts SET Balance = Balance + ? WHERE AccountNumber = ? AND Pin = ?;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        sqlite3_bind_double(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, accNumber);
        sqlite3_bind_int(stmt, 3, pin);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << amount << " $ deposited successfully!" << std::endl;
        } else {
            std::cerr << "Deposit failed!" << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Account not found or incorrect PIN!" << std::endl;
        sqlite3_finalize(stmt);
    }
}

// Withdraw money while making sure the User cannot withdraw more that what the balance is!
void withdraw(int accNumber, int pin, double amount) {
    std::string sql = "SELECT Balance FROM Accounts WHERE AccountNumber = ? AND Pin = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, accNumber);
    sqlite3_bind_int(stmt, 2, pin);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double balance = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
        //Cannot withdraw more than the balance in the account
        if (amount <= balance) {
            sql = "UPDATE Accounts SET Balance = Balance - ? WHERE AccountNumber = ? AND Pin = ?;";
            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
            sqlite3_bind_double(stmt, 1, amount);
            sqlite3_bind_int(stmt, 2, accNumber);
            sqlite3_bind_int(stmt, 3, pin);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                std::cout << amount << " $ withdrawn successfully!" << std::endl;
            } else {
                std::cerr << "Withdrawal failed!" << std::endl;
            }

            sqlite3_finalize(stmt);
        } else {
            std::cout << "Insufficient balance!" << std::endl;
        }
    } else {
        std::cerr << "Account not found or incorrect PIN!" << std::endl;
        sqlite3_finalize(stmt);
    }
}

void deleteAccount(int accNumber, int pin) {
    std::string sql = "DELETE FROM Accounts WHERE AccountNumber = ? AND Pin = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, accNumber);
    sqlite3_bind_int(stmt, 2, pin);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Account deleted successfully!" << std::endl;
    } else {
        std::cerr << "Error deleting account!" << std::endl;
    }

    sqlite3_finalize(stmt);
}