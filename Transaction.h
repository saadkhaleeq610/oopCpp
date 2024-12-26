#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include "Date.h" 

using namespace std;

class Transaction {
private:
    char date[11];             
    char type[20];             
    char fromAccount[13];      
    char toAccount[13];        
    double amount;             
    double balanceAfter;       

public:
    Transaction() {}

    Transaction(const string& t, double amt, const string& from, const string& to, double bal) {
        Date d;
        strncpy(date, d.toString().c_str(), sizeof(date) - 1);
        date[sizeof(date) - 1] = '\0';

        strncpy(type, t.c_str(), sizeof(type) - 1);
        type[sizeof(type) - 1] = '\0';

        strncpy(fromAccount, from.c_str(), sizeof(fromAccount) - 1);
        fromAccount[sizeof(fromAccount) - 1] = '\0';

        strncpy(toAccount, to.c_str(), sizeof(toAccount) - 1);
        toAccount[sizeof(toAccount) - 1] = '\0';

        amount = amt;
        balanceAfter = bal;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::binary | ios::app);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(this), sizeof(Transaction));
            file.close();
        }
    }

    std::string getDate() const {
        return std::string(date);
    }

    void display() const {
        cout << "\n================================"
            << "\nDate: " << date
            << "\nType: " << type
            << "\nAmount: PKR" << fixed << setprecision(2) << amount
            << "\nFrom Account: " << fromAccount
            << "\nTo Account: " << toAccount
            << "\nBalance After: PKR" << balanceAfter
            << "\n================================" << endl;
    }
};

#endif