/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
*********************************************************************  
LiteralTable.h is where the LiteralTable class is defined.
A LiteralTable object has 2 member values: a linked list of strings
used to hold the literals, and a string usermessage to display errors.
It has a constructor/destructor, 10 public methods and a Literal struct. 
********************************************************************/

#ifndef LITERALTABLE_H
#define LITERALTABLE_H

#include "ErrorCheck.h"
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Literal
{
    string line;            //stores the line read from file

    string name, opVal;     //if the literal is valid, these are populated after line is processed
    int length, address;

    string intAddress;
};

class LiteralTable
{
    public:
        LiteralTable();
        ~LiteralTable();

        void view(Literal);
        void printLitTable();
        void insertLit(string);
        bool checkDuplicate(string);
        void displayError();

        bool isValidLit(string);
        bool isHex(string);
        string assignValue(string);
        int assignLength(string);
        int assignAddress(string);
        vector<Literal> lits;

    private:
        ErrorCheck e;
};

#endif
