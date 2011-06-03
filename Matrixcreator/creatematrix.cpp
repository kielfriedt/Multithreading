/*
 *	Copyright (C) 2011  Kiel Friedt
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 Creates a random martix of doubles. I originally tried using the random function but found that the results were very focused.
 In an attempt to create a more random number i created a random double with a char array and then converted it to a double.
 I first generated a random number for before the decimal Ex. 12345
 placed the decimal 12345.----
 next generated a random number for after the decimal 12345.789
 
 I then iterated over the array placing random number until the number was completed. 
 I noticed that when creating random number then placing them in the array was to consistant and used this value to run
 the random number into a trash variable then use the next.
 This is way i created a more "random" number.
 
 Results may very....
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <limits>


typedef std::numeric_limits< double > dbl;

using namespace std;

/*
 generates a random double using the above description and returns.
 */
double dbrand(){
	//get number of digits before the decimal
	int beforedec = rand() % 11;
	int flag = 0, tot = 0, trash = 0;
	if(beforedec == 0){
		beforedec = 1;
		flag = 1;
	}
	//get number of digits after the decimal
	int afterdec = rand() % 16; 
	//total number of digits including decimal point
	int totaldigits = beforedec+afterdec+1;
	//create container for char double
	char number[totaldigits];
	//iterator over array inputing in values
	for (int x = 0; x < totaldigits; x++){
		// this is so we get something before the decimal point-- could be 0 or whatever i just liked 9
		if(flag == 1 && x == 0){
			number[x] = '9';
			continue;
		}
		//place decimal point
		if(x == beforedec+1){
			number[x] = '.';
			continue;
		}
		//generates random number, runs for loop then takes the random number after.
		//Helps generate a more "random" number
		tot = (rand() % 9);
		for(int y = 0; y <tot; y ++)
			trash = ((rand() % 9) + 48);
		//adds number to char array
		number[x] = (char)((rand() % 9) + 48);
		
	}
	//return "random double" from char to double
	return strtod(number,NULL);
}

/* 
 Insures that the arguments are correct and output a matrix of random double values in standard C format for arrays.
 aka double a[] = { 1, 2, 3};
 */

int main (int argc, char * const argv[])
{
	//input check for correct syntax
	if (argc <= 2 || argc > 3) {
		cout << "Syntax: ./create_matrix <Row> <Columns>\n";
		return 0;
	}
	
	//for NxM
	int Rows = atoi(argv[1]);
	int Columns = atoi(argv[2]);
	
	//for NxN
	Columns = Rows;
	
	int Total = Rows * Columns; 
	
	double RandomNum[Total];
	
	cout << "\n\ntotal: "<< Total< "\n";
	
	int NegOrNot = 0, x = 0;
	int Negative[3];
	
	//sets output
	cout.precision(16);
	//set seed for random--- aka not very random...
	srand(time(NULL));
	
	//used to make a more "random" number
	while(x < 3) {
		Negative[x] = rand() % 10 + 1;
		if(x == 2)
			if(Negative[2] == Negative[1]|| Negative[2] == Negative[0]){ 
				x = 0;
				continue;
			}
		x+=1;
	}
	
	//making some numbers negative "randomally" 
	for (int y = 0; y < Total; y++) {
		RandomNum[y] =  dbrand();
		NegOrNot = rand() % 10 + 1;	
		if(NegOrNot == Negative[0] || NegOrNot == Negative[1] || NegOrNot == Negative[2] )
			RandomNum[y] *= -1;
	}
	//pretty printing section
	cout << "\n\n\nA: \n";
	for(int j = 0; j < Total; j++){
		cout << fixed << RandomNum[j];
		
		//Make it print out so i can copy in paste into C format
		if(((j+1) % Columns) == 0 && (j+1) != Total)
			cout << ","<< "\n";
		else if((j+1) != Total)
			cout << ", ";	
	}
	
	cout << "\n\n\n S: \n";
	for(int j = 0; j < Rows; j++){
		//effort to make more distinct values
		cout << (((dbrand()/2.893)+(dbrand()/2.2343))*2.90713897129);
		if((j+1) != Rows)
			cout <<", ";
	}
	cout << "\n\n\n";
	
	return 0;
}
