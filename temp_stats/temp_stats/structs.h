#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

//global constants
static int constexpr MinYear = 1600;
static int constexpr MaxYear = 2200;
static int constexpr NotMonth = -1;
static int constexpr NotTemperature = -999;
static int constexpr MinTemp = -100;
static int constexpr MaxTemp = 100;
static char constexpr Celsius = 'C';
static char constexpr Fareneight = 'F';

static std::vector<std::string> monthtbl;		//vector with months to convert months to int

struct Reading {			//takes in input in the form of readings, then the readings are converted to days/hours in the vect
	int day;
	int hour;
	int temperature;
	char mesure;
};
struct Day {					//vector of hours
	bool init;
	int daynum;
	std::vector<double> Hours;
	Day():
		daynum(0), Hours(24), init(false)
	{
		for (int i = 0; i < Hours.size(); i++) Hours[i] = NotTemperature;
	}
};
struct Month {				//vector of days uninitialized by default at NotMonth
	int monthnum;
	std::vector<Day> Days;
	Month():
		monthnum(NotMonth), Days(32)
	{
		for (int i = 1; i < Days.size(); i++) Days[i].daynum = i;
	}
};
struct Year {					//vector of months
	int yearnum;
	std::vector<Month> Months;

	Year() :
		yearnum(0), Months(12)
	{}
};

struct OperatedMonth {		//struct for months with operated data in them
	int monthnum;
	double averagetmp;
	double mediantmp;
	int readings;

	OperatedMonth() :
		monthnum(NotMonth), averagetmp(NotTemperature), mediantmp(NotTemperature), readings(0)
	{}
};
struct OperatedYear {		//structs for years who's data have been operated on so months don't need a vector of days anymore
	int yearnum;
	double averagetmp;
	double mediantmp;
	int readings;
	std::vector<OperatedMonth> Months;

	OperatedYear() :
		yearnum(0), averagetmp(NotTemperature), mediantmp(NotTemperature), readings(0)
	{}
};


void InitMonthTbl();				//puts the months into monthtbl
int MonthToInt(std::string month);		//converts a month to an int
std::string IntToMonth(int x);

bool IsYearValid(Year year);			//checks if years are in the range minyear maxyear
bool IsReadingValid(Reading read);		//checks if readings are valid checking days, hours, temperature and mesure

//input operators overloads
std::istream& operator >>(std::istream& is, Year& y);
std::istream& operator >>(std::istream& is, Month& m);
std::istream& operator >>(std::istream& is, Reading& r);

//output overloads
std::ostream& operator <<(std::ostream& os, OperatedYear& oy);
std::ostream& operator <<(std::ostream& os, OperatedMonth& om);