#pragma once
#include <iostream>
#include <vector>


void error(std::string what);		//sends a runtime error
void error(std::string what, int x);	//sends a runtime error with a number

void EndOfLoop(std::istream& is, char terminator, std::string error);		//when input stream fails checks if it failed because the input it needed to read terminated or because of an error

double ToCelsius(int temperature);		//converts farenheight to celsius

void SortTemperatures(std::vector<double>& temperatures);		//sorts vector of numbers from smallest to biggest

double CalculateMean(const std::vector<double>& temperatures);		//calculates mean
double CalculateMedian(const std::vector<double>& temperatures);	//calculates median