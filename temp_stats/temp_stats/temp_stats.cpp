#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "structs.h"
#include "utility.h"

void GetInput(std::vector<Year>& yearvec);        //gets input from file

OperatedMonth OperateMonth(const Month& month);    //operate a month and turns it into and OperatedMonth calculating mean and median
OperatedYear OperateYear(const Year& year);        //operates on a year calculating mean and median by getting 12 OperatedMonths
std::vector<OperatedYear> OperateInput(const std::vector<Year>& YearVec);        //operates on input

void SendOutput(std::vector<OperatedYear>& YearsStats);      //sends operated output to file

int main()
{
    std::vector<Year> Yearvec;      //vector of input years
    try {
        InitMonthTbl();
        GetInput(Yearvec);
        std::vector<OperatedYear> YearStats = OperateInput(Yearvec);        //operates on input
        SendOutput(YearStats);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }
}

void GetInput(std::vector<Year>& YearVec) {
    std::string filename;                       //accesses the file

    std::cout << "please enter file name:\n";
    std::cin >> filename;
    std::ifstream file(filename);
    if (!file) error("file nonexistent");
    
    Year TempYear;                          //gets input using overloaded >> operator
    while (file >> TempYear) {
        YearVec.push_back(TempYear);
    }
    std::cout << "all done\n";
}

std::vector<OperatedYear> OperateInput(const std::vector<Year>& yearvec) {
    std::vector<OperatedYear> YearsStats;                                   //makes an operatedyear vector to store all the various data
    for (int i = 0; i < yearvec.size(); i++) {
        OperatedYear year = OperateYear(yearvec[i]);                        //operates every year in the original vector and turns them into operated years
        YearsStats.push_back(year);
    }
    std::cout << "operated input!\n";
    return YearsStats;
}
OperatedYear OperateYear(const Year& year) {
    OperatedYear YearStats;                     //creates an operated year to return at the end and sets it up with the yearnum
    YearStats.yearnum = year.yearnum;
    for (int i = 0; i < year.Months.size(); i++) {          //for every initialized month turn it into an OperatedMonth and add it's readings
        if (year.Months[i].monthnum != NotMonth) {
            OperatedMonth month = OperateMonth(year.Months[i]);
            YearStats.readings += month.readings;
            YearStats.Months.push_back(month);
        }
    }
    std::vector<double> temperatures;                       //gets all the temperature from the months into a vector and calculates mean and median
    for (int i = 0; i < YearStats.Months.size(); i++) {
        temperatures.push_back(YearStats.Months[i].averagetmp);
    }
    SortTemperatures(temperatures);
    YearStats.averagetmp = CalculateMean(temperatures);
    YearStats.mediantmp = CalculateMedian(temperatures);
    return YearStats;
}
OperatedMonth OperateMonth(const Month& month) {
    OperatedMonth MonthStats;                       //creates an operatedmonth to return ad initializes it with the monthnumber
    MonthStats.monthnum = month.monthnum;
    std::vector<double> temperatures;
    for (int i = 1; i < month.Days.size(); i++) {               //gets all the initialized hours into the temperatures vector and calculates mean and median
        if (month.Days[i].init) {
            for (int y = 0; y < month.Days[i].Hours.size(); y++) {
                if (month.Days[i].Hours[y] != NotTemperature) temperatures.push_back(month.Days[i].Hours[y]);
            }
        }
    }
    SortTemperatures(temperatures);
    MonthStats.averagetmp = CalculateMean(temperatures);
    MonthStats.mediantmp = CalculateMedian(temperatures);
    MonthStats.readings = temperatures.size();              //updates how many readings the original month had
    return MonthStats;
}

void SendOutput(std::vector<OperatedYear>& YearsStats) {
    std::string filename;                                   //loads file

    std::cout << "please enter file name:\n";
    std::cin >> filename;
    std::ofstream file(filename);
    if (!file) error("file nonexistent");

    file << std::right << std::setw(24) << "Mean(C°)" << std::setw(12) << "Median(C°)" << std::setw(18) << "Readings number\n";     //colums
    file << "Year" << std::setw(8) << "Month\n";
    for (int i = 0; i < YearsStats.size(); i++) {       //prints every OperatedYear with overloaded << operator
        file << YearsStats[i];
    }
}