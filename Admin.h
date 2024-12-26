#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <iostream>
#include <fstream>
#include "Account.h"

const int ADMIN_PASSWORD = 12345;

class Admin {
private:
    static bool isAuthenticated;

public:
    static bool authenticate(int password) {
        isAuthenticated = (password == ADMIN_PASSWORD);
        return isAuthenticated;
    }

    static void viewAllAccounts() {
        if (!isAuthenticated) {
            std::cout << "\nAdmin authentication required!";
            return;
        }

        std::ifstream file("AccountsDetails.dat", std::ios::binary);
        if (!file) {
            std::cout << "\nNo accounts found!";
            return;
        }

        Account acc;
        std::cout << "\n====== All Account Details ======\n";
        while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            acc.displayDetails();
        }
        file.close();
    }

    static void deleteAccount(const std::string& accNum) {
        if (!isAuthenticated) {
            std::cout << "\nAdmin authentication required!";
            return;
        }

        std::ifstream inFile("AccountsDetails.dat", std::ios::binary);
        std::ofstream outFile("Temp.dat", std::ios::binary);

        if (!inFile) {
            std::cout << "\nNo accounts found!";
            return;
        }

        bool found = false;
        Account acc;

        while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() == accNum) {
                found = true;
                std::cout << "\nAccount with number " << accNum << " deleted successfully!";
            }
            else {
                outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            }
        }

        inFile.close();
        outFile.close();

        if (remove("AccountsDetails.dat") != 0) {
            perror("Error deleting the file");
        }
        if (rename("Temp.dat", "AccountsDetails.dat") != 0) {
            perror("Error renaming the file");
        }

        if (!found) {
            std::cout << "\nAccount with number " << accNum << " not found!";
        }
    }

    static void modifyAccount(const std::string& accNum) {
        if (!isAuthenticated) {
            std::cout << "\nAdmin authentication required!";
            return;
        }

        Account* acc = Account::findAccount(accNum);
        if (acc) {
            std::cout << "\nAccount modification feature coming soon...";
            delete acc;
        }
        else {
            std::cout << "\nAccount not found!";
        }
    }

    static void searchAccountDetails(const std::string& accNum) {
        if (!isAuthenticated) {
            std::cout << "\nAdmin authentication required!";
            return;
        }

        std::ifstream file("AccountsDetails.dat", std::ios::binary);
        if (!file) {
            std::cout << "\nNo accounts found!";
            return;
        }

        Account acc;
        bool found = false;

        while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccountNumber() == accNum) {
                std::cout << "\nAccount found:\n";
                acc.displayDetails();
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "\nAccount with number " << accNum << " not found!";
        }

        file.close();
    }
};

bool Admin::isAuthenticated = false;

#endif

