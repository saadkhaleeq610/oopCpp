#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include "Transaction.h"
#include "Date.h" 
#define MIN_INITIAL_BALANCE 1000

using namespace std;

class Account {
protected:
    char accountNumber[13];
    char name[50];
    char password[20];
    char email[50];
    char phone[15];
    double balance;
    bool isActive;
    char type;

public:
    Account() : balance(0), isActive(true) {}

    void generateAccountNumber() {
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < 12; i++) {
            accountNumber[i] = '0' + (rand() % 10);
        }
        accountNumber[12] = '\0';
    }

    bool authenticate(const string& pwd) const {
        return strcmp(password, pwd.c_str()) == 0;
    }

    void createAccount() {
        cout << "\n\nNew Account being created!" << endl;
        cout << "Please note that this will be your account's number\nMake sure to save your account number somewhere! Else you won't be able to access your account" << endl;
        cout << "\nAccount Number:" << endl;
        generateAccountNumber();
        cout << accountNumber << endl;

        while (true) {
            try {
                cout << "\n\nEnter Full Name (Only alphabets, max 22 characters): ";
                cin.ignore();
                cin.getline(name, 50);
                if (!regex_match(name, regex("^[a-zA-Z ]{1,22}$"))) {
                    throw invalid_argument("Invalid name. Please enter alphabets only (max 22 characters).");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }

        while (true) {
            try {
                cout << "Enter Password (Minimum 8 characters): ";
                cin.getline(password, 20);
                if (strlen(password) < 8) {
                    throw invalid_argument("Password must be at least 8 characters long.");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }

        while (true) {
            try {
                cout << "Enter Email: ";
                cin.getline(email, 50);
                if (!regex_match(email, regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"))) {
                    throw invalid_argument("Invalid email format.");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }

        while (true) {
            try {
                cout << "Enter Phone (Only digits, max 11 digits): ";
                cin.getline(phone, 15);
                if (!regex_match(phone, regex("^\\d{11}$"))) {
                    throw invalid_argument("Invalid phone number. Please enter up to 11 digits only.");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }

        while (true) {
            try {
                cout << "Enter Account Type (S-Savings/C-Current): ";
                cin >> type;
                type = toupper(type);
                if (type != 'S' && type != 'C') {
                    throw invalid_argument("Invalid account type. Please enter 'S' for Savings or 'C' for Current.");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }

        while (true) {
            try {
                cout << "Enter Initial Deposit (Minimum Rs. 1000): ";
                cin >> balance;
                if (balance < 1000) {
                    throw invalid_argument("Initial deposit must be at least Rs 1000.");
                }
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
            catch (const ios_base::failure& e) {
                cout << "Invalid input. Please enter a numeric value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        saveToFile();
        Transaction t("Opening Balance", balance, "INITIAL", accountNumber, balance);
        string filename = string(accountNumber) + "TransactionDetails.dat";
        t.saveToFile(filename);
    }

    void generateMonthlyStatement(int month, int year) const {
        string filename = string(accountNumber) + "TransactionDetails.dat";
        ifstream file(filename, ios::binary);

        if (!file) {
            cout << "\nNo transaction history found!";
            return;
        }

        cout << "\n=== Monthly Statement for " << month << "/" << year << " ===\n";
        Transaction t;
        bool found = false;

        while (file.read(reinterpret_cast<char*>(&t), sizeof(Transaction))) {
            string date = t.getDate();
            int transDay, transMonth, transYear;

            sscanf(date.c_str(), "%d/%d/%d", &transDay, &transMonth, &transYear);

            if (transMonth == month && transYear == year) {
                t.display();
                found = true;
            }
        }

        if (!found) {
            cout << "\nNo transactions found for the specified month and year!";
        }

        file.close();
    }


    string getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    double getBalance() const { return balance; }
    bool getIsActive() const { return isActive; }
    char getType() const { return type; }

    void setInactive() {
        isActive = false;
        saveToFile();
    }

    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            Transaction t("Deposit", amount, "CASH", accountNumber, balance);
            string filename = string(accountNumber) + "TransactionDetails.dat";
            t.saveToFile(filename);
            saveToFile();
            return true;
        }
        return false;
    }

    bool withdraw(double amount) {

        if (amount > 0 && (balance - amount) >= MIN_INITIAL_BALANCE) {
            balance -= amount;
            Transaction t("Withdrawal", amount, accountNumber, "CASH", balance);
            string filename = string(accountNumber) + "TransactionDetails.dat";
            t.saveToFile(filename);
            saveToFile();
            return true;
        }
        return false;
    }

    bool transfer(Account& recipient, double amount) {

        if (amount > 0 && (balance - amount) >= MIN_INITIAL_BALANCE) {
            balance -= amount;
            recipient.balance += amount;

            Transaction tSender("Transfer Sent", amount, accountNumber, recipient.accountNumber, balance);
            string senderFile = string(accountNumber) + "TransactionDetails.dat";
            tSender.saveToFile(senderFile);

            Transaction tRecipient("Transfer Received", amount, accountNumber, recipient.accountNumber, recipient.balance);
            string recipientFile = string(recipient.accountNumber) + "TransactionDetails.dat";
            tRecipient.saveToFile(recipientFile);

            saveToFile();
            recipient.saveToFile();
            return true;
        }
        return false;
    }

    void displayDetails() const {
        cout << "\n==== Account Details ===="
            << "\nAccount Number: " << accountNumber
            << "\nName: " << name
            << "\nEmail: " << email
            << "\nPhone: " << phone
            << "\nAccount Type: " << (type == 'S' ? "Savings" : "Current")
            << "\nBalance: PKR" << fixed << setprecision(2) << balance
            << "\nStatus: " << (isActive ? "Active" : "Inactive")
            << "\n======================" << endl;
    }

    void saveToFile() const {
        bool found = false;
        fstream file("AccountsDetails.dat", ios::binary | ios::in | ios::out);

        if (!file) {
            ofstream newFile("AccountsDetails.dat", ios::binary);
            newFile.write(reinterpret_cast<const char*>(this), sizeof(Account));
            newFile.close();
            return;
        }

        Account temp;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Account))) {
            if (strcmp(temp.accountNumber, this->accountNumber) == 0) {
                found = true;
                file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
                file.write(reinterpret_cast<const char*>(this), sizeof(Account));
                break;
            }
        }

        if (!found) {
            file.clear();
            file.seekp(0, ios::end);
            file.write(reinterpret_cast<const char*>(this), sizeof(Account));
        }
        file.close();

        string filename = string(accountNumber) + "Details.dat";
        ofstream accFile(filename, ios::binary);
        accFile.write(reinterpret_cast<const char*>(this), sizeof(Account));
        accFile.close();
    }

    static Account* findAccount(const string& accNum) {
        ifstream file("AccountsDetails.dat", ios::binary);
        if (!file) return nullptr;

        Account* acc = new Account();
        while (file.read(reinterpret_cast<char*>(acc), sizeof(Account))) {
            if (acc->getAccountNumber() == accNum) {
                file.close();
                return acc;
            }
        }
        file.close();
        delete acc;
        return nullptr;
    }
};

#endif
