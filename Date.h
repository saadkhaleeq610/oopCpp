#pragma once
#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

class Date {
	private:
	    int day, month, year;
	
	public:
	    Date() {
	        time_t now = time(0);
	        tm* ltm = localtime(&now);
	        day = ltm->tm_mday;
	        month = 1 + ltm->tm_mon;
	        year = 1900 + ltm->tm_year;
	    }
	
	    string toString() const {
	        stringstream ss;
	        ss << setfill('0') << setw(2) << day << "/"
	            << setfill('0') << setw(2) << month << "/"
	            << year;
	        return ss.str();
	    }
	};

#endif
