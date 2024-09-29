#include <iostream>
#include <vector>
#include "InputHandler.h"


//format {year [year]{month [month](reading)}}

static bool quitted = false;        //global variable that handles quitting, not good practice :(

void RunCommand(Token commandtk, InputHandler handler);     //runs the various commands using inputhandler
void GetInput(InputHandler handler);    //gets input and uses inputhandler to get tokens and process them

int main()
{
    InputHandler handler;           //gets handler object
    InitMonthTbl();     //initializes monthtbl

    while (true && !quitted) {      //runs till quitted
        try {
            std::cout << "\ncurrent commands: new [reading], mod [reading], rm [date], print, export [filename], quit\n";
            std::cout << "reading format: /yyyy/mm/dd hh temperature mesure(C or F)\n";
            std::cout << "date format: /yyyy/mm/dd hh       months go from 0-11, days 1-31, hours 0-23\n";
            std::cout << "if you type a date it will give you the temperature at that date if there is one recorded\n";
            GetInput(handler);          //gets input

        }
        catch (const std::runtime_error& e) {           //error handling
            std::cout << e.what() << '\n';
            std::cin.clear();                   //clear cin from errors
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     //clear input buffer
        }
    }
}


void GetInput(InputHandler handler) {

Token tk = handler.GetToken();      //get a token from cin
    if (tk.Tk == DateToken) {       //if is a date
        if (!handler.IsDateValid(tk.ReadingDate)) throw std::runtime_error("unvalid date");
        DatedReading reading = handler.GetDatedReading(tk.ReadingDate);
        std::cout << reading;
    }
    else {                      //if is a command run it
        RunCommand(tk, handler);
    }
    
}

void RunCommand(Token commandtk, InputHandler handler) {

    switch (commandtk.Tk) {         //switch for the command tokens
    case NewToken: {
        DatedReading input;     //get a dated reading and add it error handling is processed by the class
        std::cin >> input;
        handler.AddReading(input);
        break;
    }

    case RmToken: {
        Date date;              //get a date and use it as location to remove a reading
        std::cin >> date;
        handler.RmReading(date);
        break;
    }

    case ModToken: {            
        DatedReading input;         //get a dated input and call modify reading
        std::cin >> input;
        handler.ModReading(input);
        break;
    }

    case PrintToken:                //prints all reading
        handler.PrintDatedReadings();
        break;

    case ExportToken: {
        char confirm;
        Token parameter = handler.GetToken();
        if (parameter.Tk != ParameterToken) throw std::runtime_error("invalid parameter");
        std::cout << "are you sure you want to print the vector to the file: " << parameter.Command << "? Y to confirm\n";
        std::cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') handler.ExportVector(parameter.Command);
        break;
    }
    case QuitToken:
        quitted = true;         //quits
        break;

    default:
        throw std::runtime_error("invalid token");      //error handling
        break;
    }
}
   