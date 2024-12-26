#pragma once
#ifndef BANKINGSYSTEM_H
#define BANKINGSYSTEM_H

#include <iostream>
#include "Admin.h"
#include "Account.h"
#include "Transaction.h"

class BankingSystem {
private:
    static Account* currentAccount;

public:
    static void displayMainMenu() {
        while (true) {
            system("cls");
            cout << "\n=== Banking System ==="
                << "\n1. Admin Login"
                << "\n2. User Login"
                << "\n3. Create New Account"
                << "\n4. Exit"
                << "\nEnter choice: ";

            char choice;

            try {
                cin >> choice;
                if (cin.fail()) {
                    throw invalid_argument("Input must be a valid choice (1-4).");
                }

                switch (choice) {
                case '1':
                    adminLogin();
                    break;
                case '2':
                    userLogin();
                    break;
                case '3':
                    createAccount();
                    break;
                case '4':
                    cout << "\nThank you for using our banking system!\n";
                    return;
                default:
                    throw out_of_range("Invalid choice! Please select between 1 and 4.");
                }
            }
            catch (const exception& e) {
                cout << "\nError: " << e.what();
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    static void adminLogin() {
        try {
            int password;
            cout << "\nEnter admin password: ";
            cin >> password;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Password must be a valid integer.");
            }

            if (Admin::authenticate(password)) {
                displayAdminMenu();
            }
            else {
                throw runtime_error("Invalid admin password!");
            }
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what();
        }
    }

    static void userLogin() {
        try {
            string accNum, password;
            cout << "\nEnter account number: ";
            cin >> accNum;
            if (cin.fail() || accNum.empty()) {
                throw invalid_argument("Account number must be a non-empty string.");
            }

            cout << "Enter password: ";
            cin >> password;
            if (cin.fail() || password.empty()) {
                throw invalid_argument("Password must be a non-empty string.");
            }

            Account* acc = Account::findAccount(accNum);
            if (acc && acc->authenticate(password)) {
                currentAccount = acc;
                displayUserMenu();
                delete currentAccount;
                currentAccount = nullptr;
            }
            else {
                delete acc;
                throw runtime_error("Invalid credentials!");
            }
        }
        catch (const exception& e) {
            cout << "\nError: " << e.what();
        }
    }

    static void createAccount() {
        Account newAccount;
        newAccount.createAccount();
        cout << "\nAccount created successfully!";
    }

    static void displayAdminMenu() {
        while (true) {
            system("cls");
            cout << "\n=== Admin Menu ==="
                << "\n1. View All Accounts"
                << "\n2. Delete Account"
                << "\n3. Search a Bank Account"
                << "\n4. Logout"
                << "\nEnter choice: ";

            char choice;
            cin >> choice;

            switch (choice) {
            case '1':
                Admin::viewAllAccounts();
                break;
            case '2': {
                string accNum;
                cout << "\nEnter account number to delete: ";
                cin >> accNum;
                Admin::deleteAccount(accNum);
                break;
            }
            case '3': {
                string accNum;
                cout << "\nEnter account number to search: ";
                cin >> accNum;
                Admin::searchAccountDetails(accNum);
                break;
            }
            case '4':
                return;
            default:
                cout << "\nInvalid choice!";
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    static void generateMonthlyStatement() {
        if (currentAccount) {
            try {
                int month, year;

                cout << "\nEnter month (1-12): ";
                cin >> month;

                if (cin.fail() || month < 1 || month > 12) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid month. Please enter a value between 1 and 12.");
                }

                cout << "Enter year (e.g., 2024): ";
                cin >> year;

                if (cin.fail() || year < 1000 || year > 9999) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid year. Please enter a valid 4-digit year.");
                }

                currentAccount->generateMonthlyStatement(month, year);
            }
            catch (const invalid_argument& e) {
                cerr << "\nInput Error: " << e.what() << endl;
            }
            catch (const runtime_error& e) {
                cerr << "\nError: " << e.what() << endl;
            }
            catch (...) {
                cerr << "\nAn unexpected error occurred." << endl;
            }
        }
        else {
            cerr << "\nError: No account is currently logged in." << endl;
        }
    }

    static void displayUserMenu() {
        while (true) {
            system("cls");
            cout << "\n=== User Menu ==="
                << "\n1. View Account Details"
                << "\n2. View Balance"
                << "\n3. Deposit Money"
                << "\n4. Withdraw Money"
                << "\n5. Transfer Money"
                << "\n6. View Transaction History"
                << "\n7. Generate Monthly Statement"
                << "\n8. Logout"
                << "\nEnter choice: ";

            char choice;
            cin >> choice;

            switch (choice) {
            case '1':
                viewAccountDetails();
                break;
            case '2':
                viewBalance();
                break;
            case '3':
                deposit();
                break;
            case '4':
                withdraw();
                break;
            case '5':
                transfer();
                break;
            case '6':
                viewTransactionHistory();
                break;
            case '7':
                generateMonthlyStatement();
                break;
            case '8':
                return;
            default:
                cout << "\nInvalid choice!";
            }

            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    static void viewAccountDetails() {
        if (currentAccount) {
            currentAccount->displayDetails();
        }
    }

    static void viewBalance() {
        if (currentAccount) {
            cout << "\nCurrent Balance: PKR" << fixed << setprecision(2)
                << currentAccount->getBalance();
        }
    }

    static void deposit() {
        if (currentAccount) {
            try {
                double amount;
                cout << "\nEnter amount to deposit: PKR";
                cin >> amount;

                if (cin.fail() || amount <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid amount. Please enter a positive number.");
                }

                if (currentAccount->deposit(amount)) {
                    cout << "\nDeposit successful!"
                        << "\nNew balance: PKR" << currentAccount->getBalance();
                }
                else {
                    throw runtime_error("Deposit failed! Please try again.");
                }
            }
            catch (const invalid_argument& e) {
                cerr << "\nInput Error: " << e.what() << endl;
            }
            catch (const runtime_error& e) {
                cerr << "\nError: " << e.what() << endl;
            }
            catch (...) {
                cerr << "\nAn unexpected error occurred." << endl;
            }
        }
        else {
            cerr << "\nError: No account is currently logged in." << endl;
        }
    }

    static void withdraw() {
        if (currentAccount) {
            try {
                double amount;
                cout << "\nEnter amount to withdraw: PKR";
                cin >> amount;

                if (cin.fail() || amount <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("Invalid amount. Please enter a positive number.");
                }

                if (currentAccount->withdraw(amount)) {
                    cout << "\nWithdrawal successful!"
                        << "\nNew balance: PKR" << currentAccount->getBalance();
                }
                else {
                    throw runtime_error("Withdrawal failed! Please check your balance and minimum balance requirements.");
                }
            }
            catch (const invalid_argument& e) {
                cerr << "\nInput Error: " << e.what() << endl;
            }
            catch (const runtime_error& e) {
                cerr << "\nError: " << e.what() << endl;
            }
            catch (...) {
                cerr << "\nAn unexpected error occurred." << endl;
            }
        }
        else {
            cerr << "\nError: No account is currently logged in." << endl;
        }
    }

    static void transfer() {
        if (currentAccount) {
            try {
                string recipientAccNum;
                double amount;

                cout << "\nEnter recipient's account number: ";
                cin >> recipientAccNum;

                // Ensure valid input
                if (recipientAccNum.empty()) {
                    throw invalid_argument("Account number cannot be empty.");
                }

                Account* recipientAcc = Account::findAccount(recipientAccNum);
                if (!recipientAcc) {
                    throw runtime_error("Recipient account not found!");
                }

                cout << "\nRecipient Details:"
                    << "\nName: " << recipientAcc->getName()
                    << "\nAccount Number: " << recipientAcc->getAccountNumber();

                cout << "\n\nEnter amount to transfer: PKR";
                cin >> amount;

                if (cin.fail() || amount <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    throw std::invalid_argument("Invalid transfer amount. Please enter a positive number.");
                }

                cout << "\nConfirm transfer? (Y/N): ";
                char confirm;
                cin >> confirm;

                if (toupper(confirm) == 'Y') {
                    if (currentAccount->transfer(*recipientAcc, amount)) {
                        cout << "\nTransfer successful!"
                            << "\nNew balance: PKR" << currentAccount->getBalance();
                    }
                    else {
                        throw std::runtime_error("Transfer failed! Please check your balance and minimum balance requirements.");
                    }
                }
                else if (toupper(confirm) == 'N') {
                    cout << "\nTransfer cancelled.";
                }
                else {
                    throw invalid_argument("Invalid confirmation input. Please enter 'Y' or 'N'.");
                }

                delete recipientAcc;
            }
            catch (const invalid_argument& e) {
                cerr << "\nInput Error: " << e.what() << endl;
            }
            catch (const std::runtime_error& e) {
                cerr << "\nError: " << e.what() << endl;
            }
            catch (...) {
                cerr << "\nAn unexpected error occurred." << endl;
            }
        }
        else {
            cerr << "\nError: No account is currently logged in." << endl;
        }
    }

    static void viewTransactionHistory() {
        if (currentAccount) {
            string filename = currentAccount->getAccountNumber() + "TransactionDetails.dat";
            ifstream file(filename, ios::binary);

            if (!file) {
                cout << "\nNo transaction history found!";
                return;
            }

            cout << "\n=== Transaction History ===\n";
            Transaction t;
            while (file.read(reinterpret_cast<char*>(&t), sizeof(Transaction))) {
                t.display();
            }
            file.close();
        }
    };

};

Account* BankingSystem::currentAccount = nullptr;

#endif
