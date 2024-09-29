#include "InputHandler.h"




//class implementation:
     
std::vector<Year> InputHandler::YearVec;            //vector of years
std::vector<Date> InputHandler::ReadingLocations;   //vector of every reading location, located using dates


Token  InputHandler::GetToken() {           //gets a token from cin
    char ch;
    std::cin >> ch;
    if (ch == '/') {          //as this funtion is called at the start if ch is a date starter it must be a date or its unvalid
        Date date;
        std::cin.unget();
        std::cin >> date;
        return Token(date);
    }
    else {
        std::string command;    //if its a letter its considered a command and tokenized as so
        std::cin.unget();
        std::cin >> command;
        if (command == NewCommand) return Token(command, NewToken);
        if (command == RemoveCommand) return Token(command, RmToken);
        if (command == ModifyCommand) return Token(command, ModToken);
        if (command == PrintCommand) return Token(command, PrintToken);
        if (command == ExportCommand) return Token(command, ExportToken);
        if (command == QuitCommand) return Token(command, QuitToken);
        return Token(command, ParameterToken);
    }
}

bool  InputHandler::IsDatedReadingValid(const DatedReading& r) {            //checks validity of datedreading
    if (r.Year < MinYear || r.Year > MaxYear) return false;
    if (r.Month < 0 || r.Month > 11) return false;
    if (r.Day < 1 || r.Day > 31) return false;
    if (r.Hour < 0 || r.Hour > 23) return false;
    if (r.Temperature < MinTemp || r.Temperature > MaxTemp) return false;
    if (r.Mesure != 'C' && r.Mesure != 'F') return false;
    return true;
}

bool InputHandler::IsDateValid(const Date& d){                  //checks validity of date
    if (d.Year < MinYear || d.Year > MaxYear) return false;
    if (d.Month < 0 || d.Month > 11) return false;
    if (d.Day < 1 || d.Day > 31) return false;
    if (d.Hour < 0 || d.Hour > 23) return false;
    return true;
}
bool InputHandler::IsYearInVec(const int& year) {           //checks if a year is in the vector 
    for (int i = 0; i < YearVec.size(); i++) {
        if (year == YearVec[i].year) return true;;
    }
    return false;
}
bool InputHandler::IsReadingInVec(const Date& readdate) {       //checks if a reading is in the vector
    for (int i = 0; i < ReadingLocations.size(); i++) {
        if (readdate == ReadingLocations[i]) return true;           //tryes to find the date of the reading in the date vector
    }
    return false;
}

int InputHandler::GetYearIndex(const int& year) {               //get a vector index based on integer year
    for (int i = 0; i < YearVec.size(); i++) {
        if (year == YearVec[i].year) return i;
    }
    throw std::runtime_error("no year found in searchyear()");
}
DatedReading InputHandler::GetDatedReading(const Date& date) {
    if (!IsReadingInVec(date)) throw std::runtime_error("can't get reading as it doesn't exists");
    InputHandler ih;
    return DatedReading(date.Year, date.Month, date.Day, date.Hour, YearVec[ih.GetYearIndex(date.Year)].Months[date.Month].Days[date.Day].Hours[date.Hour].Temperature, YearVec[ih.GetYearIndex(date.Year)].Months[date.Month].Days[date.Day].Hours[date.Hour].Mesure);
}

