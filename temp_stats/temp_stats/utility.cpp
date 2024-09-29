#include "utility.h"

void error(std::string what) {
	throw std::runtime_error(what);
}
void error(std::string what, int x) {
	std::string error = what + " " + char(x + '0');
	throw std::runtime_error(error);
}

void EndOfLoop(std::istream& is, char terminator, std::string errormsg) {		//handles errors after loops of input and checks if they are for the termination of the input
	if (is.fail()) {		
		is.clear();		//clears the inputstream
		char ch;
		is >> ch;
		if (ch == terminator) return;		//checks if the character that made the input stream fail is a terminator and if it is it's all okay, if it isn't errors
		error(errormsg);
	}
}

double ToCelsius(int temperature) {		//converts F to C
	return (temperature - 32) / 1.8;
}

void SortTemperatures(std::vector<double>& temperatures) {		//sorts numbers from smallest to biggest
	std::vector<double> temperaturescopy = temperatures;
	std::vector<double> temperaturesfinal;
	double tempvar;
	bool smallest;
	for (int i = 0; i < temperaturescopy.size(); i++) {
		tempvar = temperaturescopy[i];
		smallest = true;
		for (int y = 0; y < temperaturescopy.size(); y++) {
			if (tempvar > temperaturescopy[y]) smallest = false;			//for every number it checks against the entire vector if its the smallest
		}
		if (smallest) {
			temperaturesfinal.push_back(tempvar);
			temperaturescopy.erase(temperaturescopy.begin() + i);			//if it is it pushes it into the finalvect erases it from the original and restarts the loop
			i = -1;
		}
	}
	temperatures = temperaturesfinal;
}

double CalculateMean(const std::vector<double>& temperatures) {			//calculates mean
	double sum = 0;
	int size = temperatures.size();

	for (int i = 0; i < temperatures.size(); i++) {
		sum += temperatures[i];
	}

	return (sum / size);		
}
double CalculateMedian(const std::vector<double>& temperatures) {		//calculates median temperatures
	int size = temperatures.size() - 1;

	if ((size + 1) % 2 == 0) {		//here i use the full size as i need the elements and dont need to select them
		return (temperatures[size / 2] + temperatures[(size / 2) + 1]) / 2;			//size needs to be size - 1 or big crashes happen
	}
	else {
		return temperatures[(size + 1) / 2];
	}
}