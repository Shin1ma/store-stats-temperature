#include "InputHandler.h"


void InitMonthTbl() {         //initializes monthtbl with pushbacks
    MonthTbl.push_back("January");
    MonthTbl.push_back("February");
    MonthTbl.push_back("March");
    MonthTbl.push_back("April");
    MonthTbl.push_back("May");
    MonthTbl.push_back("June");
    MonthTbl.push_back("July");
    MonthTbl.push_back("August");
    MonthTbl.push_back("September");
    MonthTbl.push_back("October");
    MonthTbl.push_back("November");
    MonthTbl.push_back("December");
}
std::string IntToMonth(int m) {               //transforms an int of range 0-11 to a month using monthtbl
    for (int i = 0; i < MonthTbl.size(); i++) {
        if (i == m) return MonthTbl[i];
    }
    throw std::runtime_error("Invalid month for conversion");
    return "-1";                                                //error code
}


//operator overloads for structs:

std::istream& operator >>(std::istream& is, Date& d) {      // /yyyy/mm/dd hh
    int year, month, day, hour;
    char ch1, ch2, ch3;

    is >> ch1;
    if (is.fail()) {                        //handles fails
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad Date input");
        return is;
    }
    if (ch1 != '/') {                       //checks for character that starts a date
        is.clear(std::ios_base::badbit);            //Used badbit as I have no use for incorrect input
        throw std::runtime_error("bad Date format");
        return is;
    }

    is >> year >> ch2 >> month >> ch3 >> day >> hour;           //handles the rest of the characters
    if (is.fail()) {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad Date input");
        return is;
    }
    if (ch2 != '/' || ch3 != '/') {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad date format");
        return is;
    }

    InputHandler ih;                        //used to check for IsDateValid()
    Date tempdate(year, month, day, hour);      //creates a date with the inputted values
    if (!ih.IsDateValid(tempdate)) {            //checks for validity
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("unvalid date");
        return is;
    }
    d = tempdate;           //copy tempdate to original input request
    return is;
}
std::istream& operator >>(std::istream& is, DatedReading& ri) {         // /yyyy/mm/dd hh temperature mesure
    int year, month, day, hour, temperature;
    char ch1, ch2, ch3, mesure;

    is >> ch1;
    if (is.fail()) {                                    //handles failstate of is
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad reading input");
        return is;
    }
    if (ch1 != '/') {                                   //checks starting character
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad reading format");
        return is;
    }

    is >> year >> ch2 >> month >> ch3 >> day >> hour >> temperature >> mesure;          //handles other characters
    if (is.fail()) {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad reading input");
        return is;
    }
    if (ch2 != '/' || ch3 != '/') {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad reading format");
        return is;
    }
    if (mesure != Celsius && mesure != Fareneight) {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("bad mesure");
        return is;
    }

    InputHandler ih;                                                        //checks for validity of datedreading
    DatedReading tempreading(year, month, day, hour, temperature, mesure);      //creates a tempreading to copy to check and copy over when done
    if (!ih.IsDatedReadingValid(tempreading)) {
        is.clear(std::ios_base::badbit);
        throw std::runtime_error("unvalid Reading");
        return is;
    }
    ri = tempreading;        //copies tempreading to original input request
    return is;
}

std::ostream& operator <<(std::ostream& os, const DatedReading& ri) {
    os << "\n/" << ri.Year << "/" << ri.Month << "/" << ri.Day << " " << ri.Hour << " " << ri.Temperature << " " << ri.Mesure << "\n";
    return os;
}
std::ostream& operator <<(std::ostream& os, const Year& y) {
    os << "{ " << y.year << " ";
    for (int i = 0; i < 12; i++) {              //outputs the initialized months
        if (y.Months[i].month != NotMonth) {
            os << y.Months[i];
        }
    }
    os << " }\n";
    return os;
}
std::ostream& operator <<(std::ostream& os, const Month& m) {
    os << "{ " << IntToMonth(m.month) << " ";
    for (int i = 1; i <= 31; i++) {             //outputs the initialized days
        if (m.Days[i].init) {
            os << m.Days[i];
        }
    }
    os << " }";
    return os;
}
std::ostream& operator <<(std::ostream& os, const Day& d) {
    for (int i = 0; i < 24; i++) {
        if (d.Hours[i].Temperature != NotTemperature) {                                 //transforms day's into readings for easier output
            Reading tempread(d.day, i, d.Hours[i].Temperature, d.Hours[i].Mesure);
            os << tempread;
        }
    }
    return os;
}
std::ostream& operator <<(std::ostream& os, const Reading& r) {                         //outputs readings
    os << "( " << r.Day << " " << r.Hour << " " << r.Temperature << r.Mesure << " )";
    return os;
}


bool operator >(const Date& d1, const Date& d2) {               //comparators for dates
    if (d1.Year > d2.Year) return true;
    if (d1.Year < d2.Year) return false;
    if (d1.Year == d2.Year) {
        if (d1.Month > d2.Month) return true;
        if (d1.Month < d2.Month) return false;
        if (d1.Month == d2.Month) {
            if (d1.Day > d2.Day) return true;
            if (d1.Day < d2.Day) return false;
            if (d1.Day == d2.Day) {
                if (d1.Hour > d2.Hour) return true;
                if (d1.Hour < d2.Hour) return false;
                if (d1.Hour == d2.Hour) return false;
            }
        }
    }
}
bool operator <(const Date& d1, const Date& d2) {
    if (d1.Year < d2.Year) return true;
    if (d1.Year > d2.Year) return false;
    if (d1.Year == d2.Year) {
        if (d1.Month < d2.Month) return true;
        if (d1.Month > d2.Month) return false;
        if (d1.Month == d2.Month) {
            if (d1.Day < d2.Day) return true;
            if (d1.Day > d2.Day) return false;
            if (d1.Day == d2.Day) {
                if (d1.Hour < d2.Hour) return true;
                if (d1.Hour > d2.Hour) return false;
                if (d1.Hour == d2.Hour) return false;
            }
        }
    }
}


bool operator ==(const Date& d1, const Date& d2) {
    if (d1.Year != d2.Year) return false;
    if (d1.Month != d2.Month) return false;
    if (d1.Day != d2.Day) return false;
    if (d1.Hour != d2.Hour) return false;
    return true;
}