void  InputHandler::AddReading(const DatedReading& input) {                             //adds a reading in the vector and adds a year if necessary
    if (!IsDatedReadingValid(input)) throw std::runtime_error("invalid reading input");     //handles validity
   
    if (IsYearInVec(input.Year)) {              //if the year is already in the vector

        int index = GetYearIndex(input.Year);       //gets the index

        if (IsReadingInVec(Date(input.Year, input.Month, input.Day, input.Hour))) throw std::runtime_error("can't modify an existing reading with new command");  //checks if we are modifying an existing reading and returns an error if so, to avoid errors the command to modify is different

        YearVec[index].Months[input.Month].month = input.Month;         //initializes month
        YearVec[index].Months[input.Month].Days[input.Day].init = true;     //initializes day
        YearVec[index].Months[input.Month].Days[input.Day].Hours[input.Hour].Temperature = input.Temperature;       //initializes temperature
        YearVec[index].Months[input.Month].Days[input.Day].Hours[input.Hour].Mesure = input.Mesure;             //initializes mesure
        ReadingLocations.push_back(Date(input.Year, input.Month, input.Day, input.Hour));  //gets the location of the reading in a vector using Date
        SortLocationVec();      //sorts location vector chronologically
        std::cout << "OK!\n";
    }
    else {

        Year year;              //creates a temporary year
        year.year = input.Year;                             //initializes all members
        year.Months[input.Month].month = input.Month;
        year.Months[input.Month].Days[input.Day].init = true;
        year.Months[input.Month].Days[input.Day].Hours[input.Hour].Temperature = input.Temperature;
        year.Months[input.Month].Days[input.Day].Hours[input.Hour].Mesure = input.Mesure;

        YearVec.push_back(year);        //pushes the temporary year into the vector
        SortYearVec();                  //sorts year vector chronologically
        ReadingLocations.push_back(Date(input.Year, input.Month, input.Day, input.Hour)); //gets the location of the reading in a vector using Date
        SortLocationVec();              //sorts location vector chronologically
        std::cout << "OK!\n";
    }
}
void  InputHandler::RmReading(const Date& date) {                                                           //removes a year (uses date to determine position)
    if (!IsYearInVec(date.Year)) throw std::runtime_error("can't remove a reading in an unexistant year");  //checks if the year is in the vector
    int index = GetYearIndex(date.Year);        //gets index for the year

    if (!IsReadingInVec(date)) throw std::runtime_error("can't remove, Unexistent reading");  //checks if requested reading is initialized
    
    YearVec[index].Months[date.Month].Days[date.Day].Hours[date.Hour].Temperature = NotTemperature;     //deinitializes the reading
    for (int i = 0; i < ReadingLocations.size(); i++) {
        if (date == ReadingLocations[i]) ReadingLocations.erase(ReadingLocations.begin() + i);      //deletes the reading location from the vector as the reading is deleted
    }

    NormalizeYear(date.Year);       //normalizes the year (checks if they need to deinitialize things look at normalize functions for details)
    std::cout << "OK!\n";
}
void  InputHandler::ModReading(const DatedReading& input) {         //modifyes a reading
    if (!IsDatedReadingValid(input)) throw std::runtime_error("unvalid reading can't modify");//checks for validity of input
    if (!IsYearInVec(input.Year)) throw std::runtime_error("can't modify a reading in an unexistant year");     //checks for year existence
    int index = GetYearIndex(input.Year);       //gets index

    if (!IsReadingInVec(Date(input.Year, input.Month, input.Day, input.Hour))) throw std::runtime_error("can't modify, Unexistent reading");  //check if the reading exists
    
    YearVec[index].Months[input.Month].Days[input.Day].Hours[input.Hour].Temperature = input.Temperature;       //modifyes temperature and mesure
    YearVec[index].Months[input.Month].Days[input.Day].Hours[input.Hour].Mesure = input.Mesure;
    std::cout << "OK\n";
}

