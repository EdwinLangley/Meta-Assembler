// Conversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const string Definition = "definition.txt"; // Stores all the file locations as constant strings
const string AssInput = "New.txt"; 
const string CdmOutput = "output.cdm";


struct contents { // makes a struct with two strings
	string AssCode, HexCode;
};

struct commands { // makes a struct with two strings
	string instruction, data;
};

class action {
public:
							// All of the variables that will be needed within this class 
	action();
	void readAssFile();
	void getTranslation();
	commands inHere[100];
	contents instructionSize[10];
	string match;
	int sizeOfFile;
	int currentLine;
};

int main()
{
	action start; // creates and object of type action 
    return 0;
}

	action::action() { // This method reads in the definitions and stores it in an array of contents, the instruction and translation are at the same index position

		ifstream infile(Definition);  // loads the contents of the definition file into the ifstream object infile

		string a, b;    // defines two strings a and b 
		int i = 0; 
		while (infile >> a >> b)   // a loop to count the amount of lines in the file 
		{
			i++;
		}

		infile.clear(); //clears the position of the read pointer
		infile.seekg(0);  // sets the read pointer to line 0



		i = 0;  // Resets the counter

		while (infile >> a >> b) // iterates through each line of the defintions
		{
			instructionSize[i].AssCode = a;   // reads in the first element in every row and stores it as the first element in the struct
			instructionSize[i].HexCode = b;   // Reads in the second element (if there is one) in every row and stores 
										      // it in the second element of the array.
			i++;
		}

		infile.close();  //Closes the file to avoid corruption
		readAssFile();   // Starts next method
	}

	void action::readAssFile() {

		ifstream infile(AssInput, ifstream::in); // Reads in the assembly file

		string beforeSplit;  // a temp string that is allocated each unmodified line of the file

		int i = 0;

		while (getline(infile, beforeSplit)) { // iterates through the file and stores each line in the beforesplit string
		
			istringstream Split(beforeSplit); // an istringstream obeject is easy to manipulate so the conents of before split is loaded into it

			Split >> inHere[i].instruction;    // uses a space as a delimiter to load in the first part of each row
			Split >> inHere[i].data;		   // uses a a space as a delimiter to load in the second part of each row
			i++;
		
		}

		sizeOfFile = i; // gets the size of the assembly file
		
		infile.close(); // Closes the file

		getTranslation(); //goes to the next method

		

	}

	void action::getTranslation() {

		
		

		ofstream output(CdmOutput); //creates an output object 

		for (int i = 0; i < sizeOfFile; i++) {  

			match = inHere[i].instruction; //sets match to the current instruction to be translated
			stringstream lineStream;
			lineStream << hex << i;  // takes the value of i and converts it to its equivilent hex value
			string lineNumber = lineStream.str();  // sets the string lineNumber from a conversion from the int lineStream

			if (match != "ORG") // if the current command is ORG take this branch 
			{
				for (int j = 0; j < 10; j++)   // loops through the earlier stored definitions
				{
					if (match == instructionSize[j].AssCode) {  // if the instruction matches the one found then the appriate action is taken
						cout << lineNumber << " : " << instructionSize[j].HexCode << inHere[i].data << endl;
						output << lineNumber << " : " << instructionSize[j].HexCode << inHere[i].data << endl;
					}
				}
			}
			if(match == "ORG") // if the ORG instruction is found then the line is translated from hex to decimal and the loop is found
			{
				stringstream ss;
				ss << hex << inHere[i].data;
				ss >> i;
				i--;
			}
		}
	}
