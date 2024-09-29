#pragma once
#include <iostream>
#include <vector>
#include <fstream>

// global constants for checking purposes and initializing/deinitializing
static int constexpr NotMonth = -1;
static int constexpr NotDay = -1;
static int constexpr NotHour = -1;
static int constexpr NotTemperature = -999;
static int constexpr MinTemp = -100;
static int constexpr MaxTemp = 100;
static int constexpr MinYear = 1700;
static int constexpr MaxYear = 2200;
static char constexpr Celsius = 'C';
static char constexpr Fareneight = 'F';
static char constexpr NotMesure = '0';

//token constants
static char constexpr NotToken = '0';
static char constexpr DateToken = '1';
static char constexpr NewToken = 'N';
static std::string const NewCommand = "new";
static char constexpr RmToken = 'R';
static std::string const RemoveCommand = "rm";
static char constexpr ModToken = 'M';
static std::string const ModifyCommand = "mod";
static char constexpr QuitToken = 'Q';
static std::string const QuitCommand = "quit";
static char constexpr PrintToken = 'P';
static std::string const PrintCommand = "print";
static char constexpr ExportToken = 'E';
static std::string const ExportCommand = "export";
static char constexpr ParameterToken = 'A';


struct Date {                   //Provides locations in the vector to find readings
    int Year;
    int Month;
    int Day;
    int Hour;
    Date() :
        Year(0), Month(NotMonth), Day(NotDay), Hour(NotHour)
    {}
    Date(int year, int month, int day, int hour) :
        Year(year), Month(month), Day(day), Hour(hour)
    {}
};
struct DatedReading {               //Provides a Reading and its location in the vector, so it's a dated reading
    int Year;
    int Month;
    int Day;
    int Hour;
    int Temperature;
    char Mesure;

    DatedReading() :
        Year(0), Month(NotMonth), Day(NotDay), Hour(NotHour), Temperature(NotTemperature), Mesure(NotMesure)
    {}
    DatedReading(int year, int month, int day, int hour, int temperature, char mesure) :
        Year(year), Month(month), Day(day), Hour(hour), Temperature(temperature), Mesure(mesure)
    {}
};
struct Reading {            //provides a single reading composed of day, hour, temperature and mesure
    int Day;
    int Hour;
    int Temperature;
    char Mesure;

    Reading() :
        Hour(NotHour), Temperature(NotTemperature), Mesure(NotMesure), Day(NotDay)
    {}

    Reading(int day, int hour, int temperature, char mesure) :
       Day(day), Hour(hour), Temperature(temperature), Mesure(mesure)
    {}
};

struct Token {              //handles command and values
    std::string Command;    //command in itself
    char Tk;                //identification token
    Date ReadingDate;      // handles dates

    Token() :                           //default
        Command(""), Tk(NotToken)
    {}
    Token(Date date) :                                  //date token
        Command(""), Tk(DateToken), ReadingDate(date)
    {}
    Token(std::string command, char tk) :               //command token
        Command(command), Tk(tk)
    {}
};

struct Hour {                                   //similar to reading but used in the vector the hour of days can contain a temperature and mesure if not they are not initialized (-999, '0')
    double Temperature;
    char Mesure;

    Hour() :
        Temperature(NotTemperature), Mesure('0')
    {}
};
struct Day {                //composed of an initialized parameter and an int for it's number, as a vector of 0-23 hours               
    bool init;
    int day;
    std::vector<Hour> Hours;

    Day() :
        Hours(24), init(false), day(0)
    {
    }
};
struct Month {                  //composed of an int and a vector of 1-31 days the int handles whenever or not the month is initialized, uninitialized value is Notmonth
    int month;
    std::vector<Day> Days;

    Month() :
        Days(32), month(NotMonth)
    {
        for (int i = 0; i < Days.size(); i++) Days[i].day = i;          //provides the day's in the vector with a number
    }
};
struct Year {                           //composed of an int to identify its number and a vector of 0-11 months
    int year;
    std::vector<Month> Months;

    Year() :
        Months(12), year(0)
    {}
};

static std::vector<std::string> MonthTbl;           //table for string months
void InitMonthTbl();                        //inits MonthTbl by putting months in
std::string IntToMonth(int m);      //converts an int month to a string month using monthtbl


std::istream& operator >>(std::istream& is, Date& d);       //handles operator input for date
std::istream& operator >>(std::istream& is, DatedReading& ri);  //operator handling input for DatedReading

std::ostream& operator <<(std::ostream& os, const DatedReading& ri);  //operator handling output for DatedReading
std::ostream& operator <<(std::ostream& os, const Year& y);  //operator handling output for DatedReading
std::ostream& operator <<(std::ostream& os, const Month& m);  //operator handling output for DatedReading
std::ostream& operator <<(std::ostream& os, const Day& d);  //operator handling output for DatedReading
std::ostream& operator <<(std::ostream& os, const Reading& r);  //operator handling output for DatedReading

bool operator >(const Date& d1, const Date& d2);
bool operator <(const Date& d1, const Date& d2);
bool operator ==(const Date& d1, const Date& d2);

class InputHandler
{
private:
    static std::vector<Year> YearVec;                   //input vector
    static std::vector<Date> ReadingLocations;          //store locations of every reading to make finding reading easy

public:

    Token GetToken();                           //gets a token from cin

    bool IsDatedReadingValid(const DatedReading& r);    //checks for validity of DatedReading object
    bool IsDateValid(const Date& d);
    bool IsYearInVec(const int& year);          //searches a year in the vector returns true if found
    bool IsReadingInVec(const Date& readdate);     //checks if a reading is in the vector using his date

    int GetYearIndex(const int& year);           //searches a year in the vector returns an index if found
    DatedReading GetDatedReading(const Date& date); //gets a reading from a date

    void AddReading(const DatedReading& input);    //adds a reading in the vector (if year of the read isn't already in the vector it adds it), cannot modify already existent readings
    void RmReading(const Date& date);     //removes a reading
    void ModReading(const DatedReading& input);    //modify a reading

                                //after deleting something program rechecks the vector and determines if specific months and days need to be uninitialized as they dont have any readings, if a year is empty it gets deleted
    void NormalizeYear(const int& year);               //uninitializes the year if empty (deletes it)
    void NormalizeMonth(const int& year, const int& month);   //unitializes the month if empty(sets it to NotAMonth)
    void NormalizeDay(const int& year, const int& month, const int& day); //unitializes the day if empty (sets init to false)

    void SortYearVec();         //sorts vectors by smallest year to largest year/smallest date to largest date
    void SortLocationVec();

    void PrintDatedReadings(); //prints all Readings
    void ExportVector(std::string filename); //exports the vector to a file in the { year { month (day hour temperature mesure)}} format
};

