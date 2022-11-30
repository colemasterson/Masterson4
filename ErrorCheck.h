/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
DESCRIPTION : ErrorCheck.h is where the ErrorCheck class is defined. 
An ErrorCheck object has 2 member values: a string errorMessage, and 
a bool isValid flag. ErrorCheck.h contains a constructor/destructor,  
and 9 public methods..         
********************************************************************/
#ifndef ERRORCHECK_H
#define ERRORCHECK_H

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

class ErrorCheck
{
    public:
        ErrorCheck();
        ~ErrorCheck();

        string nameCheck(string);
        bool flagCheck(string);
        int valCheck(string);
        bool validSymbol(string);

        bool isNum(string);
        bool isHex(string);
        bool isValidLit(string );

        void displayExp();

        string parseRFlag(bool);


        bool isValid, newR;
        string errorMessage;
    private:
};
#endif