#include "structs.h"
#include "utility.h"

//format { yyyy { mm (dd hh temperature mesure)}}

std::istream& operator >>(std::istream& is, Year& y) {			//overload for year
	char ch1;
	is >> ch1;					
	if (ch1 != '{') {						//checks if there is an open bracket signaling a year start if there isn't it fails and puts back the character so that the caller can handle with it
		is.unget();
		is.clear(std::ios_base::failbit);
		return is;
	}

	int year;							//gets the year and error handling
	is >> year;
	if (is.fail()) {
		is.clear(std::ios_base::badbit);
		error("bad year input");
	}
	
	Year tempyear;				//creates a temporary year to then substitute the input request
	Month tempmonth;			//temporary month that after getting the input gets put in the year month vector
	tempyear.yearnum = year;
	if (!IsYearValid(tempyear)) {			//checks if year number is valid
		is.clear(std::ios_base::badbit);
		error("unvalid year"); 
	}

	while (is >> tempmonth) {								//gets all the months in the year, when the months are finished the month input operator will fail triggering end of loop that will search for a year terminator
		tempyear.Months[tempmonth.monthnum] = tempmonth;	//puts the inputted month into the months vector in year
	}
	EndOfLoop(is, '}', "bad terminator for year");

	y = tempyear;		//substitutes input requester with tempyear
	return is;
}
std::istream& operator >>(std::istream& is, Month& m) {
	char ch1;
	is >> ch1;
	if (ch1 != '{') {						//checks for month opening if not found puts the character back and lets the caller handle with the fail
		is.unget();
		is.clear(std::ios_base::failbit);
		return is;
	}

	std::string monthstr;				//gets the month
	is >> monthstr;
	if (is.fail()) {						//error handling
		is.clear(std::ios_base::badbit);
		error("bad month input");
	}

	Month tempmonth;			//temporary month for substituting the input request later on
	Reading tempread;			//temporary read to store all the readings in the month
	int doubles = 0;			//amount od duplicate/invalid readings
	int invalids = 0;
	tempmonth.monthnum = MonthToInt(monthstr);		//converts the month string to number and puts it back into monthnum

	while (is >> tempread) {				//for every reading in the month
		if (!IsReadingValid(tempread)) {		//checks for validity first and continues to next loop if found as as invalid readings could cause vector out of subscript exception
			invalids++;
			continue;
		}
		if (tempmonth.Days[tempread.day].Hours[tempread.hour] != NotTemperature) doubles++;			//checks for duplicates checking if the hour in the month is initialized
		if (tempread.mesure == Fareneight) tempread.temperature = ToCelsius(tempread.temperature);	//converts temperature

		tempmonth.Days[tempread.day].init = true;									//initializes day
		tempmonth.Days[tempread.day].Hours[tempread.hour] = tempread.temperature;		//copys temperature
	}
	EndOfLoop(is, '}', "bad terminator for month");		//when reading input fails checks for month terminator

	if (invalids) {						//errors if there are invalids or duplicates
		is.clear(std::ios_base::badbit);
		error("invalid readings: ", invalids);
	}
	if (doubles) {
		is.clear(std::ios_base::badbit);
		error("double readings: ", doubles);
	}

	m = tempmonth;			//copyes the tempmonth with the input to the input request
	return is;
}
std::istream& operator >>(std::istream& is, Reading& r) {
	char ch1;
	is >> ch1;
	if (ch1 != '(') {						//checks for opening char if not found the caller handles the error
		is.unget();
		is.clear(std::ios_base::failbit);
		return is;
	}

	int day, hour, temperature;
	char mesure, ch2;

	is >> day >> hour >> temperature >> mesure >> ch2;			//gets all the information and closing bracket
	if (is.fail()) {						//format and fail errors handler
		is.clear(std::ios_base::badbit);
		error("bad reading");
	}
	if (ch2 != ')') {
		is.clear(std::ios_base::badbit);
		error("bad reading terminator");
	}

	Reading tempread;						//creates a temporary reading and then substitutes it, validity checking is done by the caller
	tempread.day = day;
	tempread.hour = hour;
	tempread.temperature = temperature;
	tempread.mesure = mesure;

	r = tempread;
	return is;
}

std::ostream& operator <<(std::ostream& os, OperatedYear& oy) {
	os << "\n" << std::left << std::setw(16) << oy.yearnum << std::left << std::setw(10) << oy.averagetmp << std::left << std::setw(12) << oy.mediantmp << oy.readings << "\n\n";	//various setw gibberish to make evrything allineated
	for (int i = 0; i < oy.Months.size(); i++) {
		os << std::left << std::setw(6) << "" << oy.Months[i];		//prints every month of the year
	}
	return os;
}
std::ostream& operator <<(std::ostream& os, OperatedMonth& om) {
	os << std::left << std::setw(10) << IntToMonth(om.monthnum);
	os << std::left << std::setw(10) << om.averagetmp << std::left << std::setw(12) << om.mediantmp << om.readings << "\n";		//other setw gibberish
	return os;
}


bool IsYearValid(Year year) {													//checks if year is in range
	if (year.yearnum < MinYear || year.yearnum > MaxYear) return false;
	return true;
}	
bool IsReadingValid(Reading read) {							//checks for validity of reading day hour temp and mesure	
	if (read.day < 1 || read.day > 31) return false;
	if (read.hour < 0 || read.hour > 23) return false;
	if (read.temperature < MinTemp || read.temperature > MaxTemp) return false;
	if (read.mesure != Celsius && read.mesure != Fareneight) return false;
	return true;
}

void InitMonthTbl() {					//initializes monthtbl
	monthtbl.push_back("January");
	monthtbl.push_back("February");
	monthtbl.push_back("March");
	monthtbl.push_back("April");
	monthtbl.push_back("May");
	monthtbl.push_back("June");
	monthtbl.push_back("July");
	monthtbl.push_back("August");
	monthtbl.push_back("September");
	monthtbl.push_back("October");
	monthtbl.push_back("November");
	monthtbl.push_back("December");
}
int MonthToInt(std::string month) {					//month to int converts a string month to an int, has an unplanned function of checking month validity
	for (int i = 0; i < monthtbl.size(); i++) {
		if (month == monthtbl[i]) return i;
	}
	error("unvalid month can't convert");
}
std::string IntToMonth(int x) {										//converts an int to a string month
	if (x < 0 || x > 11) error("unvalid int month can't convert");
	return monthtbl[x];
}