void InputHandler::NormalizeYear(const int& year) {             //checks entire year to normalize it: if after a remove a month is empy it needs to be deinitialized, if the year is empty it gets deleted
    bool initialized = false;           //variable to check for emptiness
    if (!IsYearInVec(year)) throw std::runtime_error("can't normalize year, non existent year");       //checks if the year exists
    int index = GetYearIndex(year);

    for (int i = 0; i <= 11; i++) {         //normalizes all the months
        NormalizeMonth(year, i);
    }
    for (int i = 0; i <= 11; i++) {
        if (YearVec[index].Months[i].month != NotMonth) initialized = true;     //after the month normalization checks if at least one of the months is initialized 
    }

    if (!initialized) YearVec.erase(YearVec.begin() + index);       //if the variable remains as is it means the year is empty and can be deleted
}
void InputHandler::NormalizeMonth(const int& year, const int& month) {           //normalizes a month
    bool initialized = false;       //variable that checks for emptyness of month
    if (!IsYearInVec(year)) throw std::runtime_error("can't normalize month, non existent year");   //checks if year exist
    if (month > 11 || month < 0) throw std::runtime_error("can't normalize month, unvalid month");  //checks if month is valid
    int index = GetYearIndex(year);     //gets the index

    for (int i = 1; i <= 31; i++) {
        NormalizeDay(year, month, i);       //normalizes all the days
    }
    for (int i = 1; i <= 31; i++) {
        if (YearVec[index].Months[month].Days[i].init) initialized = true;      //checks if at least one day is initialized
    }

    if (initialized) YearVec[index].Months[month].month = month;        //if is not empty initialize it
    else YearVec[index].Months[month].month = NotMonth;     //if is empty deinitialize it
}
void InputHandler::NormalizeDay(const int& year, const int& month, const int& day) {        //normalized a day
    bool initialized = false;       //variable for emptiness
    if (!IsYearInVec(year)) throw std::runtime_error("can't normalize day, non existent year");     //checks if year exists
    if (month > 11 || month < 0) throw std::runtime_error("can't normalize day, unvalid month");        //validates month
    if (day > 31 || day < 1) throw std::runtime_error("can't normalize day, unvalid day");      //validates day
    int index = GetYearIndex(year);     //gets index

    for (int i = 0; i <= 23; i++) {
        if (YearVec[index].Months[month].Days[day].Hours[i].Temperature != NotTemperature) initialized = true;  //check if at least one of the hours is initialized
    }

    if (initialized) YearVec[index].Months[month].Days[day].init = true;        //if not empty initializes the day
    else YearVec[index].Months[month].Days[day].init = false;               //if empty deinitializes it
}

void InputHandler::SortYearVec() {          //sorts the vectors in chronological order
    std::vector<Year> backupvect = YearVec;     //creates a backup onto it will operate
    std::vector<Year> finalvec;         //final vector
    bool smallest;                  //is the element the smallest
    int tempvar;                    //temporary variable
    if (backupvect.size() != 1) {                           //if its only one element it's useless
        for (int i = 0; i < backupvect.size(); i++) {
            tempvar = backupvect[i].year;
            smallest = true;                                //assumes its the smallest
            for (int y = 0; y < backupvect.size(); y++) {
                if (tempvar > backupvect[y].year) smallest = false;         //check's if it's the smallest
            }
            if (smallest) {
                finalvec.push_back(backupvect[i]);              //if it is it adds it to finalvec, erases it from backupvec and restarts the loop at element 0
                backupvect.erase(backupvect.begin() + i);
                i = -1;
            }
        }
        YearVec = finalvec;
    }
}
void InputHandler::SortLocationVec() {                  //same as SortYearVec
    std::vector<Date> backupvect = ReadingLocations;
    std::vector<Date> finalvec;
    bool smallest;
    Date tempvar;
    if (backupvect.size() != 1) {
        for (int i = 0; i < backupvect.size(); i++) {
            tempvar = backupvect[i];
            smallest = true;
            for (int y = 0; y < backupvect.size(); y++) {
                if (tempvar > backupvect[y]) smallest = false;
            }
            if (smallest) {
                finalvec.push_back(backupvect[i]);
                backupvect.erase(backupvect.begin() + i);
                i = -1;
            }
        }
        ReadingLocations = finalvec;
    }
}

void InputHandler::PrintDatedReadings() {                   //prints all DatedReadings
    DatedReading tempread;
    for (int i = 0; i < ReadingLocations.size(); i++) {
       tempread = GetDatedReading(ReadingLocations[i]);
       std::cout << tempread;
    }
}
void InputHandler::ExportVector(std::string filename) {         //exports to file the vector
    std::ofstream file(filename);
    if (!file) throw std::runtime_error("file not found");
    for (int i = 0; i < YearVec.size(); i++) {
        file << YearVec[i];
    }
}